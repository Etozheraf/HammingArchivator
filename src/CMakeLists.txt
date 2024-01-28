add_executable(HamArc main.cpp)

target_link_libraries(HamArc Operations)

target_include_directories(HamArc PUBLIC ${PROJECT_SOURCE_DIR})