#!/usr/bin/env python

import os
import re
import sys
import platform
from argparse import ArgumentParser
from subprocess import Popen, PIPE
from threading import Timer
from os.path import abspath, basename, dirname, isdir, isfile, join, realpath, relpath, splitext

## ----------------------------------------------------------------------------

DIR = dirname(dirname(realpath(__file__)))
APP = join(DIR, 'saynaa')

APP_WITH_EXT = APP
if platform.system() == "Windows":
  APP_WITH_EXT += ".exe"

if not isfile(APP_WITH_EXT):
  print("The binary file 'saynaa' was not found, expected it to be at " + APP)
  print("In order to run the tests, you need to build saynaa first!")
  sys.exit(1)

passed = 0
failed = 0

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
    # Invoke saynaa and run the test.
    test_arg = self.path
    proc = Popen([app, test_arg], stdin=PIPE, stdout=PIPE, stderr=PIPE)

    # If a test takes longer than five seconds, kill it.
    #
    # This is mainly useful for running the tests while stress testing the GC,
    # which can make a few pathological tests much slower.
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
    if self.compile_errors and self.runtime_error_message:
      self.fail("Test error: Cannot expect both compile and runtime errors.")
      return

    try:
      out = out.decode("utf-8").replace('\r\n', '\n')
      err = err.decode("utf-8").replace('\r\n', '\n')
    except:
      self.fail('Error decoding output.')

    error_lines = err.split('\n')

    self.validate_exit_code(exit_code, error_lines)

  def validate_exit_code(self, exit_code, error_lines):
    if exit_code == self.exit_code: return

    self.failures += error_lines

  def fail(self, message):
    self.failures.append(message)


def color_text(text, color):
  """Converts text to a string and wraps it in the ANSI escape sequence for
  color, if supported."""
  return color + str(text) + '\u001b[0m'

def green(text):  return color_text(text, '\u001b[32m')
def pink(text):   return color_text(text, '\u001b[91m')
def red(text):    return color_text(text, '\u001b[31m')
def yellow(text): return color_text(text, '\u001b[33m')

def walk(dir, ignored=None):
  if not ignored:
    ignored = []
  ignored += [".",".."]

  dir = abspath(dir)
  for file in [file for file in os.listdir(dir) if not file in ignored]:
    nfile = join(dir, file)
    if isdir(nfile):
      walk(nfile)
    else:
      run_script(nfile)

def print_line(line=None):
  # Erase the line.
  print('\u001b[2K', end='')
  # Move the cursor to the beginning.
  print('\r', end='')
  if line:
    print(line, end='')
    sys.stdout.flush()

def run_script(path):
  global APP
  global passed
  global failed

  if (splitext(path)[1] != '.sa'):
    return

  # Update the status line.
  print_line('({}) Passed: {} Failed: {} '.format(
      relpath(APP, DIR), green(passed), red(failed)))

  # Make a nice short path relative to the working directory.

  # Normalize it to use "/" since, among other things, saynaa expects its argument
  # to use that.
  path = relpath(path).replace("\\", "/")

  # Read the test
  test = Test(path)
  test.run(APP)

  # Display the results.
  if len(test.failures) == 0:
    passed += 1
  else:
    failed += 1
    print_line(red('FAIL') + ':')

    for failure in test.failures:
      print('  ' + pink(failure))
    print('')

def main():
  print_line()

  if failed == 0:
    print('All ' + green(passed) + ' tests passed.')
  else:
    print(green(passed) + ' tests passed. ' + red(failed) + ' tests failed.')
    sys.exit(1)

## ----------------------------------------------------------------------------

if __name__ == '__main__':
  ## This will enable ANSI codes in windows terminal.
  os.system('')
  print('')
  walk(join(DIR, 'test'), ignored=['example'])
  main()
