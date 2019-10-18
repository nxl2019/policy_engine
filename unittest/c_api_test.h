//
// Created by jeff on 2019/10/14.
//

#ifndef POLICY_ENGINE_C_API_TEST_H
#define POLICY_ENGINE_C_API_TEST_H
#include "microtest.h"
#include "Policy.h"
#include "policy_expression.h"
#include <list>


#include <thread>

TEST(policy_engine_module_init_case_1) {
    std::string cchost = "https://cc80-console.qapf1.qalab01.nextlabs.com"; // ERROR PARAMS
    std::string tag = "pe_test";
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;
    {
        PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                                           10);
        ASSERT_TRUE(ret == POLICY_ENGINE_CCCONNECT_ERROR);
    }
}

TEST(policy_engine_module_init_case_2) {

    std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    std::string tag = "pe_test";

    //std::string cchost = "https://cc85-console.qapf1.qalab01.nextlabs.com" ;
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;
    //std::string tag = "spe" ;
    {
        PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), NULL,
                                                           10);
        ASSERT_TRUE(ret == POLICY_ENGINE_FAIL);
    }
    PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                                       10);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                    10);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ret = policy_engine_module_init(NULL, ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                    10);
    ASSERT_TRUE(ret == POLICY_ENGINE_FAIL);
}


TEST(policy_engine_analyze_case_1) {

    {
        PolicyEngineStringList sublist = NULL;
        PolicyEngineStringList actlist = NULL;
        PolicyEngineStringList resourcelist = NULL;
        PolicyEngineStringList host = NULL;
        PolicyEngineStringList app = NULL;
        PolicyEngineReturn ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);

        const char * sub_v;
        ret = policy_engine_string_list_current(sublist, &sub_v);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);

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
        policy_engine_destroy_string_list(resourcelist);
        policy_engine_destroy_string_list(host);
        policy_engine_destroy_string_list(actlist);
        policy_engine_destroy_string_list(app);

    }

}
TEST(policy_engine_analyze_case_2) {
    {
        PolicyEngineStringList sublist = NULL;
        PolicyEngineReturn ret = policy_engine_analyze(&sublist, NULL, NULL, NULL, NULL);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        ret = policy_engine_destroy_string_list(sublist);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    }
}
TEST(policy_engine_analyze_case_3) {
    {
        PolicyEngineStringList actlist = NULL;
        PolicyEngineReturn ret = policy_engine_analyze(NULL, &actlist, NULL, NULL, NULL);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
        ret = policy_engine_destroy_string_list(actlist);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    }
}
TEST(policy_engine_analyze_case_4) {
    {
        PolicyEngineReturn ret = policy_engine_analyze(NULL, NULL, NULL, NULL, NULL);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    }
}

TEST(policy_engine_match_case_1) {

    PolicyEngineHandle pdictionary = NULL;
    PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_SUBJECT, &pdictionary);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ret = policy_engine_insert_into_dictionary(pdictionary, "username", "xxx");
    ret = policy_engine_insert_into_dictionary(pdictionary, "level", "4");
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    POLICY_ENGINE_MATCH_RESULT result;
    ret =  policy_engine_match( pdictionary, "SELECT", NULL,NULL, NULL , &result);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ASSERT_TRUE(result == PE_NO_MATCHED);

    ret = policy_engine_destroy_dictionary(pdictionary);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
}
TEST(policy_engine_match_case_2) {

    PolicyEngineHandle pdictionary = NULL;
    PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_SUBJECT, &pdictionary);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ret = policy_engine_insert_into_dictionary(pdictionary, "username", "xxx");
    ret = policy_engine_insert_into_dictionary(pdictionary, "level", "4");
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    POLICY_ENGINE_MATCH_RESULT result;
    ret =  policy_engine_match( pdictionary, NULL, NULL,NULL, NULL , &result);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    ASSERT_TRUE(result == PE_NEED_MORE_WORK);

    ret = policy_engine_destroy_dictionary(pdictionary);
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
}


TEST(policy_engine_module_exit_case_1) {
    PolicyEngineReturn ret = policy_engine_module_exit();
    ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


#endif //POLICY_ENGINE_C_API_TEST_H
