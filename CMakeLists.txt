cmake_minimum_required(VERSION 3.20)
project(WinDevDiary)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(VS_STARTUP_PROJECT WinDevDiary)
set(IMGUI_DIR vendor/ImGui)

file(GLOB IMGUI_SOURCES
	${IMGUI_DIR}/*.cpp
	${IMGUI_DIR}/*.h
	${IMGUI_DIR}/misc/cpp/imgui_stdlib.h
	${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.h
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.h
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.cpp)

add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMGUI_DIR}/misc/cpp vendor/glfw/include)

add_subdirectory(vendor/glfw)
add_subdirectory(src)
target_link_libraries(WinDevDiary PUBLIC ImGui opengl32 ${CMAKE_CURRENT_BINARY_DIR}/Debug/ImGui.lib ${CMAKE_CURRENT_BINARY_DIR}/vendor/glfw/src/Debug/glfw3.lib)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/Debug ${CMAKE_CURRENT_BINARY_DIR}/vendor/glfw/src/Debug)
link_libraries(WinDevDiary ImGui)
