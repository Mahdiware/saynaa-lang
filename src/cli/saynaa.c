/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include "saynaa.h"

#include "../utils/saynaa_utils.h"

#include <stdio.h>

#define _ARGPARSE_IMPL
#include "argparse.h"
#undef _ARGPARSE_IMPL

// FIXME: Refactor `isatty` logic to a portable utility module.
// Verify portability of `<unistd.h>` and `<io.h>` across target platforms.
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#if defined(__linux__)
#include <signal.h>
static bool typeAgain = 0;

void signalHandler(int signum) {
  if (!typeAgain) {
    printf("\n\aTo exit, press ^C again or ^D or type exit();\n");
    typeAgain++;
    return;
  }
  // Perform necessary cleanup here if needed.
  // Terminate the process.
  exit(0);
}
#endif

// Initialize a new VM instance with default configuration.
static VM* initializeVM(int argc, const char** argv) {
  Configuration config = NewConfiguration();
  config.argument.argc = argc;
  config.argument.argv = argv;

  // FIXME: Implement a portable `is_tty()` wrapper.
  // Note: Windows deprecates `isatty` in favor of `_isatty`.
  if (!!isatty(fileno(stderr))) {
#ifdef _WIN32
    DWORD outmode = 0;
    HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
    GetConsoleMode(handle, &outmode);
    SetConsoleMode(handle, outmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    config.use_ansi_escape = true;
  }

  VM* vm = NewVM(&config);
  return vm;
}

int main(int argc, const char** argv) {
  // Register signal handlers for graceful termination.
#if defined(__linux__)
  signal(SIGINT, signalHandler);
  signal(SIGTSTP, signalHandler);
  signal(SIGSEGV, signalHandler);
#endif
  // Parse command line arguments.

  const char* usage[] = {
      "saynaa ... [-c cmd | file] ...",
      NULL,
  };

  nanotime_t tstart, tend;
  const char* cmd = NULL;
  int debug = false, help = false, quiet = false, version = false, millisecond = false;
  struct argparse_option cli_opts[] = {
      OPT_STRING('c', "cmd", (void*) &cmd, "Evaluate and run the passed string.", NULL, 0, 0),

      OPT_BOOLEAN('d', "debug", (void*) &debug,
                  "Compile and run the debug version.", NULL, 0, 0),

      OPT_BOOLEAN('h', "help", (void*) &help,
                  "Prints this help message and exit.", NULL, 0, 0),

      OPT_BOOLEAN('q', "quiet", (void*) &quiet,
                  "Don't print version and copyright "
                  "statement on REPL startup.",
                  NULL, 0, 0),

      OPT_BOOLEAN('v', "version", &version, "Print version and exit.", NULL, 0, 0),

      OPT_BOOLEAN('m', "ms", &millisecond, "Prints runtime millisecond.", NULL, 0, 0),

      OPT_END(),
  };

  // Parse the options.
  struct argparse argparse;
  argparse_init(&argparse, cli_opts, usage, 0);
  int arg_parse = argparse_parse(&argparse, argc, argv);

  if (help) { // --help.
    argparse_usage(&argparse);
    return 0;
  }

  if (version) { // --version
    fprintf(stdout, "%s %s\n", LANGUAGE, VERSION_STRING);
    return 0;
  }

  int exitcode = 0;

  // Create and initialize the VM.
  VM* vm = initializeVM(argc, argv);

  if (cmd != NULL) { // -c "print('foo')"
    Result result = RunString(vm, cmd);
    exitcode = (int) result;

  } else if (arg_parse == 0) { // Run on REPL mode.

    // Print the copyright and license notice, if --quiet not set.
    if (!quiet) {
      printf("%s\n", COPYRIGHT);
    }
    exitcode = RunREPL(vm);

  } else { // file. ...

    const char* file_path = argv[0];
    Result result = RunFile(vm, file_path);
    exitcode = (int) result;
  }

  if (millisecond)
    printf("runtime: %.4f ms\n", vm_time(vm));

  // Cleanup the VM and exit.
  FreeVM(vm);
  return exitcode;
}
