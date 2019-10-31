//
// Created by jeff on 2019/10/11.
//

#ifndef POLICY_ENGINE_POLICY_PARSE_TEST_H
#define POLICY_ENGINE_POLICY_PARSE_TEST_H

#include "microtest.h"
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
    ppolicy->ParseFromJson(JSON_TEST,NULL);
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





#endif //POLICY_ENGINE_POLICY_PARSE_TEST_H
