

#include "nativeapi.h"

Handle* variable2;
Handle* mylib;

#define ARG(n) (vm->fiber->ret[n])

// Evaluates to the current function's argument count.
#define ARGC ((int) (vm->fiber->sp - vm->fiber->ret) - 1)

// Set return value for the current native function and return.
#define RET(value) \
  do { \
    *(vm->fiber->ret) = value; \
    return; \
  } while (false)

// --- Variable Class ---

typedef struct {
  int value;
} Variable;

EXPORT void* _newVariable(VM* vm) {
  Variable* var = (Variable*) Realloc(vm, NULL, sizeof(Variable));
  var->value = 0;
  return var;
}

EXPORT void _deleteVariable(VM* vm, void* ptr) {
  Realloc(vm, ptr, 0);
}

EXPORT void _initVariable(VM* vm) {
  double val;
  if (!ValidateSlotNumber(vm, 1, &val))
    return;

  Variable* thiz = (Variable*) GetThis(vm);
  thiz->value = (int) val;
}

EXPORT void _Setter(VM* vm) {
  const char* name = GetSlotString(vm, 1, NULL);
  Variable* thiz = (Variable*) GetThis(vm);
  if (strcmp("value", name) == 0) {
    double value;
    if (!ValidateSlotNumber(vm, 2, &value))
      return;
    thiz->value = (int) value;
    return;
  }
}

EXPORT void _Getter(VM* vm) {
  const char* name = GetSlotString(vm, 1, NULL);
  Variable* thiz = (Variable*) GetThis(vm);
  if (strcmp("value", name) == 0) {
    setSlotNumber(vm, 0, (double) thiz->value);
    return;
  }
}

// --- Vector Class ---

typedef struct {
  double x, y;
} Vector;

EXPORT void* _newVector(VM* vm) {
  Vector* vec = (Vector*) Realloc(vm, NULL, sizeof(Vector));
  vec->x = 0;
  vec->y = 0;
  return vec;
}

EXPORT void _deleteVector(VM* vm, void* ptr) {
  Realloc(vm, ptr, 0);
}

EXPORT void _initVector(VM* vm) {
  double x, y;
  if (!ValidateSlotNumber(vm, 1, &x))
    return;
  if (!ValidateSlotNumber(vm, 2, &y))
    return;

  Vector* self = (Vector*) GetThis(vm);
  self->x = x;
  self->y = y;
}

EXPORT void _vecAdd(VM* vm) {
  Vector* self = (Vector*) GetThis(vm);

  // We expect the argument to be another Vector instance.
  // In a production environment, you should check the type match using handles.
  Vector* other = (Vector*) GetSlotNativeInstance(vm, 1);
  if (other == NULL)
    return; // Error should have been set by GetSlotNativeInstance if invalid.

  // Perform vector addition in place
  self->x += other->x;
  self->y += other->y;

  // Return self (for chaining)
  // We need to set return value (slot 0) to 'this'.
  // 'this' is available in the fiber, or we can use PlaceThis(vm, 0).
  PlaceThis(vm, 0);
}

EXPORT void _vecToString(VM* vm) {
  Vector* self = (Vector*) GetThis(vm);
  char buffer[128];
  snprintf(buffer, sizeof(buffer), "Vector(%.2f, %.2f)", self->x, self->y);
  setSlotString(vm, 0, buffer);
}

// --- Module Export ---

EXPORT Handle* ExportModule(VM* vm) {
  mylib = NewModule(vm, "mylib");

  // Register Variable
  Handle* variable = NewClass(vm, "Value", NULL, mylib, _newVariable,
                              _deleteVariable, "new variable");
  ClassAddMethod(vm, variable, "_init", _initVariable, 1, "");
  ClassAddMethod(vm, variable, "_getter", _Getter, 1, "");
  ClassAddMethod(vm, variable, "_setter", _Setter, 2, "");
  releaseHandle(vm, variable);

  // Register Vector
  Handle* vector = NewClass(vm, "Vector", NULL, mylib, _newVector,
                            _deleteVector, "2D Vector class");
  ClassAddMethod(vm, vector, "_init", _initVector, 2, "init(x, y)");
  ClassAddMethod(vm, vector, "add", _vecAdd, 1, "add(otherVector)");
  ClassAddMethod(vm, vector, "toString", _vecToString, 0, "toString()");
  releaseHandle(vm, vector);

  return mylib;
}
