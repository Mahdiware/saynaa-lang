:: Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
:: Distributed Under The MIT License

@echo off
setlocal enabledelayedexpansion
pushd %~dp0

:: Root directory of the project
set "project_root=%~dp0"

:: ----------------------------------------------------------------------------
:: DEPENDENCIES
:: ----------------------------------------------------------------------------
set "pcre2_path=%project_root%deps\pcre2"
set "pcre2_inc=/I"%pcre2_path%\include""
set "pcre2_lib="%pcre2_path%\lib\pcre2-8-static.lib""

:: ----------------------------------------------------------------------------
:: PARSE COMMAND LINE ARGUMENTS
:: ----------------------------------------------------------------------------
set "enable_debug=true"

:PARSE_ARGS
if "%~1"=="" goto :CHECK_MSVC
if "%~1"=="-r" (set "enable_debug=false" & shift & goto :PARSE_ARGS)
if "%~1"=="-c" goto :CLEAN

:: ----------------------------------------------------------------------------
:: INITIALIZE MSVC ENVIRONMENT
:: ----------------------------------------------------------------------------
:CHECK_MSVC
if defined INCLUDE goto :START

echo Not running on an MSVC prompt, searching for one...

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "!vswhere!" set "vswhere=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "!vswhere!" (
    echo Error: can't find vswhere.exe
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"!vswhere!" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "install_path=%%i"
)

set "vcvars=!install_path!\VC\Auxiliary\Build\vcvars64.bat"
if not exist "!vcvars!" (
    echo Error: can't find vcvars64.bat
    exit /b 1
)

call "!vcvars!"
if errorlevel 1 exit /b 1

:: ----------------------------------------------------------------------------
:: START BUILD
:: ----------------------------------------------------------------------------
:START
set "target_dir=%project_root%obj\"
set "add_defines=/D_CRT_SECURE_NO_WARNINGS /DPCRE2_STATIC"
set "add_cflags=-W3 -GR /FS -EHsc"

if "!enable_debug!"=="false" (
    set "cflags=-O2 -MD /DNDEBUG"
) else (
    set "cflags=-MDd -ZI"
    set "add_defines=!add_defines! /DDEBUG"
)

:: Create directories
if not exist "!target_dir!saynaa\" mkdir "!target_dir!saynaa\"
if not exist "!target_dir!cli\" mkdir "!target_dir!cli\"
if not exist "!target_dir!lib\" mkdir "!target_dir!lib\"

:: 1. Compile Core
cd /d "!target_dir!saynaa"
cl /nologo /c !add_defines! !pcre2_inc! !add_cflags! !cflags! ^
    "!project_root!src\compiler\*.c" ^
    "!project_root!src\optionals\*.c" ^
    "!project_root!src\optionals\json\*.c" ^
    "!project_root!src\optionals\dirent\*.c" ^
    "!project_root!src\optionals\path\*.c" ^
    "!project_root!src\optionals\term\*.c" ^
    "!project_root!src\runtime\*.c" ^
    "!project_root!src\shared\*.c" ^
    "!project_root!src\utils\*.c"
if errorlevel 1 goto :FAIL

:: 2. Create Library
set "mylib=!target_dir!lib\saynaa.lib"
lib /nologo /OUT:"!mylib!" *.obj
if errorlevel 1 goto :FAIL

:: 3. Compile CLI
cd /d "!target_dir!cli"
cl /nologo /c !add_defines! !pcre2_inc! !add_cflags! !cflags! "!project_root!src\cli\*.c"
if errorlevel 1 goto :FAIL

:: 4. Final Link
cd /d "!project_root!"
cl /nologo !add_defines! "!target_dir!cli\*.obj" "!mylib!" !pcre2_lib! /Fe"saynaa.exe"
if errorlevel 1 goto :FAIL

echo Build Successful: saynaa.exe created.
goto :END

:CLEAN
if exist "obj" rmdir /S /Q "obj"
if exist "saynaa.exe" del "saynaa.exe"
goto :END

:FAIL
echo Build failed.
exit /b 1

:END
popd
endlocal
