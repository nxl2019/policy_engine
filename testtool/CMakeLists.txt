include_directories(../third/microtest)
include_directories(../pe/include)
include_directories(../third/jsoncpp)
include_directories(../tool)

add_executable(test_tool main.cpp task.cpp)
target_link_libraries(test_tool policy_engine)