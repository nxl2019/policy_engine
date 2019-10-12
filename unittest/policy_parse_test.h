//
// Created by jeff on 2019/10/11.
//

#ifndef POLICY_ENGINE_POLICY_PARSE_TEST_H
#define POLICY_ENGINE_POLICY_PARSE_TEST_H

#include "microtest.h"
#include "../policymgr/include/CCLoginHelper.h"
#include "../policymgr/include/CCPolicyHelper.h"
#include "Policy.h"
#include "json_string.h"

void get_policy(std::list<std::string> & policies);
void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies);




TEST(parse_policies)
{
    //json string
    //--JSON_TEST
    //parse to Policy(ast)
    Policy * ppolicy = new Policy();
    ppolicy->ParseFromJson(JSON_TEST);
   // ppolicy->Dump();
    //traversal ast  validate result
    AstExpr * pexpr = ppolicy->GetAst();
    ASSERT_TRUE(pexpr->GetExprType() == AstExpr::AND);
    {
        AstExpr * pexpr1 = dynamic_cast<AstBinaryOpExpr*>(pexpr)->GetLeft();
        ASSERT_TRUE(pexpr1->GetExprType() == AstExpr::AND);
        {
            AstExpr * pexpr2 = dynamic_cast<AstBinaryOpExpr*>(pexpr1)->GetLeft();
            ASSERT_TRUE(pexpr2->GetExprType() == AstExpr::AND);
            {
                AstExpr * pexpr3 = dynamic_cast<AstBinaryOpExpr*>(pexpr2)->GetLeft();
                ASSERT_TRUE(pexpr3->GetExprType() == AstExpr::OR);
                {
                    AstExpr * pexpr4 = dynamic_cast<AstBinaryOpExpr*>(pexpr3)->GetLeft();
                    ASSERT_TRUE(pexpr4->GetExprType() == AstExpr::COMP_EQ);
                }
                {
                    AstExpr * pexpr4 = dynamic_cast<AstBinaryOpExpr*>(pexpr3)->GetRight();
                    ASSERT_TRUE(pexpr4->GetExprType() == AstExpr::COMP_EQ);
                }
            }
            {
                AstExpr * pexpr3 = dynamic_cast<AstBinaryOpExpr*>(pexpr2)->GetRight();
                ASSERT_TRUE(pexpr3->GetExprType() == AstExpr::COMP_EQ);
            }
        }
        {
            AstExpr * pexpr2 = dynamic_cast<AstBinaryOpExpr*>(pexpr1)->GetRight();
            ASSERT_TRUE(pexpr2->GetExprType() == AstExpr::C_UNKNOWN);
        }
    }
    {
        AstExpr * pexpr1 = dynamic_cast<AstBinaryOpExpr*>(pexpr)->GetRight();
        ASSERT_TRUE(pexpr1->GetExprType() == AstExpr::C_TRUE);
    }

    delete ppolicy;




//    std::list<std::string>  policies;
//    get_policy(policies);
//    std::list<Policy*>  asts;
//    get_asts(asts, policies);
//
//    for (auto poly: asts) {
////      int ideep = 0 ;
////      deepness(poly->GetAst(), ideep);
////      printf("%d\n",ideep);
////      print(poly->GetAst(), 1);
//
//        poly->Dump();
//        printf("\n---------------------------\n");
//    }


}
//TEST(XX){
//    std::list<std::string>  policies;
//    get_policy(policies);
//    std::list<Policy*>  asts;
//    get_asts(asts, policies);
//
//    for (auto poly: asts) {
//        poly->Dump();
//        printf("\n---------------------------\n");
//    }
//}

void get_asts(std::list<Policy*> & asts, const std::list<std::string> & policies) {
    for (auto poly:policies) {
        Policy * policy = new Policy();
        policy->ParseFromJson(poly);
        asts.push_back(policy);
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

TEST(all_process) {
//    //GetPolicies
//    std::list<std::string>  policies;
//    get_policy(policies);
//    ASSERT_TRUE(policies.size() > 0);
//
//    //Parse
//    std::list<Policy*>  asts;
//    get_asts(asts, policies);
//    ASSERT_TRUE(policies.size() == asts.size());
//
//    //GetAttributesAndActions
//    Policy * ppolicy = asts.back();

    Policy * ppolicy = new Policy();
    ppolicy->ParseFromJson(JSON_TEST);

    ASSERT_NOTNULL(ppolicy);
    std::set<std::string> ractions;
    ppolicy->GetAction(ractions);
    std::set<std::string> rattributes;
    ppolicy->GetSubjectAttributes(rattributes);
    //TryMatch
    std::string key = "groupid";
    std::string value = "S-1-5-21-2018228179-1005617703-974104760-188953";
    Subject sub;
    {
        sub.InsertValue(key, value);
    }
    std::string action = "DELETE";
    BOOLEAN blret;
    ppolicy->TryMatch(&sub, action, blret);
    ppolicy->Dump();
    printf("TryMatch-------------------------------------\n");
    printf("  Input:SUB(%s, %s) ACTION(%s) \n", key.c_str(), value.c_str(), action.c_str());
    if (blret == BOOLEAN::B_TRUE) {
        printf("  Output:BOOLEAN::B_TRUE\n");
    } else if (blret == BOOLEAN::B_FALSE) {
        printf("  Output:BOOLEAN::B_FALSE\n");
    } else {
        printf("  Output:BOOLEAN::B_UNKNOWN\n");
    }
    ASSERT_TRUE(blret == BOOLEAN::B_UNKNOWN);
    delete ppolicy;
}




//void deepness(AstExpr * pexpr, int &  deep) {
//    deep++;
//    switch (pexpr->GetExprType()) {
//        case AstExpr::OR:
//        case AstExpr::AND:
//        case AstExpr::LIKE:
//        case AstExpr::NOT_LIKE:
//        case AstExpr::COMP_EQ:
//        case AstExpr::COMP_NEQ:
//        case AstExpr::COMP_GT:
//        case AstExpr::COMP_GE:
//        case AstExpr::COMP_LT:
//        case AstExpr::COMP_LE:{
//            int i = deep;
//            deepness(((AstBinaryOpExpr*)pexpr)->GetLeft(), i);
//            int j = deep;
//            deepness(((AstBinaryOpExpr*)pexpr)->GetRight(), j);
//
//            i > j?deep = i:deep = j;
//        } break;
//        case AstExpr::NOT: {
//            return ;
//        } break;
//        default:
//            break;
//    }
//}





#endif //POLICY_ENGINE_POLICY_PARSE_TEST_H
