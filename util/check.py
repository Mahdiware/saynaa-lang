#!/usr/bin/env python3

import sys
import os
import re
import argparse
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor

# -----------------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------------

SOURCE_DIRS = [
    "src/cli",
    "src/compiler",
    "src/optionals",
    "src/runtime",
    "src/shared",
    "src/utils",
]

HASH_CHECK_FILES = [
    "src/runtime/saynaa_core.c",
    "src/shared/saynaa_value.c",
]

CHECK_EXTENSIONS = {'.c', '.h', '.sa', '.py', '.js'}

IGNORE_FILES = {
    "src/runtime/saynaa_native.h",
    "src/optionals/saynaa_opt_term.c",
}

IGNORE_FOLDERS = {
    "src/optionals/thirdparty/",
}

ALLOW_LONG_LINES_PREFIXES = ('http://', 'https://', '<script ', '<link ', '<svg ')

MAX_LINE_LENGTH = 100

# -----------------------------------------------------------------------------
# Utilities
# -----------------------------------------------------------------------------

class Colors:
    HEADER = '\033[95m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    
    if not sys.stdout.isatty() or os.environ.get('NO_COLOR'):
        HEADER = OKGREEN = WARNING = FAIL = ENDC = BOLD = ''

def get_root_dir():
    return Path(__file__).parent.parent.resolve()

def resolve_paths(root, relative_paths):
    return [root / p for p in relative_paths]

# -----------------------------------------------------------------------------
# Logic
# -----------------------------------------------------------------------------

class Issue:
    def __init__(self, file, line_no, message, is_error=True):
        self.file = file
        self.line_no = line_no
        self.message = message
        self.is_error = is_error

    def __str__(self):
        ctx = f"{self.file.relative_to(get_root_dir())}:{self.line_no}"
        color = Colors.FAIL if self.is_error else Colors.WARNING
        return f"{Colors.BOLD}{ctx:<25}{Colors.ENDC} {color}{self.message}{Colors.ENDC}"

class CheckResult:
    def __init__(self, file):
        self.file = file
        self.issues = []
        self.fixed = False

    def add_error(self, line, msg):
        self.issues.append(Issue(self.file, line, msg, True))

    def has_errors(self):
        return any(i.is_error for i in self.issues)

# --- FNV-1a Hash Checker ---

def fnv1a_hash(string):
    """Computes the FNV-1a hash of a string."""
    FNV_prime_32_bit = 16777619
    FNV_offset_basis_32_bit = 2166136261
    
    hash_value = FNV_offset_basis_32_bit
    for char in string:
        hash_value ^= ord(char)
        hash_value *= FNV_prime_32_bit
        hash_value &= 0xffffffff
    return hash_value

def check_hashes(file_path):
    result = CheckResult(file_path)
    if not file_path.exists():
        result.add_error(0, f"File not found for hash check")
        return result

    # Pattern: CHECK_HASH("name", 0x123...)
    pattern = re.compile(r'CHECK_HASH\(\s*"([A-Za-z0-9_]+)"\s*,\s*(0x[0-9abcdef]+)\)', re.IGNORECASE)
    
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            for line_no, line in enumerate(f, start=1):
                matches = pattern.findall(line)
                for name, expected_hash_str in matches:
                    expected_hash = int(expected_hash_str, 16)
                    computed_hash = fnv1a_hash(name)
                    
                    if expected_hash != computed_hash:
                        result.add_error(line_no, 
                            f"Hash mismatch for '{name}'. Expected {hex(expected_hash)}, Got {hex(computed_hash)}")
    except Exception as e:
        result.add_error(0, f"Error reading file: {e}")
        
    return result

# --- Style Checker ---

def check_style(file_path, fix=False):
    result = CheckResult(file_path)
    
    # Check if ignored
    rel_path = file_path.relative_to(get_root_dir()).as_posix()
    if rel_path in IGNORE_FILES or any(rel_path.startswith(folder) for folder in IGNORE_FOLDERS):
        return result

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            
        new_lines = []
        is_last_empty = False
        file_changed = False
        
        for line_no, line in enumerate(lines, start=1):
            original_line = line
            stripped_content = line.rstrip('\n')
            
            # 1. Check Tabs
            if '\t' in line:
                result.add_error(line_no, "Contains tab character (\t)")
                if fix:
                    line = line.replace('\t', '  ')
                    file_changed = True

            # 2. Check Line Length
            # Don't check length if it contains long string literals like URLs
            if len(stripped_content) > MAX_LINE_LENGTH:
                if not any(prefix in line for prefix in ALLOW_LONG_LINES_PREFIXES):
                    result.add_error(line_no, f"Line too long ({len(stripped_content)} > {MAX_LINE_LENGTH})")

            # 3. Trailing Whitespace
            if stripped_content.endswith(' '):
                result.add_error(line_no, "Trailing whitespace")
                if fix:
                    line = line.rstrip() + '\n'
                    file_changed = True

            # 4. Consecutive Empty Lines
            if stripped_content == '':
                if is_last_empty:
                    result.add_error(line_no, "Consecutive empty lines")
                    if fix:
                        # Skip adding this line to new_lines
                        file_changed = True
                        continue 
                is_last_empty = True
            else:
                is_last_empty = False

            new_lines.append(line)
            
        if fix and file_changed:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.writelines(new_lines)
            result.fixed = True
            
    except UnicodeDecodeError:
        result.add_error(0, "Binary or non-UTF-8 file detected")
    except Exception as e:
        result.add_error(0, f"Error processing file: {e}")

    return result

# -----------------------------------------------------------------------------
# Main Runner
# -----------------------------------------------------------------------------

def collect_files(root_dir, explicit_paths=None):
    files = set()
    
    if explicit_paths:
        for p in explicit_paths:
            path = Path(p).resolve()
            if path.is_file():
                files.add(path)
            elif path.is_dir():
                for ext in CHECK_EXTENSIONS:
                    files.update(path.rglob(f"*{ext}"))
    else:
        # Default source dirs
        for d in SOURCE_DIRS:
            path = root_dir / d
            if path.exists():
                for ext in CHECK_EXTENSIONS:
                    files.update(path.rglob(f"*{ext}"))
                    
    # Filter out hidden or build files if needed
    return sorted(list(files))

def main():
    parser = argparse.ArgumentParser(description="Saynaa Static Code Analyzer")
    parser.add_argument('paths', nargs='*', help="Files or directories to check")
    parser.add_argument('--fix', action='store_true', help="Automatically fix simple style issues (whitespace)")
    parser.add_argument('-v', '--verbose', action='store_true', help="Verbose output")
    args = parser.parse_args()

    root_dir = get_root_dir()
    print(f"{Colors.HEADER}Saynaa Code Style Checker{Colors.ENDC}")
    print(f"Root: {root_dir}")
    
    # 1. Run Hash Checks
    print(f"\n{Colors.BOLD}>> Verifying Hashes...{Colors.ENDC}")
    hash_files = resolve_paths(root_dir, HASH_CHECK_FILES)
    hash_failed = False
    
    with ThreadPoolExecutor() as executor:
        results = list(executor.map(check_hashes, hash_files))
        
    for res in results:
        for issue in res.issues:
            print(issue)
            hash_failed = True
            
    if not hash_failed:
        print(f"{Colors.OKGREEN}Hashes OK{Colors.ENDC}")

    # 2. Run Style Checks
    print(f"\n{Colors.BOLD}>> Checking Code Style...{Colors.ENDC}")
    if args.fix:
        print(f"{Colors.WARNING}(Fix mode enabled){Colors.ENDC}")

    check_files = collect_files(root_dir, args.paths)
    print(f"Scanning {len(check_files)} files...")
    
    style_failed = False
    fixed_count = 0
    
    with ThreadPoolExecutor() as executor:
        # Pass 'fix' arg using lambda or partial, but map works easiest with helper
        futures = [executor.submit(check_style, f, args.fix) for f in check_files]
        
        for future in futures:
            res = future.result()
            if res.fixed:
                fixed_count += 1
                print(f"{Colors.OKGREEN}Fixed: {res.file.relative_to(root_dir)}{Colors.ENDC}")
                
            if res.has_errors():
                style_failed = True
                for issue in res.issues:
                    print(issue)

    print("-" * 60)
    if hash_failed or style_failed:
        print(f"{Colors.FAIL}Checks Failed!{Colors.ENDC}")
        sys.exit(1)
    else:
        print(f"{Colors.OKGREEN}All Checks Passed.{Colors.ENDC}")
        if fixed_count > 0:
            print(f"Fixed {fixed_count} files.")
        sys.exit(0)

if __name__ == '__main__':
    main()
