#!/usr/bin/env python3

import sys
import os
import re
import shlex
import shutil
import platform
import argparse
import subprocess
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

# -----------------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------------
DEFAULT_TIMEOUT = 5.0  # Seconds per test

# -----------------------------------------------------------------------------
# Colors and formatting
# -----------------------------------------------------------------------------
class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    # Disable colors if not a TTY or explicitly disabled
    if not sys.stdout.isatty() or os.environ.get('NO_COLOR'):
        HEADER = OKBLUE = OKCYAN = OKGREEN = WARNING = FAIL = ENDC = BOLD = UNDERLINE = ''

# -----------------------------------------------------------------------------
# Test Expectation Parser
# -----------------------------------------------------------------------------
class TestExp:
    def __init__(self):
        self.expect_output = []
        self.expect_runtime_error = None
        self.expect_exit_code = 0
        self.skip = False

    @staticmethod
    def parse(filepath):
        exp = TestExp()
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                line = line.strip()
                # Parse expectations from comments.
                # Supported formats:
                #   # expect: <text>          -> Expect line in stdout
                #   # expect error: <text>    -> Expect substring in stderr
                #   # expect exit: <int>      -> Expect exit code
                #   # skip                    -> Skip test
                
                if '#' not in line:
                    continue
                
                # Extract comment part
                comment = line.split('#', 1)[1].strip()
                
                if comment.startswith('expect:'):
                    exp.expect_output.append(comment[7:].strip())
                elif comment.startswith('expect error:'):
                    exp.expect_runtime_error = comment[13:].strip()
                elif comment.startswith('expect exit:'):
                    try:
                        exp.expect_exit_code = int(comment[12:].strip())
                    except ValueError:
                        pass
                elif comment.startswith('skip'):
                    exp.skip = True
                    
        return exp

# -----------------------------------------------------------------------------
# Test Runner Logic
# -----------------------------------------------------------------------------
class TestResult:
    def __init__(self, file, success, message, duration):
        self.file = file
        self.success = success
        self.message = message
        self.duration = duration

def run_single_test(test_file, interpreter, timeout):
    if not test_file.exists():
        return TestResult(test_file, False, f"File not found: {test_file}", 0)
    
    exp = TestExp.parse(test_file)
    if exp.skip:
        return TestResult(test_file, True, "SKIPPED", 0)

    start_time = time.time()
    
    try:
        proc = subprocess.Popen(
            [str(interpreter), str(test_file)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin=subprocess.PIPE,
            text=True
        )
        
        try:
            stdout, stderr = proc.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            proc.kill()
            return TestResult(test_file, False, "Timed out", time.time() - start_time)
            
        duration = time.time() - start_time
        
        # Validation Logic
        failures = []
        
        # 1. Check Exit Code
        # If specific exit code expected, check it.
        # If 'expect error' is present, we generally assume non-zero exit code is okay unless 0 is enforced.
        if proc.returncode != exp.expect_exit_code:
            # If we expected a runtime error, typically the app exits with non-zero.
            # If default (0) was expected but we got error, that's a fail.
            # Only ignore mismatch if we expected an error AND didn't specify an exit code.
            if not (exp.expect_runtime_error and exp.expect_exit_code == 0):
                failures.append(f"Exit Code: Expected {exp.expect_exit_code}, got {proc.returncode}")

        # 2. Check Standard Output (stdout)
        # We verify that 'expect:' lines appear in stdout in the correct relative order.
        if exp.expect_output:
            out_lines = stdout.splitlines()
            out_idx = 0
            for eline in exp.expect_output:
                found = False
                while out_idx < len(out_lines):
                    oline = out_lines[out_idx].strip()
                    out_idx += 1
                    if oline == eline:
                        found = True
                        break
                if not found:
                    failures.append(f"Missing expected output: '{eline}'")
                    break

        # 3. Check Standard Error (stderr) or Runtime Errors
        if exp.expect_runtime_error:
            if exp.expect_runtime_error not in stderr:
                failures.append(f"Missing expected error in stderr: '{exp.expect_runtime_error}'")
        else:
            # If we didn't expect an error, but got a crash (non-zero exit) or stderr output
            # (assuming stderr is clean on success)
            if proc.returncode != 0 and len(failures) == 0:
                 # Only report if we haven't already reported exit code mismatch (to avoid duplicate noise)
                 pass

        if failures:
            msg = "\n".join(failures)
            if stderr and not exp.expect_runtime_error:
                 msg += f"\n\n--- Stderr ---\n{stderr}"
            if stdout and not exp.expect_output:
                 # Optionally show stdout on failure for debugging
                 # msg += f"\n\n--- Stdout ---\n{stdout}"
                 pass
            # Include input file path for easier debugging copy/paste
            msg = f"Failed: {test_file}\n" + msg
            return TestResult(test_file, False, msg, duration)
            
        return TestResult(test_file, True, "", duration)

    except Exception as e:
        return TestResult(test_file, False, f"Runner internal error: {str(e)}", 0)

# -----------------------------------------------------------------------------
# Main Entry Point
# -----------------------------------------------------------------------------
def scan_tests(root_dir):
    files = []
    for r, d, f in os.walk(root_dir):
        # Skip example directory and hidden dirs
        if 'example' in r or '/.' in str(Path(r).as_posix()): 
            continue
            
        for file in f:
            if file.endswith('.sa'):
                files.append(Path(r) / file)
    return sorted(files)

def main():
    parser = argparse.ArgumentParser(description="Saynaa Language Test Runner")
    parser.add_argument('--app', default=None, help="Path to Saynaa executable")
    parser.add_argument('tests', nargs='*', help="Specific test files or directories")
    parser.add_argument('-j', '--jobs', type=int, default=os.cpu_count() or 4, help="Number of parallel jobs")
    parser.add_argument('-v', '--verbose', action='store_true', help="Show all output")
    args = parser.parse_args()

    # Determine interpreter path
    root_dir = Path(__file__).parent.parent.resolve()
    interpreter = args.app
    if not interpreter:
        exe_name = "saynaa.exe" if platform.system() == "Windows" else "saynaa"
        interpreter = root_dir / exe_name
    
    interpreter = Path(interpreter).resolve()
    if not interpreter.exists():
        print(f"{Colors.FAIL}Error: Saynaa binary not found at: {interpreter}{Colors.ENDC}")
        print(f"Please build the project first or specify --app")
        sys.exit(1)
        
    print(f"{Colors.HEADER}Saynaa Test Runner{Colors.ENDC}")
    print(f"Interpreter: {Colors.BOLD}{interpreter}{Colors.ENDC}")
    
    # Collect tests
    test_files = []
    if args.tests: # Specific tests requested
        for t in args.tests:
            p = Path(t).resolve()
            if p.is_file():
                test_files.append(p)
            elif p.is_dir():
                test_files.extend(scan_tests(p))
            else:
                # Try relative to workspace root if not found
                p2 = root_dir / t
                if p2.exists():
                    test_files.append(p2)
                else:
                    print(f"{Colors.WARNING}Warning: Test path not found: {t}{Colors.ENDC}")
    else:
        # Default: everything in tests/
        test_dir = root_dir / 'test'
        if not test_dir.exists():
             print(f"{Colors.FAIL}Error: 'test' directory not found at {test_dir}{Colors.ENDC}")
             sys.exit(1)
        test_files = scan_tests(test_dir)

    print(f"Queueing {len(test_files)} tests with {args.jobs} threads...\n")
    
    start_all = time.time()
    passed = 0
    failed = 0
    skipped = 0
    results = []
    
    # Use simple Lock for printing to avoid mixed output lines
    print_lock = threading.Lock()

    def handle_result(res):
        nonlocal passed, failed, skipped
        if res.message == "SKIPPED":
            symbol = f"{Colors.WARNING}SKIP{Colors.ENDC}"
            skipped += 1
        elif res.success:
            symbol = f"{Colors.OKGREEN}PASS{Colors.ENDC}"
            passed += 1
        else:
            symbol = f"{Colors.FAIL}FAIL{Colors.ENDC}"
            failed += 1
            
        rel_path = res.file.relative_to(root_dir)
        
        with print_lock:
            # Print single line status
            print(f"[{symbol}] {rel_path} ({res.duration:.3f}s)")

    # Execute
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        futures = [executor.submit(run_single_test, tf, interpreter, DEFAULT_TIMEOUT) for tf in test_files]
        for future in futures:
            res = future.result()
            results.append(res)
            handle_result(res)
            
    total_time = time.time() - start_all
    
    # Summary
    print("\n" + "="*60)
    print(f"Summary: {Colors.OKGREEN}{passed} passed{Colors.ENDC}, "
          f"{Colors.FAIL}{failed} failed{Colors.ENDC}, "
          f"{Colors.WARNING}{skipped} skipped{Colors.ENDC}")
    print(f"Total time: {total_time:.2f}s")
    
    if failed > 0:
        print(f"\n{Colors.BOLD}Failure Details:{Colors.ENDC}")
        for res in results:
            if not res.success and res.message != "SKIPPED":
                rel_path = res.file.relative_to(root_dir)
                print(f"\n{Colors.FAIL}>>> {rel_path}{Colors.ENDC}")
                print(res.message)
        sys.exit(1)
    
    sys.exit(0)

if __name__ == '__main__':
    main()
