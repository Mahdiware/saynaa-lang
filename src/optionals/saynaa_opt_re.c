/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include "saynaa_optionals.h"

#include <regex.h>

saynaa_function(_reMatch, "re.match(pattern: String, text: String) -> Bool",
                "Returns true if the pattern matches anywhere in the string."
                " Uses POSIX Extended Regex.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  int reti;
  char msgbuf[100];

  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Invalid Regex: %s", msgbuf);
    return;
  }

  reti = regexec(&regex, text, 0, NULL, 0);
  regfree(&regex);

  if (!reti) {
    setSlotBool(vm, 0, true);
  } else if (reti == REG_NOMATCH) {
    setSlotBool(vm, 0, false);
  } else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Regex Match Error: %s", msgbuf);
  }
}

// Search: Returns the first matching substring or null.
saynaa_function(
    _reSearch, "re.search(pattern: String, text: String) -> String|Null",
    "Returns the first matching substring, or null if no match found.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  int reti;
  regmatch_t pmatch[1];
  char msgbuf[100];

  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Invalid Regex: %s", msgbuf);
    return;
  }

  reti = regexec(&regex, text, 1, pmatch, 0);
  regfree(&regex);

  if (!reti) {
    // Match found
    int len = pmatch[0].rm_eo - pmatch[0].rm_so;

    setSlotStringLength(vm, 0, text + pmatch[0].rm_so, len);
  } else if (reti == REG_NOMATCH) {
    setSlotNull(vm, 0);
  } else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Regex Search Error: %s", msgbuf);
  }
}

// Extract: Returns a list of captured groups.
saynaa_function(_reExtract, "re.extract(pattern: String, text: String) -> List|Null",
                "Returns a list of captured groups if match found."
                " Index 0 is whole match. Returns null if no match.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  int reti;
  // Support up to 10 capture groups for now
  size_t nmatch = 11;
  regmatch_t pmatch[11];
  char msgbuf[100];

  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Invalid Regex: %s", msgbuf);
    return;
  }

  reti = regexec(&regex, text, nmatch, pmatch, 0);
  regfree(&regex);

  if (!reti) {
    // Match found, create List
    NewList(vm, 0);

    for (size_t i = 0; i < nmatch; i++) {
      if (pmatch[i].rm_so == -1)
        break; // No more groups

      int len = pmatch[i].rm_eo - pmatch[i].rm_so;

      setSlotStringLength(vm, 1, text + pmatch[i].rm_so, len);
      ListInsert(vm, 0, -1, 1);
    }
  } else if (reti == REG_NOMATCH) {
    setSlotNull(vm, 0);
  } else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Regex Extract Error: %s", msgbuf);
  }
}

// FindAll: Returns a list of all matches.
saynaa_function(
    _reFindAll, "re.findall(pattern: String, text: String) -> List",
    ""
    "Returns a list of all non-overlapping matches in the string.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  int reti;
  char msgbuf[100];

  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    SetRuntimeErrorFmt(vm, "Invalid Regex: %s", msgbuf);
    return;
  }

  size_t nmatch = regex.re_nsub + 1;
  regmatch_t* pmatch = (regmatch_t*) malloc(sizeof(regmatch_t) * nmatch);

  NewList(vm, 0); // Result list at slot 0

  const char* cursor = text;
  int offset = 0;

  while (true) {
    // If the last match was empty,
    // we might need to advance manually to avoid infinite loop
    // But regexec handles empty matches.
    // If we just matched empty string at cursor, we must advance.

    int flags = 0;
    if (cursor > text)
      flags |= REG_NOTBOL;

    reti = regexec(&regex, cursor, nmatch, pmatch, flags);

    if (reti == REG_NOMATCH) {
      break;
    }

    if (reti) {
      regerror(reti, &regex, msgbuf, sizeof(msgbuf));
      SetRuntimeErrorFmt(vm, "Regex FindAll Error: %s", msgbuf);
      free(pmatch);
      regfree(&regex);
      return;
    }

    // Match found
    // Python behavior:
    // If no groups (re_nsub == 0): return list of whole matches (group 0).
    // If 1 group: return list of group 1.
    // If >1 groups: return list of tuples (Lists here).

    if (regex.re_nsub == 0) {
      int len = pmatch[0].rm_eo - pmatch[0].rm_so;
      setSlotStringLength(vm, 3, cursor + pmatch[0].rm_so, len);
      ListInsert(vm, 0, -1, 3);
    } else if (regex.re_nsub == 1) {
      // Group 1
      if (pmatch[1].rm_so != -1) {
        int len = pmatch[1].rm_eo - pmatch[1].rm_so;
        setSlotStringLength(vm, 3, cursor + pmatch[1].rm_so, len);
      } else {
        // Optional group not matched? Empty string? or null? Python uses empty string or None.
        setSlotStringLength(vm, 3, "", 0); // Use empty string for simplicity
      }
      ListInsert(vm, 0, -1, 3);
    } else {
      // Multiple groups -> List of strings
      NewList(vm, 3); // Create inner list at slot 3
      for (size_t i = 1; i <= regex.re_nsub; i++) {
        if (pmatch[i].rm_so != -1) {
          int len = pmatch[i].rm_eo - pmatch[i].rm_so;
          setSlotStringLength(vm, 4, cursor + pmatch[i].rm_so, len);
        } else {
          setSlotStringLength(vm, 4, "", 0);
        }
        ListInsert(vm, 3, -1, 4);
      }
      ListInsert(vm, 0, -1, 3);
    }

    // Correct logic: pmatch is relative to `cursor`.
    // Next search should start at `cursor + pmatch[0].rm_eo`.
    if (pmatch[0].rm_eo == 0) {
      // Empty match at start of cursor. Advance 1 char.
      if (*cursor == '\0')
        break; // End of string
      cursor++;
    } else {
      cursor += pmatch[0].rm_eo;
    }
  }

  free(pmatch);
  regfree(&regex);
}

void registerModuleRegex(VM* vm) {
  Handle* re = NewModule(vm, "re");

  ModuleAddFunction(vm, re, "match", _reMatch, 2, NULL);
  ModuleAddFunction(vm, re, "search", _reSearch, 2, NULL);
  ModuleAddFunction(vm, re, "extract", _reExtract, 2, NULL);
  ModuleAddFunction(vm, re, "findall", _reFindAll, 2, NULL);

  registerModule(vm, re);
  releaseHandle(vm, re);
}
