/*
 * Copyright (c) 2022-2026 Mohamed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 */

// !! THIS FILE IS GENERATED DO NOT EDIT !!

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT
#define IMPORT
#endif

typedef struct VM VM;
typedef struct Handle Handle;
typedef struct Class Class;
typedef struct Configuration Configuration;

typedef void (*nativeFn)(VM* vm);
typedef void* (*ReallocFn)(void* memory, size_t new_size, void* user_data);
typedef void (*WriteFn)(VM* vm, const char* text);
typedef char* (*ReadFn)(VM* vm);
typedef void (*SignalFn)(void*);
typedef char* (*LoadScriptFn)(VM* vm, const char* path);
typedef void* (*LoadDL)(VM* vm, const char* path);
typedef Handle* (*ImportDL)(VM* vm, void* handle);
typedef void (*UnloadDL)(VM* vm, void* handle);
typedef char* (*ResolvePathFn)(VM* vm, const char* from, const char* path);
typedef void* (*NewInstanceFn)(VM* vm);
typedef void (*DeleteInstanceFn)(VM* vm, void*);
typedef void (*Destructor)(void*);

typedef enum VarType {
  vOBJECT = 0,

  vNULL,
  vBOOL,
  vNUMBER,
  vSTRING,
  vLIST,
  vMAP,
  vRANGE,
  vMODULE,
  vCLOSURE,
  vMETHOD_BIND,
  vFIBER,
  vCLASS,
  vPOINTER,
  vINSTANCE,
} VarType;

typedef enum Result {
  RESULT_SUCCESS = 0, // Successfully finished the execution.

  // Note that this result is internal and will not be returned to the host
  // anymore.
  //
  // Unexpected EOF while compiling the source. This is another compile time
  // error that will ONLY be returned if we're compiling in REPL mode. We need
  // this specific error to indicate the host application to add another line
  // to the last input. If REPL is not enabled this will be compile error.
  RESULT_UNEXPECTED_EOF,

  RESULT_COMPILE_ERROR, // Compilation failed.
  RESULT_RUNTIME_ERROR, // An error occurred at runtime.
} Result;

typedef struct Argument {
  // argument count: is number of value inside argv
  int argc;

  // argument value that passed file: main.c in function: int main(argc, argv)
  const char** argv;
} Argument;

typedef struct Configuration {
  // The callback used to allocate, reallocate, and free. If the function
  // pointer is NULL it defaults to the VM's realloc(), free() wrappers.
  ReallocFn realloc_fn;

  // I/O callbacks.
  WriteFn stderr_write;
  WriteFn stdout_write;
  ReadFn stdin_read;

  // Import system callbacks.
  ResolvePathFn resolve_path_fn;
  LoadScriptFn load_script_fn;

#ifndef NO_DL
  LoadDL load_dl_fn;
  ImportDL import_dl_fn;
  UnloadDL unload_dl_fn;
#endif

  // If true stderr calls will use ansi color codes.
  bool use_ansi_escape;

  // User defined data associated with VM.
  void* user_data;

  Argument argument;
} Configuration;

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

Configuration NewConfiguration();
VM* NewVM(Configuration* config);
void FreeVM(VM* vm);
void SetUserData(VM* vm, void* user_data);
void* GetUserData(const VM* vm);
void RegisterBuiltinFn(VM* vm, const char* name, nativeFn fn, int arity, const char* docstring);
void AddSearchPath(VM* vm, const char* path);
void* Realloc(VM* vm, void* ptr, size_t size);
void releaseHandle(VM* vm, Handle* handle);
Handle* NewModule(VM* vm, const char* name);
void registerModule(VM* vm, Handle* module);
void ModuleAddFunction(VM* vm, Handle* module, const char* name, nativeFn fptr, int arity, const char* docstring);
Handle* NewClass(VM* vm, const char* name, Handle* base_class, Handle* module, NewInstanceFn new_fn, DeleteInstanceFn delete_fn, const char* docstring);
void ClassAddMethod(VM* vm, Handle* cls, const char* name, nativeFn fptr, int arity, const char* docstring);
Class* NewNativeClass(VM* vm, const char* name, NewInstanceFn new_fn, DeleteInstanceFn delete_fn, const char* docstring);
void NativeClassAddMethod(VM* vm, Class* cls, const char* name, nativeFn fptr, int arity, const char* docstring);
void ModuleAddSource(VM* vm, Handle* module, const char* source);
Result RunString(VM* vm, const char* source);
Result RunFile(VM* vm, const char* path);
double vm_time(VM* vm);
Result RunREPL(VM* vm);
void SetRuntimeError(VM* vm, const char* message);
void* GetThis(const VM* vm);
int GetArgc(const VM* vm);
bool CheckArgcRange(VM* vm, int argc, int min, int max);
bool ValidateSlotBool(VM* vm, int slot, bool* value);
bool ValidateSlotNumber(VM* vm, int slot, double* value);
bool ValidateSlotInteger(VM* vm, int slot, int32_t* value);
bool ValidateSlotString(VM* vm, int slot, const char** value, uint32_t* length);
bool ValidateSlotType(VM* vm, int slot, VarType type);
bool ValidateSlotInstanceOf(VM* vm, int slot, int cls);
bool IsSlotInstanceOf(VM* vm, int inst, int cls, bool* val);
void reserveSlots(VM* vm, int count);
int GetSlotsCount(VM* vm);
VarType GetSlotType(VM* vm, int index);
bool GetSlotBool(VM* vm, int index);
double GetSlotNumber(VM* vm, int index);
const char* GetSlotString(VM* vm, int index, uint32_t* length);
void* GetSlotPointer(VM* vm, int index, void* native_ptr, Destructor destructor);
Handle* GetSlotHandle(VM* vm, int index);
void* GetSlotNativeInstance(VM* vm, int index);
void setSlotNull(VM* vm, int index);
void setSlotBool(VM* vm, int index, bool value);
void setSlotNumber(VM* vm, int index, double value);
void setSlotString(VM* vm, int index, const char* value);
void setSlotPointer(VM* vm, int index, void* native_ptr, Destructor destructor);
void setSlotClosure(VM* vm, int index, const char* name, nativeFn fptr, int arity, const char* docstring);
void setSlotStringLength(VM* vm, int index, const char* value, uint32_t length);
void setSlotHandle(VM* vm, int index, Handle* handle);
uint32_t GetSlotHash(VM* vm, int index);
void PlaceThis(VM* vm, int index);
void GetClass(VM* vm, int instance, int index);
bool NewInstance(VM* vm, int cls, int index, int argc, int argv);
void NewRange(VM* vm, int index, double first, double last);
void NewList(VM* vm, int index);
void NewMap(VM* vm, int index);
void NewString(VM* vm, int index);
void NewPointer(VM* vm, int index, void* native_ptr, Destructor destructor);
void NewClosure(VM* vm, int index, const char* name, nativeFn fptr, int arity, const char* docstring);
bool ListInsert(VM* vm, int list, int32_t index, int value);
bool ListPop(VM* vm, int list, int32_t index, int popped);
uint32_t ListLength(VM* vm, int list);
bool CallFunction(VM* vm, int fn, int argc, int argv, int ret);
bool CallMethod(VM* vm, int instance, const char* method, int argc, int argv, int ret);
bool GetAttribute(VM* vm, int instance, const char* name, int index);
bool setAttribute(VM* vm, int instance, const char* name, int value);
bool ImportModule(VM* vm, const char* path, int index);