//
// Created by jeff on 2019/10/14.
//

#ifndef POLICY_ENGINE_C_API_TEST_H
#define POLICY_ENGINE_C_API_TEST_H
#include "microtest.h"
#include "policy_engine.h"
#include <list>


#include <thread>
//prefiltertestcase4
TEST(api_test_case_1) {
    std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    std::string tag = "prefiltertestcase4";
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;
    PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                                       3600);//1 hous

    PolicyEngineStringList sublist = NULL;
    PolicyEngineStringList actlist = NULL;
    PolicyEngineStringList resourcelist = NULL;
    PolicyEngineStringList host = NULL;
    PolicyEngineStringList app = NULL;
    ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);

    //traversal display
    printf("subject ref:  ");
    for (PolicyEngineStringList it = sublist; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char * sub_v = nullptr;
        ret = policy_engine_string_list_current(it, &sub_v);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        printf("%s, ",sub_v);
    }
    printf("\naction ref:  ");
    for (PolicyEngineStringList it = actlist; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char * act = nullptr;
        ret = policy_engine_string_list_current(it, &act);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        printf("%s, ",act);
    }
    printf("\nresource ref:  ");
    for (PolicyEngineStringList it = resourcelist; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char * act = nullptr;
        ret = policy_engine_string_list_current(it, &act);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        printf("%s, ",act);
    }
    printf("\nhost ref:  ");
    for (PolicyEngineStringList it = host; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char * act = nullptr;
        ret = policy_engine_string_list_current(it, &act);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        printf("%s, ",act);
    }
    printf("\napp ref:  ");
    for (PolicyEngineStringList it = app; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char * act = nullptr;
        ret = policy_engine_string_list_current(it, &act);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        printf("%s, ",act);
    }
    printf("\n");

    policy_engine_destroy_string_list(sublist);
    policy_engine_destroy_string_list(actlist);
    policy_engine_destroy_string_list(resourcelist);
    policy_engine_destroy_string_list(host);
    policy_engine_destroy_string_list(app);

    policy_engine_module_exit();

}



#endif //POLICY_ENGINE_C_API_TEST_H
