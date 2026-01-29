/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include "saynaa_optionals.h"

#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)

/* Stubs for platforms without POSIX regex.h (Windows) */
saynaa_function(_reMatch, "re.match(pattern: String, text: String) -> Bool",
                "Unavailable on Windows.") {
  SetRuntimeErrorFmt(
      vm,
      "Regex module requires POSIX regex.h, which is unavailable on Windows.");
}

saynaa_function(_reSearch, "re.search(pattern: String, text: String) -> String|Null",
                "Unavailable on Windows.") {
  SetRuntimeErrorFmt(vm, "Regex module unavailable on Windows.");
}

saynaa_function(_reExtract, "re.extract(pattern: String, text: String) -> List|Null",
                "Unavailable on Windows.") {
  SetRuntimeErrorFmt(vm, "Regex module unavailable on Windows.");
}

saynaa_function(_reFindAll, "re.findall(pattern: String, text: String) -> List",
                "Unavailable on Windows.") {
  SetRuntimeErrorFmt(vm, "Regex module unavailable on Windows.");
}

#else /* POSIX builds (Linux, macOS, BSD) */

#include <regex.h>

// Match: Returns true if the pattern matches anywhere in the string.
saynaa_function(_reMatch, "re.match(pattern: String, text: String) -> Bool",
                "Returns true if the pattern matches the text.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  int reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    char msg[128];
    regerror(reti, &regex, msg, sizeof(msg));
    SetRuntimeErrorFmt(vm, "Invalid Regex: %s", msg);
    return;
  }

  reti = regexec(&regex, text, 0, NULL, 0);
  regfree(&regex);

  setSlotBool(vm, 0, (reti == 0));
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
  regmatch_t pmatch[1];
  int reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    SetRuntimeErrorFmt(vm, "Invalid Regex pattern.");
    return;
  }

  reti = regexec(&regex, text, 1, pmatch, 0);
  if (!reti) {
    setSlotStringLength(vm, 0, text + pmatch[0].rm_so,
                        pmatch[0].rm_eo - pmatch[0].rm_so);
  } else {
    setSlotNull(vm, 0);
  }

  regfree(&regex);
}

// Extract: Returns a list of captured groups.
saynaa_function(
    _reExtract, "re.extract(pattern: String, text: String) -> List|Null",
    "Returns a list of captured groups. Index 0 is the full match.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    SetRuntimeErrorFmt(vm, "Could not compile regex.");
    return;
  }

  size_t nmatch = regex.re_nsub + 1;
  regmatch_t* pmatch = malloc(sizeof(regmatch_t) * nmatch);

  int reti = regexec(&regex, text, nmatch, pmatch, 0);
  if (!reti) {
    NewList(vm, 0); // Result at slot 0
    for (size_t i = 0; i < nmatch; i++) {
      if (pmatch[i].rm_so != -1) {
        setSlotStringLength(vm, 1, text + pmatch[i].rm_so,
                            pmatch[i].rm_eo - pmatch[i].rm_so);
        ListInsert(vm, 0, -1, 1);
      } else {
        setSlotNull(vm, 1);
        ListInsert(vm, 0, -1, 1);
      }
    }
  } else {
    setSlotNull(vm, 0);
  }

  free(pmatch);
  regfree(&regex);
}

// FindAll: Returns a list of all matches.
saynaa_function(_reFindAll, "re.findall(pattern: String, text: String) -> List",
                "Returns a list of all non-overlapping matches.") {
  const char* pattern;
  const char* text;

  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  regex_t regex;
  if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
    SetRuntimeErrorFmt(vm, "Invalid Regex.");
    return;
  }

  size_t nmatch = regex.re_nsub + 1;
  regmatch_t* pmatch = malloc(sizeof(regmatch_t) * nmatch);
  NewList(vm, 0); // Result List

  const char* cursor = text;
  while (regexec(&regex, cursor, nmatch, pmatch, 0) == 0) {
    // Determine what to add to the list based on capture groups
    if (regex.re_nsub == 0) {
      // No groups: Add the whole match
      setSlotStringLength(vm, 3, cursor + pmatch[0].rm_so,
                          pmatch[0].rm_eo - pmatch[0].rm_so);
      ListInsert(vm, 0, -1, 3);
    } else if (regex.re_nsub == 1) {
      // One group: Add just that group
      setSlotStringLength(vm, 3, cursor + pmatch[1].rm_so,
                          pmatch[1].rm_eo - pmatch[1].rm_so);
      ListInsert(vm, 0, -1, 3);
    } else {
      // Multiple groups: Add a sub-list (tuple-like)
      NewList(vm, 3);
      for (size_t i = 1; i <= regex.re_nsub; i++) {
        setSlotStringLength(vm, 4, cursor + pmatch[i].rm_so,
                            pmatch[i].rm_eo - pmatch[i].rm_so);
        ListInsert(vm, 3, -1, 4);
      }
      ListInsert(vm, 0, -1, 3);
    }

    // Advance cursor. Handle empty matches to prevent infinite loops.
    int match_len = pmatch[0].rm_eo;
    cursor += (match_len > 0) ? match_len : 1;
    if (*(cursor - (match_len > 0 ? 0 : 1)) == '\0')
      break;
  }

  free(pmatch);
  regfree(&regex);
}

#endif /* POSIX builds */

void registerModuleRegex(VM* vm) {
  Handle* re = NewModule(vm, "re");
  ModuleAddFunction(vm, re, "match", _reMatch, 2, NULL);
  ModuleAddFunction(vm, re, "search", _reSearch, 2, NULL);
  ModuleAddFunction(vm, re, "extract", _reExtract, 2, NULL);
  ModuleAddFunction(vm, re, "findall", _reFindAll, 2, NULL);
  registerModule(vm, re);
  releaseHandle(vm, re);
}