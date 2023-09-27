/*
 *  Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
 *  Distributed Under The MIT License
 */

#ifndef __SAYNAA_NATIVE__
#define __SAYNAA_NATIVE__

#include "../cli/saynaa.h"

// !! THIS FILE IS GENERATED DO NOT EDIT !!

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
typedef void (*ModuleAddSource_t)(VM*, Handle*, const char*);
typedef Result (*RunString_t)(VM*, const char*);
typedef Result (*RunFile_t)(VM*, const char*);
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
typedef Handle* (*GetSlotHandle_t)(VM*, int);
typedef void* (*GetSlotNativeInstance_t)(VM*, int);
typedef void (*setSlotNull_t)(VM*, int);
typedef void (*setSlotBool_t)(VM*, int, bool);
typedef void (*setSlotNumber_t)(VM*, int, double);
typedef void (*setSlotString_t)(VM*, int, const char*);
typedef void (*setSlotStringLength_t)(VM*, int, const char*, uint32_t);
typedef void (*setSlotHandle_t)(VM*, int, Handle*);
typedef uint32_t (*GetSlotHash_t)(VM*, int);
typedef void (*PlaceThis_t)(VM*, int);
typedef void (*GetClass_t)(VM*, int, int);
typedef bool (*NewInstance_t)(VM*, int, int, int, int);
typedef void (*NewRange_t)(VM*, int, double, double);
typedef void (*NewList_t)(VM*, int);
typedef void (*NewMap_t)(VM*, int);
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
  ModuleAddSource_t ModuleAddSource_ptr;
  RunString_t RunString_ptr;
  RunFile_t RunFile_ptr;
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
  GetSlotHandle_t GetSlotHandle_ptr;
  GetSlotNativeInstance_t GetSlotNativeInstance_ptr;
  setSlotNull_t setSlotNull_ptr;
  setSlotBool_t setSlotBool_ptr;
  setSlotNumber_t setSlotNumber_ptr;
  setSlotString_t setSlotString_ptr;
  setSlotStringLength_t setSlotStringLength_ptr;
  setSlotHandle_t setSlotHandle_ptr;
  GetSlotHash_t GetSlotHash_ptr;
  PlaceThis_t PlaceThis_ptr;
  GetClass_t GetClass_ptr;
  NewInstance_t NewInstance_ptr;
  NewRange_t NewRange_ptr;
  NewList_t NewList_ptr;
  NewMap_t NewMap_ptr;
  ListInsert_t ListInsert_ptr;
  ListPop_t ListPop_ptr;
  ListLength_t ListLength_ptr;
  CallFunction_t CallFunction_ptr;
  CallMethod_t CallMethod_ptr;
  GetAttribute_t GetAttribute_ptr;
  setAttribute_t setAttribute_ptr;
  ImportModule_t ImportModule_ptr;
} NativeApi;

#define API_INIT_FN_NAME "InitApi"
#define EXPORT_FN_NAME "ExportModule"

#define CLEANUP_FN_NAME "CleanupModule"

typedef void (*InitApiFn)(NativeApi*);
typedef Handle* (*ExportModuleFn)(VM*);

#ifdef DL_IMPLEMENT

NativeApi MakeNativeAPI() {

  NativeApi api;

  api.NewConfiguration_ptr = NewConfiguration;
  api.NewVM_ptr = NewVM;
  api.FreeVM_ptr = FreeVM;
  api.SetUserData_ptr = SetUserData;
  api.GetUserData_ptr = GetUserData;
  api.RegisterBuiltinFn_ptr = RegisterBuiltinFn;
  api.AddSearchPath_ptr = AddSearchPath;
  api.Realloc_ptr = Realloc;
  api.releaseHandle_ptr = releaseHandle;
  api.NewModule_ptr = NewModule;
  api.registerModule_ptr = registerModule;
  api.ModuleAddFunction_ptr = ModuleAddFunction;
  api.NewClass_ptr = NewClass;
  api.ClassAddMethod_ptr = ClassAddMethod;
  api.ModuleAddSource_ptr = ModuleAddSource;
  api.RunString_ptr = RunString;
  api.RunFile_ptr = RunFile;
  api.RunREPL_ptr = RunREPL;
  api.SetRuntimeError_ptr = SetRuntimeError;
  api.GetThis_ptr = GetThis;
  api.GetArgc_ptr = GetArgc;
  api.CheckArgcRange_ptr = CheckArgcRange;
  api.ValidateSlotBool_ptr = ValidateSlotBool;
  api.ValidateSlotNumber_ptr = ValidateSlotNumber;
  api.ValidateSlotInteger_ptr = ValidateSlotInteger;
  api.ValidateSlotString_ptr = ValidateSlotString;
  api.ValidateSlotType_ptr = ValidateSlotType;
  api.ValidateSlotInstanceOf_ptr = ValidateSlotInstanceOf;
  api.IsSlotInstanceOf_ptr = IsSlotInstanceOf;
  api.reserveSlots_ptr = reserveSlots;
  api.GetSlotsCount_ptr = GetSlotsCount;
  api.GetSlotType_ptr = GetSlotType;
  api.GetSlotBool_ptr = GetSlotBool;
  api.GetSlotNumber_ptr = GetSlotNumber;
  api.GetSlotString_ptr = GetSlotString;
  api.GetSlotHandle_ptr = GetSlotHandle;
  api.GetSlotNativeInstance_ptr = GetSlotNativeInstance;
  api.setSlotNull_ptr = setSlotNull;
  api.setSlotBool_ptr = setSlotBool;
  api.setSlotNumber_ptr = setSlotNumber;
  api.setSlotString_ptr = setSlotString;
  api.setSlotStringLength_ptr = setSlotStringLength;
  api.setSlotHandle_ptr = setSlotHandle;
  api.GetSlotHash_ptr = GetSlotHash;
  api.PlaceThis_ptr = PlaceThis;
  api.GetClass_ptr = GetClass;
  api.NewInstance_ptr = NewInstance;
  api.NewRange_ptr = NewRange;
  api.NewList_ptr = NewList;
  api.NewMap_ptr = NewMap;
  api.ListInsert_ptr = ListInsert;
  api.ListPop_ptr = ListPop;
  api.ListLength_ptr = ListLength;
  api.CallFunction_ptr = CallFunction;
  api.CallMethod_ptr = CallMethod;
  api.GetAttribute_ptr = GetAttribute;
  api.setAttribute_ptr = setAttribute;
  api.ImportModule_ptr = ImportModule;

  return api;
}

#endif // DL_IMPLEMENT
#endif // __SAYNAA_NATIVE__