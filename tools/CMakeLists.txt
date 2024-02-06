project(tools)
add_executable(tool main.cpp)
target_link_libraries(tool maat::maat)