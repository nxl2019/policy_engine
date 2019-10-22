include_directories(../third/microtest)
include_directories(../pe/include)
include_directories(../policymgr/include)
include_directories(../tool)

add_executable(unit_test main.cpp)
target_link_libraries(unit_test policy_engine)
target_link_libraries(unit_test tool)