# Creating Modules

In Saynaa, you can extend the language by creating modules. Modules can be created in two ways:
1.  **Native Modules**: Written in Saynaa itself (`.sa` files).
2.  **Internal Modules**: Written in C and compiled into the Saynaa VM interactively.

## Native Modules

Native modules are simply Saynaa source files that export functions and variables. You can import them using the `import` statement.

### Single File Module

Create a file named `math_utils.sa`:

```ruby
# math_utils.sa

function square(n)
  return n * n
end

PI = 3.14159
```

You can use it in another script:

```ruby
import math_utils

print(math_utils.square(5)) # Output: 25
print(math_utils.pi)        # Output: 3.14159
```

### Package Module (Directory)

You can also create a module as a directory. The directory must contain an `_init.sa` file, which is executed when the module is imported.

Directory structure:
```
my_pkg/
  _init.sa
  helper.sa
```

In `my_pkg/helper.sa`:
```ruby
function greet(name)
  return "Hello, " + name
end
```

In `my_pkg/_init.sa`:
```ruby
import my_pkg.helper

function welcome()
   print(helper.greet("User"))
end
```

Usage:
```ruby
import my_pkg

my_pkg.welcome()
```

## Internal Modules (C API)

For performance-critical code or system-level access, you can write modules in C.

### 1. Define the Function

Use the `saynaa_function` macro to define a function exposed to the VM. The function receives a `VM*` pointer. Arguments are on the stack starting at index 1.

```c
#include "saynaa_optionals.h"

// Signature: MyModule.add(a: Number, b: Number) -> Number
// Docstring: Adds two numbers.
saynaa_function(_myModuleAdd, "MyModule.add(a:Number, b:Number) -> Number", "Adds two numbers.") {
  double a, b;
  
  // Validate and retrieve arguments from stack slots 1 and 2
  if (!ValidateSlotNumber(vm, 1, &a)) return;
  if (!ValidateSlotNumber(vm, 2, &b)) return;

  // Set the return value in slot 0
  RET(VAR_NUM(a + b));
}
```

### 2. Register the Module

You need a registration function to create the module and bind functions to it.

```c
void registerModuleMyModule(VM* vm) {
  // Create a new module named "MyModule"
  Handle* mod = NewModule(vm, "MyModule");

  // Register functions
  // REGISTER_FN(handle, name, function_pointer, argument_count)
  REGISTER_FN(mod, "add", _myModuleAdd, 2);

  // Set global constants in the module
  moduleSetGlobal(vm, AS_MODULE(mod->value), "VERSION", 7, VAR_OBJ(newString(vm, "1.0.0")));

  // Finalize registration
  registerModule(vm, mod);
  releaseHandle(vm, mod);
}
```

### 3. Integrate with VM

You must call your registration function during VM initialization (usually in `src/optionals/saynaa_optionals.c` or wherever `registerLibs` is called).

```c
// In saynaa_optionals.c
void registerLibs(VM* vm) {
  // ... existing modules ...
  registerModuleMyModule(vm);
}
```

### Useful Macros & Helper Functions

*   **`RET(value)`**: Returns a value from the function.
*   **`ValidateSlotNumber(vm, slot, &out)`**: Checks if slot is a number and gets it.
*   **`ValidateSlotString(vm, slot, &out, &len)`**: Checks/gets a string.
*   **`setSlotString(vm, slot, "str")`**: Sets a string to a slot.
*   **`VAR_NUM(double)`, `VAR_BOOL(bool)`, `VAR_NULL`**: Value constructors.
