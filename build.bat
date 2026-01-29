:: Copyright (c) 2022-2023 Mohamed Abdifatah. All rights reserved.
:: Distributed Under The MIT License

@echo off
setlocal
Pushd %cd%
cd %~dp0

:: Root directory of the project
set project_root=%~dp0

:: ----------------------------------------------------------------------------
:: DEPENDENCIES
:: ----------------------------------------------------------------------------
set pcre2_path=%project_root%deps\pcre2
set pcre2_inc=/I"%pcre2_path%\include"
set pcre2_lib="%pcre2_path%\lib\pcre2-8-static.lib"

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

:: Find vswhere - Fixed quoting for paths with spaces
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set "VSWHERE_PATH=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
) else (
    echo Can't find vswhere.exe
    goto :NO_VS_PROMPT
)

:: Get the VC installation path - Use "usebackq" to handle spaces in VSWHERE_PATH
for /f "usebackq tokens=*" %%i in (`^"%VSWHERE_PATH%^" -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -latest -property installationPath`) do (
    set "VS_INSTALL_PATH=%%i"
)

if not exist "%VS_INSTALL_PATH%" (
    echo Error: can't find Visual Studio installation directory
    goto :NO_VS_PROMPT
)

echo Found at - %VS_INSTALL_PATH%

:: Initialize VC for X86_64 - Corrected path concatenation
call "%VS_INSTALL_PATH%\VC\Auxiliary\Build\vcvars64.bat"
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

set "target_dir=%project_root%obj\"
set additional_cflags=-W3 -GR /FS -EHsc
set additional_linkflags=/SUBSYSTEM:CONSOLE
set additional_defines=/D_CRT_SECURE_NO_WARNINGS /DPCRE2_STATIC

if "%enable_debug%"=="false" (
    set additional_cflags=%additional_cflags% -O2 -MD /DNDEBUG
) else (
    set additional_cflags=%additional_cflags% -MDd -ZI
    set additional_defines=%additional_defines% /DDEBUG
)

if "%use_shared_lib%"=="true" (
    set additional_defines=%additional_defines% /D_DLL_ /D_COMPILE_
)

:: Make intermediate folders.
if not exist "%target_dir%" mkdir "%target_dir%"
if not exist "%target_dir%lib\" mkdir "%target_dir%lib\"
if not exist "%target_dir%saynaa\" mkdir "%target_dir%saynaa\"
if not exist "%target_dir%cli\" mkdir "%target_dir%cli\"

:: ----------------------------------------------------------------------------
:: COMPILE CORE & OPTIONALS
:: ----------------------------------------------------------------------------
:COMPILE

cd "%target_dir%saynaa"

cl /nologo /c %additional_defines% %pcre2_inc% %additional_cflags% ^
    "%project_root%src\compiler\*.c" ^
    "%project_root%src\optionals\*.c" ^
    "%project_root%src\runtime\*.c" ^
    "%project_root%src\shared\*.c" ^
    "%project_root%src\utils\*.c"

if errorlevel 1 goto :FAIL

if "%use_shared_lib%"=="true" (
  set "mylib=%target_dir%bin\saynaa.lib"
) else (
  set "mylib=%target_dir%lib\saynaa.lib"
)

if "%use_shared_lib%"=="true" goto :SHARED

lib /nologo %additional_linkflags% /OUT:"%mylib%" *.obj
goto :SRC_END

:SHARED
if not exist "%target_dir%bin\" mkdir "%target_dir%bin\"
link /nologo /dll /out:"%target_dir%bin\saynaa.dll" /implib:"%mylib%" *.obj %pcre2_lib%

:SRC_END
if errorlevel 1 goto :FAIL

:: ----------------------------------------------------------------------------
:: COMPILE CLI
:: ----------------------------------------------------------------------------
cd "%target_dir%cli"

cl /nologo /c %additional_defines% %pcre2_inc% %additional_cflags% "%project_root%src\cli\*.c"
if errorlevel 1 goto :FAIL

cd "%project_root%"

:: Final Link
cl /nologo %additional_defines% "%target_dir%cli\*.obj" "%mylib%" %pcre2_lib% /Fe"%project_root%saynaa.exe"
if errorlevel 1 goto :FAIL

goto :SUCCESS

:CLEAN
if exist "%project_root%obj" rmdir /S /Q "%project_root%obj"
if exist "%project_root%saynaa.exe" del "%project_root%saynaa.exe"
echo.
echo Files were cleaned.
goto :END

:SUCCESS
echo.
echo Compilation Success: saynaa.exe is ready.
goto :END

:FAIL
popd
endlocal
echo Build failed.
exit /b 1

:END
popd
endlocal
goto :eof
