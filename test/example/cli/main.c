#include "cli/saynaa.h"
#include "runtime/saynaa_vm.h"
#include "shared/saynaa_value.h"


int main(int argc, const char** argv) {
  VM* vm = NewVM(NULL);
  Handle* handle = NewModule(vm, "test");
  Module* module = (Module*) AS_OBJ(handle->value);

  Result result = RunFileWithModule(vm, module, "test1.sbc");
  Result result2 = RunFileWithModule(vm, module, "test2.sbc");

  FreeVM(vm);
  return 0;
}