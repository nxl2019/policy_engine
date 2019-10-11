//
// Created by jeff on 2019/10/11.
//

#ifndef POLICY_ENGINE_POLICY_PARSE_TEST_H
#define POLICY_ENGINE_POLICY_PARSE_TEST_H

#include "microtest.h"
#include "../policymgr/include/CCLoginHelper.h"
#include "../policymgr/include/CCPolicyHelper.h"
#include "Policy.h"
void get_policy(std::list<std::string> & policies);
void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies);

TEST(parse_policies)
{




    std::list<std::string>  policies;
    get_policy(policies);
    std::list<Policy*>  asts;
    get_asts(asts, policies);

    for (auto poly: asts) {
//        int ideep = 0 ;
//        deepness(poly->GetAst(), ideep);
//        printf("%d\n",ideep);
//
//        print(poly->GetAst(), 1);
        poly->Dump();
        printf("\n---------------------------\n");
    }


}

void get_policy(std::list<std::string> & policies) {
    std::string str_port = "443";
    std::string str_user_name = "administrator";
    std::string str_pwd = "123blue!";

    std::string str_ccservice = "https://cc85-console.qapf1.qalab01.nextlabs.com";
    std::string str_policy_tag = "spe";

//    std::string str_ccservice = "https://cc87-console.qapf1.qalab01.nextlabs.com";
//    std::string str_policy_tag = "emsmb";

    std::string str_session_cookie = CCLoginHelper::Login(str_ccservice, str_port, str_user_name, str_pwd);
    if (str_session_cookie.empty()) {
        return ;
    }
    std::list<std::string> lst_policy;
    if (!CCPolicyHelper::SyncPolicy(str_ccservice, str_port, str_session_cookie, str_policy_tag, policies)) {
        return ;
    }
}

void deepness(AstExpr * pexpr, int &  deep) {
    deep++;
    switch (pexpr->GetExprType()) {
        case AstExpr::OR:
        case AstExpr::AND:
        case AstExpr::LIKE:
        case AstExpr::NOT_LIKE:
        case AstExpr::COMP_EQ:
        case AstExpr::COMP_NEQ:
        case AstExpr::COMP_GT:
        case AstExpr::COMP_GE:
        case AstExpr::COMP_LT:
        case AstExpr::COMP_LE:{
            int i = deep;
            deepness(((AstBinaryOpExpr*)pexpr)->GetLeft(), i);
            int j = deep;
            deepness(((AstBinaryOpExpr*)pexpr)->GetRight(), j);

            i > j?deep = i:deep = j;
        } break;
        case AstExpr::NOT: {
            return ;
        } break;
        default:
            break;
    }
}
void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies) {
    for (auto poly:policies) {
        Policy * policy = new Policy();
        policy->ParseFromJson(poly);

        asts.push_back(policy);

    }

}




#endif //POLICY_ENGINE_POLICY_PARSE_TEST_H
