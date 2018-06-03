:: set path to yours
set PROJ=e:/work/nie3d/build/android

cd /d %PROJ%

@echo clean bin folder
rmdir /s /q  ..\..\bin\android\armeabi
rmdir /s /q  ..\..\bin\android\armeabi-v7a

cd ../../src/core/proj.android/jni
call :CleanBIN CORE
: CORE

cd ../../src/oglrenderer/proj.android/jni
call :CleanBIN OGLRENDERER
: OGLRENDERER

cd ../../src/engine/proj.android/jni
call :CleanBIN ENGINE
: ENGINE

cd ../../src/main/proj.android/jni
call :CleanBIN MAIN
: MAIN

goto END

: CleanBIN
rmdir /s /q ..\libs
rmdir /s /q  ..\libs
rmdir /s /q  ..\obj
rmdir /s /q  ..\obj
cd /d %PROJ%
goto :%1

:END
