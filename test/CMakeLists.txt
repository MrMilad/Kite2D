# external lib directories
link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(Test main.cpp)

# add Kite2D lib
if (WINDOWS)
    TARGET_LINK_LIBRARIES(Test "Kite2Dd.lib")
endif()

