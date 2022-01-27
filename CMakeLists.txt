cmake_minimum_required(VERSION 3.20)
project(BlitLib)
set(CMAKE_WIN32_EXECUTABLE TRUE)
set(CMAKE_CXX_STANDARD 20)

add_executable(BlitLib src/main.cpp src/core/types.hpp src/platform/Window.cpp src/platform/Window.h src/platform/Win32Framework.h src/platform/OSContext.cpp src/platform/OSContext.h src/platform/Console.cpp src/platform/Console.h src/platform/Framebuffer.cpp src/platform/Framebuffer.h src/core/Buffer2D.cpp src/core/Buffer2D.h src/graphics/Draw.h src/graphics/Color.cpp src/graphics/Color.h src/graphics/Image.cpp src/graphics/Image.h src/graphics/Draw.cpp src/graphics/Sprite.cpp src/graphics/Sprite.h src/core/Slice.cpp src/core/Slice.h src/core/Vec2.cpp src/core/Vec2.h src/graphics/BitmapFile.cpp src/graphics/BitmapFile.h)
find_package(fmt)
target_link_libraries(BlitLib PRIVATE fmt::fmt-header-only)

target_include_directories(BlitLib PUBLIC include)