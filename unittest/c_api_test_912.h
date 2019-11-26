//
// Created by jeff on 2019/11/25.
//

#ifndef POLICY_ENGINE_C_API_TEST_912_H
#define POLICY_ENGINE_C_API_TEST_912_H

#include <thread>
#include "policy_engine.h"
#include "microtest.h"

TEST(api_test_cc91_case_2) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::string cchost = "https://cc91-console.qapf1.qalab01.nextlabs.com";
        std::string ccport = "443" ;
        std::string ccuser = "Administrator" ;
        std::string ccpwd = "123Blue!123" ;
        std::string tag = "pe_jeff" ;
        PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
        3600);//1 hous
        PolicyEngineStringList sublist = NULL;
        PolicyEngineStringList actlist = NULL;
        PolicyEngineStringList resourcelist = NULL;
        PolicyEngineStringList host = NULL;
        PolicyEngineStringList app = NULL;
        ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);
        ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);


        policy_engine_destroy_string_list(sublist);
        policy_engine_destroy_string_list(actlist);
        policy_engine_destroy_string_list(resourcelist);
        policy_engine_destroy_string_list(host);
        policy_engine_destroy_string_list(app);

        // case 2
        {
            PolicyEngineHandle pdic_subjects = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_SUBJECT, &pdic_subjects);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "pe_nums", "100");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_host = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_HOST, &pdic_host);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_host, "aaaa", "vvvv");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_app = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_APPLICATION, &pdic_app);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_app, "aaa", "vvv");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }

            POLICY_ENGINE_MATCH_RESULT result;
            ret =  policy_engine_match( pdic_subjects, "DELETE", NULL,pdic_host, pdic_app , &result);
            ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            ASSERT_TRUE(result == PE_NO_MATCHED);

            policy_engine_destroy_dictionary(pdic_subjects);
            policy_engine_destroy_dictionary(pdic_host);
            policy_engine_destroy_dictionary(pdic_app);

        }
        policy_engine_module_exit();

}




#endif //POLICY_ENGINE_C_API_TEST_912_H