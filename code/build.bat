@echo off

REM THE SYNTAX FOR DISABLING A WARNING IS -wd<error-number> EX: -wd4201
REM -O2 for release builds

Set CommonCompilerFlags=-MD -GR- -EHa- -Oi -Zi -WX -W4 -wd4189 -wd4200 -wd4996
Set CommonLinkerFlags="..\data\deps\Raylib\lib\raylib.lib" opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib
Set CommonIncludePaths="..\data\deps\Raylib\include"

REM cl %CommonCompilerFlags% \Shift2\code\*.cpp /I%CommonIncludePaths% /MD /link %CommonLinkerFlags% 

REM 64-bit building

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %CommonCompilerFlags% \Clone\code\*.cpp /I%CommonIncludePaths% /link /MACHINE:X64 /OUT:"Clone.exe" %CommonLinkerFlags%
popd

@echo ====================
@echo Compilation Finished
@echo ====================
