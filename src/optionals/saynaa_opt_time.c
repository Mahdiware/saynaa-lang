/*
 * Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

#include <time.h>
#include "saynaa_optionals.h"

#ifdef _WIN32
  #include <windows.h>
#endif

#if !defined(_MSC_VER) && !(defined(_WIN32) && defined(__TINYC__))
  #include <unistd.h> // usleep
#endif

function(_timeEpoch,
  "time() -> Number",
  "Returns the number of seconds since the Epoch, 1970-01-01 "
  "00:00:00 +0000 (UTC).") {
  setSlotNumber(vm, 0, (double) time(NULL));
}

function(_nanoSecond,
  "nano() -> Number",
  "Returns the number of nano seconds.") {
  setSlotNumber(vm, 0, (double) nanotime());
}

function(_timeClock,
  "clock() -> Number",
  "Returns the number of clocks passed divied by CLOCKS_PER_SEC.") {
  setSlotNumber(vm, 0, (double) clock() / CLOCKS_PER_SEC);
}

function(_timeSleep,
    "sleep(t:num) -> Number",
    "Sleep for [t] milliseconds.") {

  double t;
  ValidateSlotNumber(vm, 1, &t);

#if defined(_MSC_VER) || (defined(_WIN32) && defined(__TINYC__))
  // Sleep(milli seconds)
  Sleep((DWORD) t);

#else // usleep(micro seconds)
  usleep(((useconds_t) (t)) * 1000);
#endif
}

/*****************************************************************************/
/* MODULE REGISTER                                                           */
/*****************************************************************************/

void registerModuleTime(VM* vm) {
  Handle* time = NewModule(vm, "time");

  REGISTER_FN(time, "epoch", _timeEpoch, 0);
  REGISTER_FN(time, "sleep", _timeSleep, 1);
  REGISTER_FN(time, "clock", _timeClock, 0);
  REGISTER_FN(time, "nano", _nanoSecond, 0);

  registerModule(vm, time);
  releaseHandle(vm, time);
}