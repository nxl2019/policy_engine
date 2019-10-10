include_directories(./include)
aux_source_directory(./src SRCS)

add_library(policy_engine SHARED ${SRCS})
target_link_libraries(policy_engine pthread)
target_link_libraries(policy_engine jsoncpp_lib)
target_link_libraries(policy_engine policymgr)
