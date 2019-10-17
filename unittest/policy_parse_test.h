//
// Created by jeff on 2019/10/11.
//

#ifndef POLICY_ENGINE_POLICY_PARSE_TEST_H
#define POLICY_ENGINE_POLICY_PARSE_TEST_H

#include "microtest.h"
#include "CCLoginHelper.h"
#include "CCPolicyHelper.h"
#include "Policy.h"
#include "json_string.h"
#include "policy_expression.h"
#include <list>

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
        ASSERT_TRUE(pexpr1->GetExprType() == AstExpr::LIKE);
    }

    delete ppolicy;


}



TEST(internal_api) {
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
    Dictionary sub(PE_SUBJECT);
    {
        sub.InsertValue(key, value);
    }
    Dictionary res(PE_RESOURCE);
    Dictionary host(PE_HOST);
    Dictionary app(PE_APPLICATION);
    std::string action = "DELETE";
    BOOLEAN blret;
    ppolicy->TryMatch(&sub, action, &res, &host, &app, blret);
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






#endif //POLICY_ENGINE_POLICY_PARSE_TEST_H
