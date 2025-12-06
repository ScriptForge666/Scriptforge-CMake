# 创建测试可执行文件
add_executable(ScriptforgeTest 
    main.cpp
)

# 链接库 - 现在会自动包含头文件路径
target_link_libraries(ScriptforgeTest Scriptforge)

# 只需要包含测试文件自己的头文件目录
target_include_directories(ScriptforgeTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 如果使用 Google Test
find_package(GTest QUIET)
if(GTest_FOUND)
    target_link_libraries(ScriptforgeTest GTest::gtest GTest::gtest_main)
    enable_testing()
    add_test(NAME ScriptforgeTests COMMAND ScriptforgeTest)
endif()
