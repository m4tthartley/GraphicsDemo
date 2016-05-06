
@echo off
IF NOT EXIST ..\build mkdir ..\build
pushd w:\GraphicsDemo\build

REM set includes=-I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include"
set libraries="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64\d3dx11.lib" "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64\d3dx10.lib"

:: cl -Zi -Od -MTd -nologo ../code/graphics_win32.cc -FeGraphicsDemo.exe %includes% -link -subsystem:windows %libraries% user32.lib w:/lib/vulkan/vulkan-1.lib d3d11.lib OpenGL32.lib gdi32.lib glu32.lib

:: cl -Zi -Od -MTd -nologo ../code/graphics_win32.cc -FeGraphicsDemoDirectx.exe -DRENDER_DIRECTX %includes% -link -subsystem:windows %libraries% user32.lib d3d11.lib
cl -Zi -Od -MTd -nologo ../code/graphics_win32.cc -FeGraphicsDemoOpenGL.exe -DRENDER_OPENGL -link -subsystem:windows user32.lib OpenGL32.lib gdi32.lib glu32.lib
:: use /P to output preprocessor file

popd