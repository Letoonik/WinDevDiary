file(GLOB WDD_SOURCES *.cpp *.hpp)
set(JSON_FILES ../vendor/json/include/nlohmann/json.hpp)


add_executable(WinDevDiary ${WDD_SOURCES} ${JSON_FILES})

# target_link_libraries(WinDevDiary PRIVATE glfw ImGui)
target_include_directories(WinDevDiary PRIVATE ${CMAKE_SOURCE_DIR}/vendor/json/include)