#include "cli/saynaa.h"
#include "runtime/saynaa_vm.h"
#include "shared/saynaa_value.h"

Result _runFile(VM* vm, Module* module, const char* path) {
  initializeModule(vm, module, true);

  bool local_is_bytecode = false;
  Result load_status = RESULT_SUCCESS;

  LoadScriptResult load_result = LoadScript(vm, path);

  compile(vm, module, load_result.content, NULL);

  module->initialized = true;
  Fiber* fiber = newFiber(vm, module->body);
  vmPushTempRef(vm, &fiber->_super); // fiber.
  vmPrepareFiber(vm, fiber, 0, NULL);
  vmPopTempRef(vm); // fiber.

  return vmRunFiber(vm, fiber);
}

int main(int argc, const char** argv) {
  VM* vm = NewVM(NULL);
  Handle* handle = NewModule(vm, "test");
  Module* module = (Module*) AS_OBJ(handle->value);

  Result result = RunFileWithModule(vm, module, "test1.sa");
  Result result2 = RunFileWithModule(vm, module, "test2.sa");

  FreeVM(vm);
  return 0;
}