@ECHO OFF

REM Assumes that you've already ran the vsvars32.bat
REM needed to set up the VC++ build environment...

MKDIR build
PUSHD build

cl.exe /Zi /EHs /I ..\include ^
       ..\tests\main.cpp ^
       /link /out:test.exe
POPD
