@echo off

set tmpdir=%cd%
if not exist %~dp0bin mkdir %~dp0bin
cd %~dp0bin

cl ^
  /Fe:pc.exe ^
  ^
  /O2 /Oi ^
  /EHsc /Gd /GL /Gy ^
  /DNDEBUG /D_CONSOLE /D_UNICODE /DUNICODE ^
  /permissive- /Zc:inline ^
  /MD ^
  /FC /nologo /utf-8 ^
  /sdl /W4 /WX ^
  ^
  ..\src\*.c ^
  ^
  /link ^
  user32.lib ^
  gdi32.lib ^
  dwmapi.lib ^
  ws2_32.lib ^
  dxgi.lib

del *.obj

cd %tmpdir%
