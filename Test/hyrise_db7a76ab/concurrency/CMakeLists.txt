include_directories(${PROJECT_SOURCE_DIR}/Src)

# 将测试代码添加到TEST_SOURCES
file(GLOB_RECURSE TEST_SOURCES *.cpp)

FOREACH (F ${TEST_SOURCES})
    get_filename_component(prjName ${F} NAME_WE) # 去除路径和拓展名 abc/test.cpp -> test, 这个就是最终可执行文件的名字
    MESSAGE("build ${prjName} according to ${F}")
    add_executable(${prjName} ${F})
    target_link_libraries(${prjName} lib_static pthread gtest gtest_main tbb)
    # add_test(NAME ${prjName} COMMAND ${prjName}) # 添加测试用例
ENDFOREACH (F)