
@echo off
mkdir build
pushd build

REM set includes=-I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include"
set libraries="C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64\d3dx11.lib" "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64\d3dx10.lib"
cl -Zi -Od -MTd ../graphics_win32.cc -FeGraphicsDemo.exe %includes% -link -subsystem:windows %libraries% user32.lib w:/lib/vulkan/vulkan-1.lib d3d11.lib

popd