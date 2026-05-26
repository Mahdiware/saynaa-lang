#include "cli/saynaa.h"
#include "runtime/saynaa_vm.h"
#include "shared/saynaa_value.h"

int main(int argc, const char** argv) {
  VM* vm = NewVM(NULL);
  Handle* handle = NewModule(vm, "test");
  Module* module = (Module*) AS_OBJ(handle->value);

  // compile test1.sa to test1.sbc and test2.sa to test2.sbc before running this test.

  // saynaa -b test1.sa
  // saynaa - b test2.sa
  Result result = RunFileWithModule(vm, module, "test1.sbc");
  Result result2 = RunFileWithModule(vm, module, "test2.sbc");

  releaseHandle(vm, handle);
  FreeVM(vm);
  return 0;
}