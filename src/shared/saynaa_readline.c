/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

// TODO: i am using linux don't have macos,
// i think linux and macos use the same readline,
// if you confirm add macro defined(__APPLE__) otherwise do this right

#if defined(__linux) && defined(READLINE)

#include <readline/readline.h>
#include <readline/history.h>
#include <readline/tilde.h>

// Array of character names
char *character_names[] = {"print(", "input(", "exit();", NULL};

// Generator function for character name completion
char *character_name_generator(const char *text, int state) {
  // Variables to keep track of list and length
  static int list_index, len;
  // Name of the character
  char *name;

  // If it's the first call
  if (!state) {
    // Initialize list index
    list_index = 0;
    // Calculate the length of the input text
    len = strlen(text);
  }

  // Iterate through character names
  while ((name = character_names[list_index++])) {
    // Check if the name matches the input text
    if (strncmp(name, text, len) == 0) {
      // Return a duplicated version of the matched name
      return strdup(name);
    }
  }

  // Return NULL when no more matches are found
  return NULL;
}

// Completion function for character names
char **character_name_completion(const char *text, int start, int end) {
  // Signal completion is over
  rl_attempted_completion_over = 1;
  // Call the completion generator
  return rl_completion_matches(text, character_name_generator);
}

char* saynaa_readline(char *listening) {
  rl_readline_name="saynaa";
  // Enable history
  using_history();

  // Set up readline to handle left button
  // Enable left button support
  rl_bind_key('\e', rl_insert);

  // Bind tab key for completion
  rl_bind_key('\t', rl_complete);

  // Set the completion function
  rl_attempted_completion_function = character_name_completion;

  char *input = readline(listening);

  if (input[0] != '\0')
    add_history(input);

  return input;
}

void saynaa_saveline(char *input) {
  if (input[0] == '\0')
    return;

  // Enable history
  using_history();
  add_history(input);
}

#endif
