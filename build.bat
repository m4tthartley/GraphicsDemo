
@echo off
mkdir build
pushd build

set includes=-I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include"
set libraries=-LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64"
cl -Zi -Od -MTd ../graphics_win32.cc -FeGraphicsDemo.exe %includes% -link -subsystem:windows %libraries% user32.lib w:/lib/vulkan/vulkan-1.lib d3d11.lib d3dx11.lib d3dx10.lib

popd