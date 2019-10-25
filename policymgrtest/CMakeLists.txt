include_directories(../third/microtest)
include_directories(../policymgr/include)
include_directories(../tool)

add_executable(policymgr_test main.cpp)
target_link_libraries(policymgr_test policy_engine)
target_link_libraries(policymgr_test tool)