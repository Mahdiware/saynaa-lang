/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#ifndef __SAYNAA_READLINE__
#define __SAYNAA_READLINE__

#if defined(__linux) && defined(READLINE)

// saynaa_readline: how to show a prompt and then read a line from
// the standard input.
char* saynaa_readline(const char *listening);

// saynaa_saveline: how to "save" a input string in a "history".
void saynaa_saveline(const char *input);

#endif

#endif // __SAYNAA_READLINE__