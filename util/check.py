#!python

## Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
## Distributed Under The MIT License

"""
This script runs static checks on source files for line length, use of tabs, trailing whitespace, etc.
"""

import os
import sys
import re
from typing import List, Iterable

ROOT_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

HASH_CHECK_LIST = [
  "src/runtime/saynaa_core.c",
  "src/shared/saynaa_value.c",
]

CHECK_EXTENSIONS = ('.c', '.h', '.sa', '.py', '.js')

ALLOW_LONG_LINES = ('http://', 'https://', '<script ', '<link ', '<svg ')

IGNORE_FILES = (
  "src/runtime/saynaa_native.h",
  "src/optionals/saynaa_opt_term.c",
)

SOURCE_DIRS = [
  "src/cli/",
  "src/compiler/",
  "src/optionals/",
  "src/runtime/",
  "src/shared/",
  "src/utils/",
]

checks_failed = False

def to_abs_paths(sources: Iterable[str]) -> List[str]:
  """Converts relative paths to absolute paths."""
  return [os.path.join(ROOT_PATH, source) for source in sources]


def to_rel_path(path: str) -> str:
  """Converts an absolute path to a relative path from the project root."""
  return os.path.relpath(path, ROOT_PATH)


def main() -> None:
  """Main function to run all checks."""
  check_fnv1_hash(to_abs_paths(HASH_CHECK_LIST))
  check_static(to_abs_paths(SOURCE_DIRS))
  if checks_failed:
    sys.exit(1)
  print("Static checks passed.")


def check_fnv1_hash(sources: Iterable[str]) -> None:
  """Checks if FNV-1a hash values match their corresponding strings."""
  pattern = r'CHECK_HASH\(\s*"([A-Za-z0-9_]+)"\s*,\s*(0x[0-9abcdef]+)\)'
  for file in sources:
    with open(file, 'r') as fp:
      for line_no, line in enumerate(fp, start=1):
        matches = re.findall(pattern, line)
        if not matches:
          continue
        name, expected_hash = matches[0]
        computed_hash = hex(fnv1a_hash(name))
        if expected_hash != computed_hash:
          file_path = to_rel_path(file)
          report_error(
            f"{location(file_path, line_no)} - hash mismatch. "
            f"hash('{name}') = {computed_hash} not {expected_hash}"
          )


def check_static(dirs: Iterable[str]) -> None:
  """Performs static checks on source files."""
  for dir in dirs:
    for file in os.listdir(dir):
      if not file.endswith(CHECK_EXTENSIONS):
        continue
      if os.path.isdir(os.path.join(dir, file)):
        continue

      curr_file = os.path.normpath(os.path.join(dir, file))
      if any(curr_file == os.path.normpath(os.path.join(ROOT_PATH, ignore)) for ignore in IGNORE_FILES):
        continue

      with open(curr_file, 'r') as fp:
        file_path = to_rel_path(curr_file)
        lines = fp.readlines()

      is_last_empty = False
      for line_no, line in enumerate(lines, start=1):
        line = line.rstrip('\n')

        if '\t' in line:
          report_error(f"{location(file_path, line_no)} - contains tab(s) ({repr(line)}).")

        if len(line) >= 100 and not any(ignore in line for ignore in ALLOW_LONG_LINES):
          report_error(f"{location(file_path, line_no)} - contains {len(line)} (> 100) characters.")

        if line.endswith(' '):
          report_error(f"{location(file_path, line_no)} - contains trailing whitespace.")

        if line == '':
          if is_last_empty:
            report_error(f"{location(file_path, line_no)} - consecutive empty lines.")
          is_last_empty = True
        else:
          is_last_empty = False


def location(file: str, line: int) -> str:
  """Returns a formatted string of the error location."""
  return f"{file:<17} : {line:4}"


def fnv1a_hash(string: str) -> int:
  """Computes the FNV-1a hash of a string."""
  FNV_prime_32_bit = 16777619
  FNV_offset_basis_32_bit = 2166136261

  hash_value = FNV_offset_basis_32_bit
  for char in string:
    hash_value ^= ord(char)
    hash_value *= FNV_prime_32_bit
    hash_value &= 0xffffffff  # Intentional 32-bit overflow
  return hash_value


def report_error(msg: str) -> None:
  """Reports an error and sets the global flag."""
  global checks_failed
  checks_failed = True
  print(msg, file=sys.stderr)


if __name__ == '__main__':
  main()
