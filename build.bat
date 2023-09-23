:: Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
:: Distributed Under The MIT License

@echo off
Pushd %cd%
cd %~dp0

:: Root directory of the project
set project_root=%~dp0

:: ----------------------------------------------------------------------------
:: PARSE COMMAND LINE ARGUMENTS
:: ----------------------------------------------------------------------------

set enable_debug=true
set use_shared_lib=false

goto :PARSE_ARGS

:SHIFT_ARG_2
shift
:SHIFT_ARG_1
shift

:PARSE_ARGS
if (%1)==(-h) goto :PRINT_USAGE
if (%1)==(-c) goto :CLEAN
if (%1)==(-r) set enable_debug=false && goto :SHIFT_ARG_1
if (%1)==(-s) set use_shared_lib=true && goto :SHIFT_ARG_1
if (%1)==() goto :CHECK_MSVC

echo Invalid argument "%1"

:PRINT_USAGE
echo Usage: call build.bat [options ...]
echo options:
echo   -h  display this message
echo   -r  Compile the release version of saynaa (default = debug)
echo   -s  Link saynaa as a shared library (default = static link).
echo   -c  Clean all compiled/generated intermediate binaries.
goto :END

:: ----------------------------------------------------------------------------
:: INITIALIZE MSVC ENVIRONMENT
:: ----------------------------------------------------------------------------
:CHECK_MSVC

if not defined INCLUDE goto :MSVC_INIT
goto :START

:MSVC_INIT
echo Not running on an MSVC prompt, searching for one...

:: Find vswhere
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set VSWHERE_PATH="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
) else (
    if exist "%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" (
        set VSWHERE_PATH="%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
    ) else (
        echo Can't find vswhere.exe
        goto :NO_VS_PROMPT
    )
)

:: Get the VC installation path
%VSWHERE_PATH% -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -latest -property installationPath > _path_temp.txt
set /p VSWHERE_PATH= < _path_temp.txt
del _path_temp.txt
if not exist "%VSWHERE_PATH%" (
    echo Error: can't find Visual Studio installation directory
    goto :NO_VS_PROMPT
)

echo Found at - %VSWHERE_PATH%

:: Initialize VC for X86_64
call "%VSWHERE_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 goto :NO_VS_PROMPT
echo Initialized MSVC x86_64
goto :START

:NO_VS_PROMPT
echo You must open a "Visual Studio .NET Command Prompt" to run this script
goto :END

:: ----------------------------------------------------------------------------
:: START
:: ----------------------------------------------------------------------------
:START

set target_dir=
set additional_cflags=-W3 -GR /FS -EHsc
set additional_linkflags=/SUBSYSTEM:CONSOLE
set additional_defines=/D_CRT_SECURE_NO_WARNINGS

:: Relative root directory from a single intermediate directory.

if "%enable_debug%"=="false" (
    set cflags=%cflags% -O2 -MD /DNDEBUG
    set target_dir=%project_root%obj\
) else (
    set cflags=%cflags% -MDd -ZI
    set additional_defines=%additional_defines% /DDEBUG
    set target_dir=%project_root%obj\
)

if "%use_shared_lib%"=="true" (
    set additional_defines=%additional_defines% /D_DLL_ /D_COMPILE_
)

:: Make intermediate folders.
if not exist %target_dir% mkdir %target_dir%
if not exist %target_dir%lib\ mkdir %target_dir%lib\
if not exist %target_dir%saynaa mkdir %target_dir%saynaa\
if not exist %target_dir%cli\ mkdir %target_dir%cli\

:: ----------------------------------------------------------------------------
:: COMPILE
:: ----------------------------------------------------------------------------
:COMPILE

cd %target_dir%saynaa

cl /nologo /c %additional_defines% %additional_cflags% %project_root%src\compiler\*.c %project_root%src\optionals\*.c %project_root%src\runtime\*.c %project_root%src\shared\*.c %project_root%src\utils\*.c
if errorlevel 1 goto :FAIL

:: If compiling a shared lib, jump past the lib/cli binaries.
if "%use_shared_lib%"=="true" (
  set mylib=%target_dir%bin\saynaa.lib
) else (
  set mylib=%target_dir%lib\saynaa.lib
)

:: If compiling a shared lib, jump past the lib/cli binaries.
if "%use_shared_lib%"=="true" goto :SHARED
lib /nologo %additional_linkflags% /OUT:%mylib% *.obj
goto :SRC_END

:SHARED
link /nologo /dll /out:%target_dir%bin\saynaa.dll /implib:%mylib% *.obj

:SRC_END
if errorlevel 1 goto :FAIL

cd %target_dir%cli

cl /nologo /c %additional_defines% %additional_cflags% %project_root%src\cli\*.c
if errorlevel 1 goto :FAIL

cd %project_root%

:: Compile the cli executable.
cl /nologo %additional_defines% %target_dir%cli\*.obj %mylib% /Fe%project_root%saynaa.exe
if errorlevel 1 goto :FAIL

:: Navigate to the root directory.
cd ..\

goto :SUCCESS

:CLEAN

if exist "%project_root%obj" rmdir /S /Q "%project_root%obj"
if exist "%project_root%obj" rmdir /S /Q "%project_root%obj"

echo.
echo Files were cleaned.
goto :END

:: ----------------------------------------------------------------------------
:: END
:: ----------------------------------------------------------------------------

:SUCCESS
echo.
echo Compilation Success
goto :END

:FAIL
popd
endlocal
echo Build failed. See the error messages.
exit /b 1

:END
popd
endlocal
goto :eof