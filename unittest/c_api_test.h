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

TEST(release) {
    std::string cchost = "https://cc85-console.qapf1.qalab01.nextlabs.com" ;
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;
    std::string tag = "spe" ;
    PolicyEngineReturn ret = policy_engine_module_init(cchost.c_str(), ccport.c_str(), ccuser.c_str(), ccpwd.c_str(), tag.c_str(),
                                                       10);
    PolicyEngineStringList sublist = NULL;
    PolicyEngineStringList actlist = NULL;
    PolicyEngineStringList resourcelist = NULL;
    PolicyEngineStringList host = NULL;
    PolicyEngineStringList app = NULL;

    ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);


    const char * sub_v;
    ret = policy_engine_string_list_current(sublist, &sub_v);

    const char * act_v;
    ret = policy_engine_string_list_current(actlist, &act_v);

    PolicyEngineStringList current = sublist;
    PolicyEngineStringList temp = NULL;
    do {
        if (current == nullptr)
            break;
        const char * sub_v;
        ret = policy_engine_string_list_current(current, &sub_v);

        printf("%s\n", sub_v);

        temp = current;

    } while (policy_engine_string_list_next(temp, &current) ==  POLICY_ENGINE_SUCCESS);

    PolicyEngineHandle sub_create = NULL;
    ret =  policy_engine_create_dictionary_handle(PE_SUBJECT, &sub_create);
    ret =  policy_engine_insert_into_dictionary(sub_create, "groupid", "S-1-5-21-2018228179-1005617703-974104760-188953");

    POLICY_ENGINE_MATCH_RESULT result;
    ret =  policy_engine_match( sub_create, "DELETE", NULL,NULL, NULL , &result);

    std::this_thread::sleep_for(std::chrono::seconds(80));

    ret = policy_engine_destroy_string_list(sublist);
    ret = policy_engine_destroy_string_list(actlist);
    ret = policy_engine_destroy_string_list(resourcelist);
    ret = policy_engine_destroy_string_list(host);
    ret = policy_engine_destroy_string_list(app);


    ret = policy_engine_destroy_dictionary(sub_create);
    ret = policy_engine_module_exit();

    assert(ret == POLICY_ENGINE_SUCCESS);


    if (result == PE_NO_MATCHED) {
        /// return ?
    } else {
        ///
    }

}











#endif //POLICY_ENGINE_C_API_TEST_H
