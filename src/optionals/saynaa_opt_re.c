/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include "saynaa_optionals.h"

#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)

/* * Windows Implementation using PCRE2
 * Note: Define PCRE2_STATIC if linking against the static library.
 */
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <string.h>

// Helper to convert PCRE2 errors to VM errors
void setRegexError(VM* vm, int errornumber) {
  PCRE2_UCHAR buffer[256];
  pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
  SetRuntimeErrorFmt(vm, "Regex Error: %s", (char*) buffer);
}

saynaa_function(_reMatch, "re.match(pattern: String, text: String) -> Bool",
                "Returns true if the pattern matches anywhere in the string.") {
  const char* pattern;
  const char* text;
  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  int errornumber;
  PCRE2_SIZE erroroffset;
  pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0,
                                 &errornumber, &erroroffset, NULL);

  if (re == NULL) {
    setRegexError(vm, errornumber);
    return;
  }

  pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);
  int rc = pcre2_match(re, (PCRE2_SPTR) text, (PCRE2_SIZE) strlen(text), 0, 0,
                       match_data, NULL);

  setSlotBool(vm, 0, rc >= 0);

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
}

saynaa_function(_reSearch, "re.search(pattern: String, text: String) -> String|Null",
                "Returns the first matching substring.") {
  const char* pattern;
  const char* text;
  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  int errornumber;
  PCRE2_SIZE erroroffset;
  pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0,
                                 &errornumber, &erroroffset, NULL);
  if (!re) {
    setRegexError(vm, errornumber);
    return;
  }

  pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);
  int rc = pcre2_match(re, (PCRE2_SPTR) text, strlen(text), 0, 0, match_data, NULL);

  if (rc >= 0) {
    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
    setSlotStringLength(vm, 0, text + ovector[0], ovector[1] - ovector[0]);
  } else {
    setSlotNull(vm, 0);
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
}

saynaa_function(_reExtract, "re.extract(pattern: String, text: String) -> List|Null",
                "Returns a list of captured groups.") {
  const char* pattern;
  const char* text;
  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  int errornumber;
  PCRE2_SIZE erroroffset;
  pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0,
                                 &errornumber, &erroroffset, NULL);
  if (!re) {
    setRegexError(vm, errornumber);
    return;
  }

  pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);
  int rc = pcre2_match(re, (PCRE2_SPTR) text, strlen(text), 0, 0, match_data, NULL);

  if (rc >= 0) {
    NewList(vm, 0);
    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
    for (int i = 0; i < rc; i++) {
      setSlotStringLength(vm, 1, text + ovector[2 * i],
                          ovector[2 * i + 1] - ovector[2 * i]);
      ListInsert(vm, 0, -1, 1);
    }
  } else {
    setSlotNull(vm, 0);
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
}

saynaa_function(_reFindAll, "re.findall(pattern: String, text: String) -> List",
                "Returns all non-overlapping matches.") {
  const char* pattern;
  const char* text;
  if (!ValidateSlotString(vm, 1, &pattern, NULL))
    return;
  if (!ValidateSlotString(vm, 2, &text, NULL))
    return;

  int errornumber;
  PCRE2_SIZE erroroffset;
  pcre2_code* re = pcre2_compile((PCRE2_SPTR) pattern, PCRE2_ZERO_TERMINATED, 0,
                                 &errornumber, &erroroffset, NULL);
  if (!re) {
    setRegexError(vm, errornumber);
    return;
  }

  NewList(vm, 0);
  pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, NULL);
  PCRE2_SIZE offset = 0;
  PCRE2_SIZE len = strlen(text);

  while (offset < len) {
    int rc = pcre2_match(re, (PCRE2_SPTR) text, len, offset, 0, match_data, NULL);
    if (rc < 0)
      break;

    PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);

    // If groups exist, logic mirrors your Linux code (List of groups or single string)
    if (rc == 1) {
      setSlotStringLength(vm, 1, text + ovector[0], ovector[1] - ovector[0]);
      ListInsert(vm, 0, -1, 1);
    } else {
      NewList(vm, 1);
      for (int i = 1; i < rc; i++) {
        setSlotStringLength(vm, 2, text + ovector[2 * i],
                            ovector[2 * i + 1] - ovector[2 * i]);
        ListInsert(vm, 1, -1, 2);
      }
      ListInsert(vm, 0, -1, 1);
    }
    offset = ovector[1];
    if (ovector[0] == ovector[1])
      offset++; // Avoid infinite loop on empty matches
  }

  pcre2_match_data_free(match_data);
  pcre2_code_free(re);
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