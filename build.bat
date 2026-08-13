:: Copyright (c) 2022-2026 Mohamed Abdifatah. All rights reserved.
:: Distributed Under The MIT License

@echo off
setlocal enabledelayedexpansion
pushd %~dp0

:: Root directory of the project
set "project_root=%~dp0"
set "NAME=saynaa"

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
set "add_cflags=/W3 /GR /FS /EHsc"

if "!enable_debug!"=="false" (
    set "cflags=/O2 /MD /DNDEBUG"
) else (
    set "cflags=/MDd /Zi"
    set "add_defines=!add_defines! /DDEBUG"
)

:: Check if optionals directory exists
if not exist "%project_root%src\optionals" (
    set "add_defines=!add_defines! /DNO_OPTIONALS"
)

set "core_objs="
set "cli_objs="

:: Dynamically process all .c files in src/
for /f "delims=" %%F in ('dir /b /s "%project_root%src\*.c" 2^>nul') do (
    set "src_file=%%F"
    set "rel_path=!src_file:%project_root%=!"
    set "skip_file=false"
    
    if not exist "%project_root%src\optionals" (
        echo !rel_path! | findstr /i /c:"src\optionals\" >nul && set "skip_file=true"
    )
    
    if "!skip_file!"=="false" (
        set "obj_file=%target_dir%!rel_path:.c=.obj!"
        
        for %%I in ("!obj_file!") do (
            if not exist "%%~dpI" mkdir "%%~dpI"
        )
        
        cl /nologo /c !add_defines! !pcre2_inc! !add_cflags! !cflags! /Fo"!obj_file!" "!src_file!"
        if errorlevel 1 goto :FAIL
        
        :: Route object files based on directory
        echo !rel_path! | findstr /i /c:"src\saynaa\" >nul
        if !errorlevel!==0 (
            set "cli_objs=!cli_objs! "!obj_file!""
        ) else (
            set "core_objs=!core_objs! "!obj_file!""
        )
    )
)

if "!core_objs!"=="" (
    echo Error: No core source files found in src.
    goto :FAIL
)

:: 2. Create Library (libsaynaa.lib)
if not exist "%target_dir%lib\" mkdir "%target_dir%lib\"
set "mylib=%target_dir%lib\%NAME%.lib"

lib /nologo /OUT:"!mylib!" !core_objs!
if errorlevel 1 goto :FAIL

:: 3. Final Link
cd /d "%project_root%"
cl /nologo !add_defines! !cli_objs! "!mylib!" !pcre2_lib! /link /MACHINE:X64 /Fe"%NAME%.exe"
if errorlevel 1 goto :FAIL

echo Build Successful: %NAME%.exe created.
goto :END

:CLEAN
if exist "obj" rmdir /S /Q "obj"
if exist "%NAME%.exe" del "%NAME%.exe"
if exist "*.pdb" del "*.pdb"
goto :END

:FAIL
echo Build failed.
exit /b 1

:END
popd
endlocal