add_library(Scriptforge STATIC 
    Scriptforge.hpp 
    ScriptforgeErr.cpp ScriptforgeErr.hpp 
    ScriptforgeLog.cpp ScriptforgeLog.hpp 
    ScriptforgeTime.cpp ScriptforgeTime.hpp 
    ScriptforgeTree.cpp ScriptforgeTree.hpp
)

# 导出头文件目录（这样其他目标可以自动包含）
target_include_directories(Scriptforge PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置库的版本和属性
set_target_properties(Scriptforge PROPERTIES
    VERSION 0.0.1
    SOVERSION 1
)
