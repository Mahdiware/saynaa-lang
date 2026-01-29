/*
 *  Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 *  Distributed Under The MIT License
 */
// !! THIS FILE IS GENERATED DO NOT EDIT !!

#include <saynaa.h>

typedef Configuration (*NewConfiguration_t)();
typedef VM* (*NewVM_t)(Configuration*);
typedef void (*FreeVM_t)(VM*);
typedef void (*SetUserData_t)(VM*, void*);
typedef void* (*GetUserData_t)(const VM*);
typedef void (*RegisterBuiltinFn_t)(VM*, const char*, nativeFn, int, const char*);
typedef void (*AddSearchPath_t)(VM*, const char*);
typedef void* (*Realloc_t)(VM*, void*, size_t);
typedef void (*releaseHandle_t)(VM*, Handle*);
typedef Handle* (*NewModule_t)(VM*, const char*);
typedef void (*registerModule_t)(VM*, Handle*);
typedef void (*ModuleAddFunction_t)(VM*, Handle*, const char*, nativeFn, int, const char*);
typedef Handle* (*NewClass_t)(VM*, const char*, Handle*, Handle*, NewInstanceFn, DeleteInstanceFn, const char*);
typedef void (*ClassAddMethod_t)(VM*, Handle*, const char*, nativeFn, int, const char*);
typedef Class* (*NewNativeClass_t)(VM*, const char*, NewInstanceFn, DeleteInstanceFn, const char*);
typedef void (*NativeClassAddMethod_t)(VM*, Class*, const char*, nativeFn, int, const char*);
typedef void (*ModuleAddSource_t)(VM*, Handle*, const char*);
typedef Result (*RunString_t)(VM*, const char*);
typedef Result (*RunFile_t)(VM*, const char*);
typedef double (*vm_time_t)(VM*);
typedef Result (*RunREPL_t)(VM*);
typedef void (*SetRuntimeError_t)(VM*, const char*);
typedef void* (*GetThis_t)(const VM*);
typedef int (*GetArgc_t)(const VM*);
typedef bool (*CheckArgcRange_t)(VM*, int, int, int);
typedef bool (*ValidateSlotBool_t)(VM*, int, bool*);
typedef bool (*ValidateSlotNumber_t)(VM*, int, double*);
typedef bool (*ValidateSlotInteger_t)(VM*, int, int32_t*);
typedef bool (*ValidateSlotString_t)(VM*, int, const char**, uint32_t*);
typedef bool (*ValidateSlotType_t)(VM*, int, VarType);
typedef bool (*ValidateSlotInstanceOf_t)(VM*, int, int);
typedef bool (*IsSlotInstanceOf_t)(VM*, int, int, bool*);
typedef void (*reserveSlots_t)(VM*, int);
typedef int (*GetSlotsCount_t)(VM*);
typedef VarType (*GetSlotType_t)(VM*, int);
typedef bool (*GetSlotBool_t)(VM*, int);
typedef double (*GetSlotNumber_t)(VM*, int);
typedef const char* (*GetSlotString_t)(VM*, int, uint32_t*);
typedef void* (*GetSlotPointer_t)(VM*, int, void*, Destructor);
typedef Handle* (*GetSlotHandle_t)(VM*, int);
typedef void* (*GetSlotNativeInstance_t)(VM*, int);
typedef void (*setSlotNull_t)(VM*, int);
typedef void (*setSlotBool_t)(VM*, int, bool);
typedef void (*setSlotNumber_t)(VM*, int, double);
typedef void (*setSlotString_t)(VM*, int, const char*);
typedef void (*setSlotPointer_t)(VM*, int, void*, Destructor);
typedef void (*setSlotClosure_t)(VM*, int, const char*, nativeFn, int, const char*);
typedef void (*setSlotStringLength_t)(VM*, int, const char*, uint32_t);
typedef void (*setSlotHandle_t)(VM*, int, Handle*);
typedef uint32_t (*GetSlotHash_t)(VM*, int);
typedef void (*PlaceThis_t)(VM*, int);
typedef void (*GetClass_t)(VM*, int, int);
typedef bool (*NewInstance_t)(VM*, int, int, int, int);
typedef void (*NewRange_t)(VM*, int, double, double);
typedef void (*NewList_t)(VM*, int);
typedef void (*NewMap_t)(VM*, int);
typedef void (*NewString_t)(VM*, int);
typedef void (*NewPointer_t)(VM*, int, void*, Destructor);
typedef void (*NewClosure_t)(VM*, int, const char*, nativeFn, int, const char*);
typedef bool (*ListInsert_t)(VM*, int, int32_t, int);
typedef bool (*ListPop_t)(VM*, int, int32_t, int);
typedef uint32_t (*ListLength_t)(VM*, int);
typedef bool (*CallFunction_t)(VM*, int, int, int, int);
typedef bool (*CallMethod_t)(VM*, int, const char*, int, int, int);
typedef bool (*GetAttribute_t)(VM*, int, const char*, int);
typedef bool (*setAttribute_t)(VM*, int, const char*, int);
typedef bool (*ImportModule_t)(VM*, const char*, int);

typedef struct {
  NewConfiguration_t NewConfiguration_ptr;
  NewVM_t NewVM_ptr;
  FreeVM_t FreeVM_ptr;
  SetUserData_t SetUserData_ptr;
  GetUserData_t GetUserData_ptr;
  RegisterBuiltinFn_t RegisterBuiltinFn_ptr;
  AddSearchPath_t AddSearchPath_ptr;
  Realloc_t Realloc_ptr;
  releaseHandle_t releaseHandle_ptr;
  NewModule_t NewModule_ptr;
  registerModule_t registerModule_ptr;
  ModuleAddFunction_t ModuleAddFunction_ptr;
  NewClass_t NewClass_ptr;
  ClassAddMethod_t ClassAddMethod_ptr;
  NewNativeClass_t NewNativeClass_ptr;
  NativeClassAddMethod_t NativeClassAddMethod_ptr;
  ModuleAddSource_t ModuleAddSource_ptr;
  RunString_t RunString_ptr;
  RunFile_t RunFile_ptr;
  vm_time_t vm_time_ptr;
  RunREPL_t RunREPL_ptr;
  SetRuntimeError_t SetRuntimeError_ptr;
  GetThis_t GetThis_ptr;
  GetArgc_t GetArgc_ptr;
  CheckArgcRange_t CheckArgcRange_ptr;
  ValidateSlotBool_t ValidateSlotBool_ptr;
  ValidateSlotNumber_t ValidateSlotNumber_ptr;
  ValidateSlotInteger_t ValidateSlotInteger_ptr;
  ValidateSlotString_t ValidateSlotString_ptr;
  ValidateSlotType_t ValidateSlotType_ptr;
  ValidateSlotInstanceOf_t ValidateSlotInstanceOf_ptr;
  IsSlotInstanceOf_t IsSlotInstanceOf_ptr;
  reserveSlots_t reserveSlots_ptr;
  GetSlotsCount_t GetSlotsCount_ptr;
  GetSlotType_t GetSlotType_ptr;
  GetSlotBool_t GetSlotBool_ptr;
  GetSlotNumber_t GetSlotNumber_ptr;
  GetSlotString_t GetSlotString_ptr;
  GetSlotPointer_t GetSlotPointer_ptr;
  GetSlotHandle_t GetSlotHandle_ptr;
  GetSlotNativeInstance_t GetSlotNativeInstance_ptr;
  setSlotNull_t setSlotNull_ptr;
  setSlotBool_t setSlotBool_ptr;
  setSlotNumber_t setSlotNumber_ptr;
  setSlotString_t setSlotString_ptr;
  setSlotPointer_t setSlotPointer_ptr;
  setSlotClosure_t setSlotClosure_ptr;
  setSlotStringLength_t setSlotStringLength_ptr;
  setSlotHandle_t setSlotHandle_ptr;
  GetSlotHash_t GetSlotHash_ptr;
  PlaceThis_t PlaceThis_ptr;
  GetClass_t GetClass_ptr;
  NewInstance_t NewInstance_ptr;
  NewRange_t NewRange_ptr;
  NewList_t NewList_ptr;
  NewMap_t NewMap_ptr;
  NewString_t NewString_ptr;
  NewPointer_t NewPointer_ptr;
  NewClosure_t NewClosure_ptr;
  ListInsert_t ListInsert_ptr;
  ListPop_t ListPop_ptr;
  ListLength_t ListLength_ptr;
  CallFunction_t CallFunction_ptr;
  CallMethod_t CallMethod_ptr;
  GetAttribute_t GetAttribute_ptr;
  setAttribute_t setAttribute_ptr;
  ImportModule_t ImportModule_ptr;
} NativeApi;

static NativeApi native_api;

EXPORT void InitApi(NativeApi* api) {
  native_api.NewConfiguration_ptr = api->NewConfiguration_ptr;
  native_api.NewVM_ptr = api->NewVM_ptr;
  native_api.FreeVM_ptr = api->FreeVM_ptr;
  native_api.SetUserData_ptr = api->SetUserData_ptr;
  native_api.GetUserData_ptr = api->GetUserData_ptr;
  native_api.RegisterBuiltinFn_ptr = api->RegisterBuiltinFn_ptr;
  native_api.AddSearchPath_ptr = api->AddSearchPath_ptr;
  native_api.Realloc_ptr = api->Realloc_ptr;
  native_api.releaseHandle_ptr = api->releaseHandle_ptr;
  native_api.NewModule_ptr = api->NewModule_ptr;
  native_api.registerModule_ptr = api->registerModule_ptr;
  native_api.ModuleAddFunction_ptr = api->ModuleAddFunction_ptr;
  native_api.NewClass_ptr = api->NewClass_ptr;
  native_api.ClassAddMethod_ptr = api->ClassAddMethod_ptr;
  native_api.NewNativeClass_ptr = api->NewNativeClass_ptr;
  native_api.NativeClassAddMethod_ptr = api->NativeClassAddMethod_ptr;
  native_api.ModuleAddSource_ptr = api->ModuleAddSource_ptr;
  native_api.RunString_ptr = api->RunString_ptr;
  native_api.RunFile_ptr = api->RunFile_ptr;
  native_api.vm_time_ptr = api->vm_time_ptr;
  native_api.RunREPL_ptr = api->RunREPL_ptr;
  native_api.SetRuntimeError_ptr = api->SetRuntimeError_ptr;
  native_api.GetThis_ptr = api->GetThis_ptr;
  native_api.GetArgc_ptr = api->GetArgc_ptr;
  native_api.CheckArgcRange_ptr = api->CheckArgcRange_ptr;
  native_api.ValidateSlotBool_ptr = api->ValidateSlotBool_ptr;
  native_api.ValidateSlotNumber_ptr = api->ValidateSlotNumber_ptr;
  native_api.ValidateSlotInteger_ptr = api->ValidateSlotInteger_ptr;
  native_api.ValidateSlotString_ptr = api->ValidateSlotString_ptr;
  native_api.ValidateSlotType_ptr = api->ValidateSlotType_ptr;
  native_api.ValidateSlotInstanceOf_ptr = api->ValidateSlotInstanceOf_ptr;
  native_api.IsSlotInstanceOf_ptr = api->IsSlotInstanceOf_ptr;
  native_api.reserveSlots_ptr = api->reserveSlots_ptr;
  native_api.GetSlotsCount_ptr = api->GetSlotsCount_ptr;
  native_api.GetSlotType_ptr = api->GetSlotType_ptr;
  native_api.GetSlotBool_ptr = api->GetSlotBool_ptr;
  native_api.GetSlotNumber_ptr = api->GetSlotNumber_ptr;
  native_api.GetSlotString_ptr = api->GetSlotString_ptr;
  native_api.GetSlotPointer_ptr = api->GetSlotPointer_ptr;
  native_api.GetSlotHandle_ptr = api->GetSlotHandle_ptr;
  native_api.GetSlotNativeInstance_ptr = api->GetSlotNativeInstance_ptr;
  native_api.setSlotNull_ptr = api->setSlotNull_ptr;
  native_api.setSlotBool_ptr = api->setSlotBool_ptr;
  native_api.setSlotNumber_ptr = api->setSlotNumber_ptr;
  native_api.setSlotString_ptr = api->setSlotString_ptr;
  native_api.setSlotPointer_ptr = api->setSlotPointer_ptr;
  native_api.setSlotClosure_ptr = api->setSlotClosure_ptr;
  native_api.setSlotStringLength_ptr = api->setSlotStringLength_ptr;
  native_api.setSlotHandle_ptr = api->setSlotHandle_ptr;
  native_api.GetSlotHash_ptr = api->GetSlotHash_ptr;
  native_api.PlaceThis_ptr = api->PlaceThis_ptr;
  native_api.GetClass_ptr = api->GetClass_ptr;
  native_api.NewInstance_ptr = api->NewInstance_ptr;
  native_api.NewRange_ptr = api->NewRange_ptr;
  native_api.NewList_ptr = api->NewList_ptr;
  native_api.NewMap_ptr = api->NewMap_ptr;
  native_api.NewString_ptr = api->NewString_ptr;
  native_api.NewPointer_ptr = api->NewPointer_ptr;
  native_api.NewClosure_ptr = api->NewClosure_ptr;
  native_api.ListInsert_ptr = api->ListInsert_ptr;
  native_api.ListPop_ptr = api->ListPop_ptr;
  native_api.ListLength_ptr = api->ListLength_ptr;
  native_api.CallFunction_ptr = api->CallFunction_ptr;
  native_api.CallMethod_ptr = api->CallMethod_ptr;
  native_api.GetAttribute_ptr = api->GetAttribute_ptr;
  native_api.setAttribute_ptr = api->setAttribute_ptr;
  native_api.ImportModule_ptr = api->ImportModule_ptr;
}

Configuration NewConfiguration() {
  return native_api.NewConfiguration_ptr();
}

VM* NewVM(Configuration* config) {
  return native_api.NewVM_ptr(config);
}

void FreeVM(VM* vm) {
  native_api.FreeVM_ptr(vm);
}

void SetUserData(VM* vm, void* user_data) {
  native_api.SetUserData_ptr(vm, user_data);
}

void* GetUserData(const VM* vm) {
  return native_api.GetUserData_ptr(vm);
}

void RegisterBuiltinFn(VM* vm, const char* name, nativeFn fn, int arity, const char* docstring) {
  native_api.RegisterBuiltinFn_ptr(vm, name, fn, arity, docstring);
}

void AddSearchPath(VM* vm, const char* path) {
  native_api.AddSearchPath_ptr(vm, path);
}

void* Realloc(VM* vm, void* ptr, size_t size) {
  return native_api.Realloc_ptr(vm, ptr, size);
}

void releaseHandle(VM* vm, Handle* handle) {
  native_api.releaseHandle_ptr(vm, handle);
}

Handle* NewModule(VM* vm, const char* name) {
  return native_api.NewModule_ptr(vm, name);
}

void registerModule(VM* vm, Handle* module) {
  native_api.registerModule_ptr(vm, module);
}

void ModuleAddFunction(VM* vm, Handle* module, const char* name, nativeFn fptr, int arity, const char* docstring) {
  native_api.ModuleAddFunction_ptr(vm, module, name, fptr, arity, docstring);
}

Handle* NewClass(VM* vm, const char* name, Handle* base_class, Handle* module, NewInstanceFn new_fn, DeleteInstanceFn delete_fn, const char* docstring) {
  return native_api.NewClass_ptr(vm, name, base_class, module, new_fn, delete_fn, docstring);
}

void ClassAddMethod(VM* vm, Handle* cls, const char* name, nativeFn fptr, int arity, const char* docstring) {
  native_api.ClassAddMethod_ptr(vm, cls, name, fptr, arity, docstring);
}

Class* NewNativeClass(VM* vm, const char* name, NewInstanceFn new_fn, DeleteInstanceFn delete_fn, const char* docstring) {
  return native_api.NewNativeClass_ptr(vm, name, new_fn, delete_fn, docstring);
}

void NativeClassAddMethod(VM* vm, Class* cls, const char* name, nativeFn fptr, int arity, const char* docstring) {
  native_api.NativeClassAddMethod_ptr(vm, cls, name, fptr, arity, docstring);
}

void ModuleAddSource(VM* vm, Handle* module, const char* source) {
  native_api.ModuleAddSource_ptr(vm, module, source);
}

Result RunString(VM* vm, const char* source) {
  return native_api.RunString_ptr(vm, source);
}

Result RunFile(VM* vm, const char* path) {
  return native_api.RunFile_ptr(vm, path);
}

double vm_time(VM* vm) {
  return native_api.vm_time_ptr(vm);
}

Result RunREPL(VM* vm) {
  return native_api.RunREPL_ptr(vm);
}

void SetRuntimeError(VM* vm, const char* message) {
  native_api.SetRuntimeError_ptr(vm, message);
}

void* GetThis(const VM* vm) {
  return native_api.GetThis_ptr(vm);
}

int GetArgc(const VM* vm) {
  return native_api.GetArgc_ptr(vm);
}

bool CheckArgcRange(VM* vm, int argc, int min, int max) {
  return native_api.CheckArgcRange_ptr(vm, argc, min, max);
}

bool ValidateSlotBool(VM* vm, int slot, bool* value) {
  return native_api.ValidateSlotBool_ptr(vm, slot, value);
}

bool ValidateSlotNumber(VM* vm, int slot, double* value) {
  return native_api.ValidateSlotNumber_ptr(vm, slot, value);
}

bool ValidateSlotInteger(VM* vm, int slot, int32_t* value) {
  return native_api.ValidateSlotInteger_ptr(vm, slot, value);
}

bool ValidateSlotString(VM* vm, int slot, const char** value, uint32_t* length) {
  return native_api.ValidateSlotString_ptr(vm, slot, value, length);
}

bool ValidateSlotType(VM* vm, int slot, VarType type) {
  return native_api.ValidateSlotType_ptr(vm, slot, type);
}

bool ValidateSlotInstanceOf(VM* vm, int slot, int cls) {
  return native_api.ValidateSlotInstanceOf_ptr(vm, slot, cls);
}

bool IsSlotInstanceOf(VM* vm, int inst, int cls, bool* val) {
  return native_api.IsSlotInstanceOf_ptr(vm, inst, cls, val);
}

void reserveSlots(VM* vm, int count) {
  native_api.reserveSlots_ptr(vm, count);
}

int GetSlotsCount(VM* vm) {
  return native_api.GetSlotsCount_ptr(vm);
}

VarType GetSlotType(VM* vm, int index) {
  return native_api.GetSlotType_ptr(vm, index);
}

bool GetSlotBool(VM* vm, int index) {
  return native_api.GetSlotBool_ptr(vm, index);
}

double GetSlotNumber(VM* vm, int index) {
  return native_api.GetSlotNumber_ptr(vm, index);
}

const char* GetSlotString(VM* vm, int index, uint32_t* length) {
  return native_api.GetSlotString_ptr(vm, index, length);
}

void* GetSlotPointer(VM* vm, int index, void* native_ptr, Destructor destructor) {
  return native_api.GetSlotPointer_ptr(vm, index, native_ptr, destructor);
}

Handle* GetSlotHandle(VM* vm, int index) {
  return native_api.GetSlotHandle_ptr(vm, index);
}

void* GetSlotNativeInstance(VM* vm, int index) {
  return native_api.GetSlotNativeInstance_ptr(vm, index);
}

void setSlotNull(VM* vm, int index) {
  native_api.setSlotNull_ptr(vm, index);
}

void setSlotBool(VM* vm, int index, bool value) {
  native_api.setSlotBool_ptr(vm, index, value);
}

void setSlotNumber(VM* vm, int index, double value) {
  native_api.setSlotNumber_ptr(vm, index, value);
}

void setSlotString(VM* vm, int index, const char* value) {
  native_api.setSlotString_ptr(vm, index, value);
}

void setSlotPointer(VM* vm, int index, void* native_ptr, Destructor destructor) {
  native_api.setSlotPointer_ptr(vm, index, native_ptr, destructor);
}

void setSlotClosure(VM* vm, int index, const char* name, nativeFn fptr, int arity, const char* docstring) {
  native_api.setSlotClosure_ptr(vm, index, name, fptr, arity, docstring);
}

void setSlotStringLength(VM* vm, int index, const char* value, uint32_t length) {
  native_api.setSlotStringLength_ptr(vm, index, value, length);
}

void setSlotHandle(VM* vm, int index, Handle* handle) {
  native_api.setSlotHandle_ptr(vm, index, handle);
}

uint32_t GetSlotHash(VM* vm, int index) {
  return native_api.GetSlotHash_ptr(vm, index);
}

void PlaceThis(VM* vm, int index) {
  native_api.PlaceThis_ptr(vm, index);
}

void GetClass(VM* vm, int instance, int index) {
  native_api.GetClass_ptr(vm, instance, index);
}

bool NewInstance(VM* vm, int cls, int index, int argc, int argv) {
  return native_api.NewInstance_ptr(vm, cls, index, argc, argv);
}

void NewRange(VM* vm, int index, double first, double last) {
  native_api.NewRange_ptr(vm, index, first, last);
}

void NewList(VM* vm, int index) {
  native_api.NewList_ptr(vm, index);
}

void NewMap(VM* vm, int index) {
  native_api.NewMap_ptr(vm, index);
}

void NewString(VM* vm, int index) {
  native_api.NewString_ptr(vm, index);
}

void NewPointer(VM* vm, int index, void* native_ptr, Destructor destructor) {
  native_api.NewPointer_ptr(vm, index, native_ptr, destructor);
}

void NewClosure(VM* vm, int index, const char* name, nativeFn fptr, int arity, const char* docstring) {
  native_api.NewClosure_ptr(vm, index, name, fptr, arity, docstring);
}

bool ListInsert(VM* vm, int list, int32_t index, int value) {
  return native_api.ListInsert_ptr(vm, list, index, value);
}

bool ListPop(VM* vm, int list, int32_t index, int popped) {
  return native_api.ListPop_ptr(vm, list, index, popped);
}

uint32_t ListLength(VM* vm, int list) {
  return native_api.ListLength_ptr(vm, list);
}

bool CallFunction(VM* vm, int fn, int argc, int argv, int ret) {
  return native_api.CallFunction_ptr(vm, fn, argc, argv, ret);
}

bool CallMethod(VM* vm, int instance, const char* method, int argc, int argv, int ret) {
  return native_api.CallMethod_ptr(vm, instance, method, argc, argv, ret);
}

bool GetAttribute(VM* vm, int instance, const char* name, int index) {
  return native_api.GetAttribute_ptr(vm, instance, name, index);
}

bool setAttribute(VM* vm, int instance, const char* name, int value) {
  return native_api.setAttribute_ptr(vm, instance, name, value);
}

bool ImportModule(VM* vm, const char* path, int index) {
  return native_api.ImportModule_ptr(vm, path, index);
}

