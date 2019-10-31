//
// Created by jeff on 2019/10/31.
//

#ifndef POLICY_ENGINE_C_API_TEST3_H
#define POLICY_ENGINE_C_API_TEST3_H


#include "policy_engine.h"
#include "tool.h"

TEST(api_test_3_case_1) {
        std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
        std::string tag = "tags_pe_1";
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

        // case 1 : when you action is "DELETE" and the action unexpected in policies.
        bool bexpect = false;
        for (PolicyEngineStringList it = actlist; it != NULL;  policy_engine_string_list_next(it, &it)) {
            const char * act = nullptr;
            ret = policy_engine_string_list_current(it, &act);
            int ret = CommonFun::StrCaseCmp(act, "DELETE");
            if (ret == 0) {
                bexpect = true;
            }
        }
        if (!bexpect) {
            /* todo */
        }
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
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "username", "rekkles");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "department", "xxx");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "pe_nums", "123");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_host = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_HOST, &pdic_host);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_host, "windowssid", "1234");
                ret = policy_engine_insert_into_dictionary(pdic_host, "machinename", "xxx");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_app = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_APPLICATION, &pdic_app);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_app, "displayname", "PC-00000");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }

            POLICY_ENGINE_MATCH_RESULT result;
            ret =  policy_engine_match( pdic_subjects, "VIEW", NULL,pdic_host, pdic_app , &result);
            ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            ASSERT_TRUE(result == PE_NO_MATCHED);

            policy_engine_destroy_dictionary(pdic_subjects);
            policy_engine_destroy_dictionary(pdic_host);
            policy_engine_destroy_dictionary(pdic_app);

        }
        // case 3
        {
            PolicyEngineHandle pdic_subjects = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_SUBJECT, &pdic_subjects);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);

                ret = policy_engine_insert_into_dictionary(pdic_subjects, "windowssid", "candyyy");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "username", "xxx");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "pe_nums", "66");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "name", "xxx");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "department", "qa");
                ret = policy_engine_insert_into_dictionary(pdic_subjects, "company", "xxx");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_host = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_HOST, &pdic_host);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_host, "windowssid", "123");
                ret = policy_engine_insert_into_dictionary(pdic_host, "machinename", "abc");
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
            }
            PolicyEngineHandle pdic_app = NULL;
            {
                PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_APPLICATION, &pdic_app);
                ASSERT_TRUE(ret == POLICY_ENGINE_SUCCESS);
                ret = policy_engine_insert_into_dictionary(pdic_app, "displayname", "pc-00001");
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


#endif //POLICY_ENGINE_C_API_TEST3_H
