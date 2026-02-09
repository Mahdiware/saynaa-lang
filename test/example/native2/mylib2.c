#include "nativeapi.h"

EXPORT void _greet(VM* vm) {
  const char* name;
  if (!ValidateSlotString(vm, 1, &name, NULL))
    return;
  printf("Hello from native2, %s!\n", name);
}

Handle* mylib2;

// Entry point
EXPORT Handle* ExportModule(VM* vm) {
  // Create module
  mylib2 = NewModule(vm, "mylib2");

  // Add functions
  ModuleAddFunction(vm, mylib2, "greet", _greet, 1, "greets the user");

  return mylib2;
}
