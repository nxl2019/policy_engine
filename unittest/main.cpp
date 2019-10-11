#include "policy_expression.h"
#include "Handle.h"
#include "eval_expression.h"
#include "translate.h"
#include <time.h>
#include "microtest.h"
#include "policy_parse_test.h"


TEST(EVAL_EXPRESSION_CASE_1) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("M", "15");
        subject.InsertValue("N", "www.baidu.com");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_FALSE);
    std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(15),N(www.baidu.com) ----FALSE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_2) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("M", "12");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_TRUE);
    std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(12),N(NIL) ---- TRUE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_3) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("M", "5");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_UNKNOWN);
    std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(5),N(NIL) ---- UNKNOWN";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_4) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("N", "www.yaha.com");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_UNKNOWN);
    std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(NIL),N(www.yaha.com) ---- UNKNOWN";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_5) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 AND SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::AND, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("N", "www.yaha.com");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_FALSE);
    std::string info = "(SUB.M > 10 AND SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(NIL),N(www.yaha.com) ---- FALSE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_6) {
    AstExpr *expr = nullptr;
    {
        //NOT ((SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15)
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = new AstUnaryOpExpr(AstExpr::NOT, and_expr);
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("M", "15");
        subject.InsertValue("N", "www.baidu.com");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_TRUE);
    std::string info = "NOT (SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(15),N(www.baidu.com) ---- B_TRUE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_7) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 AND SUB.N = 'www.baidu.com') AND SUB.M < 15 AND UNKNOWN
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::AND, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        and_expr = new AstBinaryOpExpr(AstExpr::AND, and_expr, new AstConstantValue(AstExpr::C_UNKNOWN));
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("N", "www.yaha.com");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_FALSE);
    std::string info = "(SUB.M > 10 AND SUB.N = 'www.baidu.com') AND SUB.M < 15 AND UNKNOWN ---- M(NIL),N(www.yaha.com) ---- FALSE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_8) {
    AstExpr *expr = nullptr;
    {
        //(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15
        AstId *m = new AstId("M");
        AstExpr *refm = new AstColumnRef(AstColumnRef::SUB, {m});
        AstConstantValue *c10 = new AstConstantValue(AstExpr::C_STRING);
        c10->SetValue("10");
        AstExpr *egt = new AstBinaryOpExpr(AstExpr::COMP_GT, refm, c10);

        AstId *n = new AstId("N");
        AstExpr *refn = new AstColumnRef(AstColumnRef::SUB, {n});
        AstConstantValue *cbd = new AstConstantValue(AstExpr::C_STRING);
        cbd->SetValue("www.baidu.com");
        AstExpr *eeq = new AstBinaryOpExpr(AstExpr::COMP_EQ, refn, cbd);

        AstId *m1 = new AstId("M");
        AstExpr *refm1 = new AstColumnRef(AstColumnRef::SUB, {m1});
        AstConstantValue *c15 = new AstConstantValue(AstExpr::C_STRING);
        c15->SetValue("15");
        AstExpr *elt = new AstBinaryOpExpr(AstExpr::COMP_LT, refm1, c15);

        AstExpr *or_expr = new AstBinaryOpExpr(AstExpr::OR, egt, eeq);
        AstExpr *and_expr = new AstBinaryOpExpr(AstExpr::AND, or_expr, elt);
        expr = and_expr;
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject;
    {
        subject.InsertValue("n", "WWW.BAIDU.COM");
        subject.InsertValue("M", "5");
    }
    BOOLEAN b_e = eval_expression(expr, &subject, "OPEN");
    BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN");
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == B_TRUE);
    std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(12),N(NIL) ---- TRUE";
    mt::printOk(info.c_str());
}


TEST_MAIN()












