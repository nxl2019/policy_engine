include_directories(../pe/include)
include_directories(../policymgr/include)

add_executable(performance_test main.cpp)
target_link_libraries(performance_test policy_engine)
target_link_libraries(performance_test benchmark)
target_link_libraries(performance_test pthread)