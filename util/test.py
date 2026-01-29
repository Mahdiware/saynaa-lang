#!/usr/bin/env python

import os
import platform
import sys
from argparse import ArgumentParser
from subprocess import Popen, PIPE
from threading import Timer
from os.path import abspath, dirname, isdir, isfile, join, realpath, relpath, splitext

# -----------------------------------------------------------------------------
# Constants and Globals

DIR = dirname(dirname(realpath(__file__)))
APP = join(DIR, 'saynaa')

APP_WITH_EXT = APP + ".exe" if platform.system() == "Windows" else APP

if not isfile(APP_WITH_EXT):
  print(f"The binary file 'saynaa' was not found, expected it to be at {APP}")
  print("In order to run the tests, you need to build saynaa first!")
  sys.exit(1)

passed = 0
failed = 0

# -----------------------------------------------------------------------------
# Test Class

class Test:
  def __init__(self, path):
    self.path = path
    self.output = []
    self.compile_errors = set()
    self.runtime_error_line = 0
    self.runtime_error_message = None
    self.exit_code = 0
    self.input_bytes = None
    self.failures = []

  def run(self, app):
    """Run the test using the provided application binary."""
    proc = Popen([app, self.path], stdin=PIPE, stdout=PIPE, stderr=PIPE)

    # Kill the process if it takes longer than 5 seconds.
    timed_out = [False]

    def kill_process(p):
      timed_out[0] = True
      p.kill()

    timer = Timer(5, kill_process, [proc])

    try:
      timer.start()
      out, err = proc.communicate(self.input_bytes)

      if timed_out[0]:
        self.fail("Timed out.")
      else:
        self.validate(proc.returncode, out, err)
    finally:
      timer.cancel()

  def validate(self, exit_code, out, err):
    """Validate the test results."""
    if self.compile_errors and self.runtime_error_message:
      self.fail("Test error: Cannot expect both compile and runtime errors.")
      return

    try:
      out = out.decode("utf-8").replace('\r\n', '\n')
      err = err.decode("utf-8").replace('\r\n', '\n')
    except UnicodeDecodeError:
      self.fail("Error decoding output.")
      return

    error_lines = err.split('\n')
    self.validate_exit_code(exit_code, error_lines)

  def validate_exit_code(self, exit_code, error_lines):
    """Validate the exit code."""
    if exit_code != self.exit_code:
      self.failures += error_lines

  def fail(self, message):
    """Record a failure message."""
    self.failures.append(message)

# -----------------------------------------------------------------------------
# Utility Functions

def color_text(text, color):
  """Wrap text in ANSI escape sequences for color."""
  return f"{color}{text}\u001b[0m"

def green(text):  return color_text(text, '\u001b[32m')
def pink(text):   return color_text(text, '\u001b[91m')
def red(text):    return color_text(text, '\u001b[31m')
def yellow(text): return color_text(text, '\u001b[33m')

def walk(dir, ignored=None):
  """Recursively walk through a directory and run tests."""
  ignored = ignored or []
  ignored += [".", ".."]

  dir = abspath(dir)
  for file in os.listdir(dir):
    if file in ignored:
      continue

    nfile = join(dir, file)
    if isdir(nfile):
      walk(nfile, ignored)
    else:
      run_script(nfile)

def print_line(line=None):
  """Print a status line."""
  print('\u001b[2K', end='')  # Erase the line
  print('\r', end='')         # Move the cursor to the beginning
  if line:
    print(line, end='')
    sys.stdout.flush()

def run_script(path):
  """Run a single test script."""
  global passed, failed

  if splitext(path)[1] != '.sa':
    return

  # Update the status line.
  print_line(f"({relpath(APP, DIR)}) Passed: {green(passed)} Failed: {red(failed)} ")

  # Normalize the path to use "/" for compatibility.
  path = relpath(path).replace("\\", "/")

  # Run the test.
  test = Test(path)
  test.run(APP)

  # Display the results.
  if not test.failures:
    passed += 1
  else:
    failed += 1
    print_line(red("FAIL") + ":")
    for failure in test.failures:
      print(f"  {pink(failure)}")
    print("")

# -----------------------------------------------------------------------------
# Main Function

def main():
  """Main entry point for the script."""
  print_line()

  if failed == 0:
    print(f"All {green(passed)} tests passed.")
  else:
    print(f"{green(passed)} tests passed. {red(failed)} tests failed.")
    sys.exit(1)

# -----------------------------------------------------------------------------
# Entry Point

if __name__ == '__main__':
  # Enable ANSI codes in Windows terminal.
  os.system('')
  print("")
  walk(join(DIR, 'test'), ignored=['example'])
  main()
