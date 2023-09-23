/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include <stdio.h>
#include "saynaa.h"

// FIXME: Everything below here is temporary and for testing.

#if defined(__GNUC__)
  #pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
  #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(__clang__)
  #pragma clang diagnostic ignored "-Wint-to-pointer-cast"
  #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(_MSC_VER)
  #pragma warning(disable:26812)
#endif

#define BegMillisec      \
  if(millisecond) {      \
    tstart = nanotime(); \
  }

#define EndMillisec                                        \
  if(millisecond) {                                        \
    tend = nanotime();                                     \
    printf("runtime: %.4f ms\n", millitime(tstart, tend)); \
  }

#define _ARGPARSE_IMPL
#include "argparse.h"
#undef _ARGPARSE_IMPL

// FIXME:
// Included for isatty(). This should be moved to somewhere. and I'm not sure
// about the portability of these headers. and isatty() function.
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

  void signalHandler(int signum)
  {
    if(!typeAgain){
      printf("\n\aTo exit, press ^C again or ^D or type exit();\n");
      typeAgain++;
      return;
    }
    // You can perform additional cleanup or actions here if needed
    // ...
    // Exit the program
    exit(0);
  }
#endif

// Create new VM and set it's configuration.
static VM* intializeVM(int argc, const char** argv) {

  Configuration config = NewConfiguration();
  config.argument.argc = argc;
  config.argument.argv = argv;

// FIXME:
// Refactor and make it portable. Maybe custom is_tty() function?.
// Windows isatty depricated -- use _isatty.
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

  // Register the signal handler
  #if defined(__linux__)
  // signal(SIGINT, signalHandler);
  // signal(SIGTSTP, signalHandler);
  // signal(SIGSEGV, signalHandler);
  #endif
  // Parse command line arguments.

  const char* usage[] = {
    "saynaa ... [-c cmd | file] ...",
    NULL,
  };

  nanotime_t tstart, tend;
  const char* cmd = NULL;
  int debug = false, help = false, quiet = false,
      version = false, millisecond = false;
  struct argparse_option cli_opts[] = {
      OPT_STRING('c', "cmd", (void*)&cmd,
        "Evaluate and run the passed string.", NULL, 0, 0),

      OPT_BOOLEAN('d', "debug", (void*)&debug,
        "Compile and run the debug version.", NULL, 0, 0),

      OPT_BOOLEAN('h', "help",  (void*)&help,
        "Prints this help message and exit.", NULL, 0, 0),

      OPT_BOOLEAN('q', "quiet", (void*)&quiet,
        "Don't print version and copyright statement on REPL startup.",
        NULL, 0, 0),

      OPT_BOOLEAN('v', "version", &version,
        "Print version and exit.", NULL, 0, 0),

      OPT_BOOLEAN('m', "ms", &millisecond,
        "Prints runtime millisecond.", NULL, 0, 0),

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

  // Create and initialize VM.
  VM* vm = intializeVM(argc, argv);

  if (cmd != NULL) { // -c "print('foo')"
    BegMillisec;
    Result result = RunString(vm, cmd);
    EndMillisec;
    exitcode = (int) result;

  } else if (arg_parse == 0) { // Run on REPL mode.

   // Print the copyright and license notice, if --quiet not set.
    if (!quiet) {
      printf("%s\n", COPYRIGHT);
    }
    exitcode = RunREPL(vm);

  } else { // file. ...

    const char* file_path = argv[0];
    BegMillisec;
    Result result = RunFile(vm, file_path);
    EndMillisec;
    exitcode = (int) result;
  }

  // Cleanup the VM and exit.
  FreeVM(vm);
  return exitcode;
}