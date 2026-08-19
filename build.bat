:: Copyright (c) 2022-2026 Mohamed Abdifatah. All rights reserved.
:: Distributed Under The MIT License

@echo off
setlocal EnableExtensions EnableDelayedExpansion
pushd "%~dp0"

:: ============================================================================
:: PROJECT
:: ============================================================================
set "project_root=%~dp0"
set "NAME=saynaa"

:: ============================================================================
:: DEPENDENCIES
:: ============================================================================
set "pcre2_path=%project_root%deps\pcre2"
set "pcre2_inc=/I"%pcre2_path%\include""
set "pcre2_lib="%pcre2_path%\lib\pcre2-8-static.lib""

:: ============================================================================
:: ARGUMENTS
:: ============================================================================
set "enable_debug=true"

:PARSE_ARGS

if "%~1"=="" goto :CHECK_MSVC

if "%~1"=="-r" (
    set "enable_debug=false"
    shift
    goto :PARSE_ARGS
)

if "%~1"=="-c" (
    goto :CLEAN
)

echo Unknown option: %~1
echo.
echo Usage:
echo   build.bat       Debug build
echo   build.bat -r    Release build
echo   build.bat -c    Clean
goto :FAIL

:: ============================================================================
:: CHECK MSVC
:: ============================================================================
:CHECK_MSVC

if defined VCINSTALLDIR goto :START

echo Not running from an MSVC prompt.
echo Searching for Visual Studio...

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "!vswhere!" (
    set "vswhere=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
)

if not exist "!vswhere!" (
    echo Error: cannot find vswhere.exe
    goto :FAIL
)

set "install_path="

for /f "usebackq tokens=*" %%i in (`
    "!vswhere!" -latest ^
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
    -property installationPath
`) do (
    set "install_path=%%i"
)

if not defined install_path (
    echo Error: Visual Studio C/C++ tools were not found.
    goto :FAIL
)

set "vcvars=!install_path!\VC\Auxiliary\Build\vcvars64.bat"

if not exist "!vcvars!" (
    echo Error: cannot find:
    echo !vcvars!
    goto :FAIL
)

echo Initializing MSVC:
echo !vcvars!
call "!vcvars!"

if errorlevel 1 (
    echo Error: failed to initialize MSVC.
    goto :FAIL
)

:: ============================================================================
:: BUILD CONFIGURATION
:: ============================================================================
:START

set "target_dir=%project_root%obj"

set "add_defines=/D_CRT_SECURE_NO_WARNINGS /DPCRE2_STATIC"
set "add_cflags=/W3 /FS"

if "!enable_debug!"=="false" (

    echo.
    echo ========================================
    echo        Saynaa Release Build
    echo ========================================
    echo.

    set "cflags=/O2 /MD /DNDEBUG"

) else (

    echo.
    echo ========================================
    echo         Saynaa Debug Build
    echo ========================================
    echo.

    set "cflags=/MDd /Zi"
    set "add_defines=!add_defines! /DDEBUG"
)

:: ============================================================================
:: OPTIONALS
:: ============================================================================
if not exist "%project_root%src\optionals" (
    echo Optional sources disabled.
    set "add_defines=!add_defines! /DNO_OPTIONALS"
)

:: ============================================================================
:: PREPARE OBJECT LISTS
:: ============================================================================
set "core_objs="
set "cli_objs="
set "source_count=0"

if exist "%target_dir%" (
    echo Cleaning old object files...
    rmdir /S /Q "%target_dir%"
)

mkdir "%target_dir%"

:: ============================================================================
:: COMPILE ALL C SOURCES
:: ============================================================================
echo.
echo ========================================
echo          Compiling sources
echo ========================================
echo.

for /f "delims=" %%F in ('dir /b /s "%project_root%src\*.c" 2^>nul') do (

    set "src_file=%%F"

    :: Convert absolute path to project-relative path
    set "rel_path=!src_file:%project_root%=!"

    set "skip_file=false"

    :: ------------------------------------------------------------
    :: Skip optional sources when optionals directory doesn't exist
    :: ------------------------------------------------------------
    if not exist "%project_root%src\optionals" (

        echo !rel_path! | findstr /i /c:"src\optionals\" >nul

        if !errorlevel!==0 (
            set "skip_file=true"
        )
    )

    if "!skip_file!"=="false" (

        set /a source_count+=1

        :: --------------------------------------------------------
        :: Object path
        :: Example:
        ::
        :: src\runtime\saynaa_vm.c
        ::
        :: becomes:
        ::
        :: obj\src\runtime\saynaa_vm.obj
        :: --------------------------------------------------------
        set "obj_file=%target_dir%\!rel_path:.c=.obj!"

        for %%I in ("!obj_file!") do (

            if not exist "%%~dpI" (
                mkdir "%%~dpI"
            )
        )

        echo [CC] !rel_path!

        cl /nologo ^
            /c ^
            !add_defines! ^
            !pcre2_inc! ^
            !add_cflags! ^
            !cflags! ^
            /Fo"!obj_file!" ^
            "!src_file!"

        if errorlevel 1 (
            echo.
            echo ERROR: compilation failed:
            echo !src_file!
            goto :FAIL
        )

        :: --------------------------------------------------------
        :: Route CLI sources
        ::
        :: src\saynaa\*.c
        ::
        :: into executable objects.
        :: Everything else becomes runtime/library objects.
        :: --------------------------------------------------------
        echo !rel_path! | findstr /i /b /c:"src\saynaa\" >nul

        if !errorlevel!==0 (

            set "cli_objs=!cli_objs! "!obj_file!""

        ) else (

            set "core_objs=!core_objs! "!obj_file!""
        )
    )
)

:: ============================================================================
:: VERIFY SOURCES
:: ============================================================================
if "!source_count!"=="0" (
    echo.
    echo ERROR: No C source files were found in:
    echo %project_root%src
    goto :FAIL
)

if "!core_objs!"=="" (
    echo.
    echo ERROR: No runtime/core object files were generated.
    goto :FAIL
)

if "!cli_objs!"=="" (
    echo.
    echo ERROR: No CLI object files were generated.
    echo.
    echo Expected executable sources under:
    echo   src\saynaa\
    echo.
    goto :FAIL
)

:: ============================================================================
:: CREATE STATIC LIBRARY
:: ============================================================================
echo.
echo ========================================
echo       Creating libsaynaa.lib
echo ========================================
echo.

if not exist "%target_dir%\lib" (
    mkdir "%target_dir%\lib"
)

set "mylib=%target_dir%\lib\%NAME%.lib"

echo [LIB] %mylib%

lib /nologo ^
    /OUT:"!mylib!" ^
    !core_objs!

if errorlevel 1 (
    echo.
    echo ERROR: Failed to create:
    echo !mylib!
    goto :FAIL
)

:: ============================================================================
:: FINAL LINK
:: ============================================================================
echo.
echo ========================================
echo          Linking Saynaa
echo ========================================
echo.

echo [LINK] %NAME%.exe

:: IMPORTANT:
:: /Fe belongs to CL and MUST appear before /link.
:: Anything after /link is passed directly to LINK.EXE.

cl /nologo ^
    !add_defines! ^
    !cli_objs! ^
    "!mylib!" ^
    !pcre2_lib! ^
    /Fe"%project_root%%NAME%.exe" ^
    /link ^
    /MACHINE:X64

if errorlevel 1 (
    echo.
    echo ERROR: Linking failed.
    goto :FAIL
)

:: ============================================================================
:: SUCCESS
:: ============================================================================
echo.
echo ========================================
echo          BUILD SUCCESSFUL
echo ========================================
echo.
echo Executable:
echo   %project_root%%NAME%.exe
echo.
echo Library:
echo   !mylib!
echo.

goto :END

:: ============================================================================
:: CLEAN
:: ============================================================================
:CLEAN

echo Cleaning Saynaa build files...

if exist "%project_root%obj" (
    rmdir /S /Q "%project_root%obj"
)

if exist "%project_root%%NAME%.exe" (
    del /Q "%project_root%%NAME%.exe"
)

if exist "%project_root%%NAME%.pdb" (
    del /Q "%project_root%%NAME%.pdb"
)

if exist "%project_root%%NAME%.ilk" (
    del /Q "%project_root%%NAME%.ilk"
)

echo Clean complete.
goto :END

:: ============================================================================
:: FAILURE
:: ============================================================================
:FAIL

echo.
echo ========================================
echo             BUILD FAILED
echo ========================================
echo.

popd
endlocal
exit /b 1

:: ============================================================================
:: END
:: ============================================================================
:END

popd
endlocal
exit /b 0
