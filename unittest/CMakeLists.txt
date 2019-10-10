include_directories(../third/microtest)
include_directories(../pe/include)

add_executable(unit_test main.cpp)
target_link_libraries(unit_test policy_engine)