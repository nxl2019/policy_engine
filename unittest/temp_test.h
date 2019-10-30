//
// Created by jeff on 2019/10/14.
//

#ifndef POLICY_ENGINE_TEMP_TEST_H
#define POLICY_ENGINE_TEMP_TEST_H


#include "microtest.h"
#include "CCLoginHelper.h"
#include "CCPolicyHelper.h"
#include "Policy.h"
#include "json_string.h"
#include "policy_expression.h"
#include <list>


//void get_policy(std::list<std::string> & policies);
//void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies);
//
//TEST(get_policy){
//    std::list<std::string>  policies;
//    get_policy(policies);
//    std::list<Policy*>  asts;
//    get_asts(asts, policies);
//
//    for (auto poly: asts) {
//        poly->Dump();
//        printf("\n---------------------------\n");
//        std::set<std::string> acts, subjs, ress, hosts, apps;
//
//        poly->GetAction(acts);
//        poly->GetSubjectAttributes(subjs);
//        poly->GetResourceAttributes(ress);
//        poly->GetHost(hosts);
//        poly->GetApp(apps);
//        printf("\nACTION:\n");
//        for (auto s: acts) {  printf(" %s ", s.c_str());  }
//        printf("\nSUBJECT:\n");
//        for (auto s: subjs) {  printf(" %s ", s.c_str());  }
//        printf("\nRESOURCE:\n");
//        for (auto s: ress) {  printf(" %s ", s.c_str());  }
//        printf("\nHOST:\n");
//        for (auto s: hosts) {  printf(" %s ", s.c_str());  }
//        printf("\nAPP:\n");
//        for (auto s: apps) {  printf(" %s ", s.c_str());  }
//        printf("\n---------------------------\n");
//    }
//}
//
//void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies) {
//    for (auto poly:policies) {
//        printf("%s\n",poly.c_str());
//        Policy * policy = new Policy();
//        policy->ParseFromJson(poly, NULL);
//        asts.push_back(policy);
//    }
//}
//
//void get_policy(std::list<std::string> & policies) {
//    std::string str_port = "443";
//    std::string str_user_name = "administrator";
//    std::string str_pwd = "123blue!";
//
////    std::string str_ccservice = "https://cc85-console.qapf1.qalab01.nextlabs.com";
////    std::string str_policy_tag = "spe";
//
////    std::string str_ccservice = "https://cc86-console-01.qapf1.qalab01.nextlabs.com";
////    std::string str_policy_tag = "hana_poc";
//
//    std::string str_ccservice = "https://cc87-console.qapf1.qalab01.nextlabs.com";
//    //std::string str_policy_tag = "emsmb";
//    std::string str_policy_tag = "pe_test";
//
//    std::string str_session_cookie = CCLoginHelper::Login(str_ccservice, str_port, str_user_name, str_pwd);
//    if (str_session_cookie.empty()) {
//        return ;
//    }
//    std::list<std::string> lst_policy;
//    if (!CCPolicyHelper::SyncPolicy(str_ccservice, str_port, str_session_cookie, str_policy_tag, policies)) {
//        return ;
//    }
//
//}
/*
TEST(CHAR_TEST) {
    const char * p_c = "你我他";
    int clen = strlen(p_c); //clen = 9
    const wchar_t * p_wc1 = L"abc";
    const wchar_t * p_wc2 = L"你我他";
    const char16_t * pchar16 = u"你我他";
    const char32_t * pchar32 = U"你我他";

    int wclen1 = wcslen(p_wc1); // len = 3
    int wclen2 = wcslen(p_wc2); // len = 3
// 	int wclen1 = ucslen(pchar16);
// 	int wclen2 = wcslen(pchar32);

    int csize = sizeof(char);
    int wcsize = sizeof(wchar_t);
    int wcsize_16 = sizeof(char16_t);
    int wcsize_32 = sizeof(char32_t);
    int point = sizeof(p_c);

    printf("%s", p_c);
}
*/
#endif //POLICY_ENGINE_TEMP_TEST_H
