#ifndef EVAL_EXPRESSION_TEST_H
#define EVAL_EXPRESSION_TEST_H

#include <thread>
#include <vector>
#include "policy_expression.h"
#include "Handle.h"
#include "eval_expression.h"
#include "translate.h"
#include "microtest.h"

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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("M", "15");
            subject.InsertValue("N", "www.baidu.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_FALSE);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("M", "12");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_TRUE);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("M", "5");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_UNKNOWN);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("N", "www.yaha.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_UNKNOWN);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("N", "www.yaha.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_FALSE);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("M", "15");
            subject.InsertValue("N", "www.baidu.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_TRUE);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("N", "www.yaha.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_FALSE);
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("n", "WWW.BAIDU.COM");
            subject.InsertValue("M", "5");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_TRUE);
        std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(12),N(NIL) ---- TRUE";
        mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_9_MULTI_THREAD) {
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
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("n", "WWW.BAIDU.COM");
            subject.InsertValue("M", "5");
        }

        std::vector<std::thread*> threads;
        for (unsigned i = 0; i < 8; ++i) {
            threads.push_back(new std::thread([i, expr, &instructions, &subject, &resource, &host, &app](){
                Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
                Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
                if (b_e != b_i || b_i != Value::B_TRUE) {
                    std::string info = "EVAL_EXPRESSION_CASE_9_MULTI_THREAD at ";
                    info += std::to_string(i);
                    mt::printFailed(info.c_str());
                }
            }));
        }

        for (auto it : threads) {
            it->join();
            delete (it);
        }

        threads.clear();

        delete (expr);
        free_code(instructions);
        std::string info = "(SUB.M > 10 OR SUB.N = 'www.baidu.com') AND SUB.M < 15 ---- M(12),N(NIL) ---- TRUE";
        mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_SPE_1) {
        AstExpr *expr = nullptr;
        {
            // SUB.emailaddress = 'james.polk@qapf1.qalab01.nextlabs.com' AND ACTION.ACTION = 'open' AND TRUE AND UNKNOWN
            AstId *emailaddress = new AstId("emailaddress");
            AstExpr *ref_emailaddress = new AstColumnRef(AstColumnRef::SUB, {emailaddress});
            AstConstantValue *james = new AstConstantValue(AstExpr::C_STRING); james->SetValue("james.polk@qapf1.qalab01.nextlabs.com");
            AstExpr *u = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_emailaddress, james);

            AstId *action = new AstId("ACTION");
            AstExpr *ref_act = new AstColumnRef(AstColumnRef::ACTION,{action});
            AstConstantValue *open = new AstConstantValue(AstExpr::C_STRING); open->SetValue("open");
            AstExpr *a = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_act, open);

            expr = new AstBinaryOpExpr(AstExpr::AND, u, a);
            expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_TRUE));      /* this means without resource component */
            expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_UNKNOWN));   /* this means with an advance condition */
        }
        std::vector<Instruction*> instructions;
        {
            gen_code(expr, instructions);
        }
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("EMAILADDRESS", "james.polk@qapf1.qalab01.nextlabs.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_UNKNOWN);
        std::string info = "SUB.emailaddress = 'james.polk@qapf1.qalab01.nextlabs.com' AND ACTION.ACTION = 'open' AND TRUE AND UNKNOWN ---- EMAILADDRESS(james.polk@qapf1.qalab01.nextlabs.com),ACTION(OPEN) ----UNKNOWN";
        mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_SPE_2) {
        AstExpr *expr = nullptr;
        {
            AstId *emailaddress = new AstId("emailaddress");
            AstExpr *ref_emailaddress = new AstColumnRef(AstColumnRef::SUB, {emailaddress});
            AstConstantValue *james = new AstConstantValue(AstExpr::C_STRING); james->SetValue("james.polk@qapf1.qalab01.nextlabs.com");
            AstExpr *u = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_emailaddress, james);

            AstId *action = new AstId("ACTION");
            AstExpr *ref_act = new AstColumnRef(AstColumnRef::ACTION,{action});
            AstConstantValue *open = new AstConstantValue(AstExpr::C_STRING); open->SetValue("open");
            AstExpr *a = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_act, open);

            expr = new AstBinaryOpExpr(AstExpr::AND, u, a);
            expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_TRUE));      /* this means without resource component */
            expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_UNKNOWN));   /* this means with an advance condition */
        }
        std::vector<Instruction*> instructions;
        {
            gen_code(expr, instructions);
        }
        Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
        {
            subject.InsertValue("EMAILADDRESS", "EMMA.polk@qapf1.qalab01.nextlabs.com");
        }
        Value::BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        Value::BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        delete (expr);
        free_code(instructions);
        ASSERT_TRUE(b_e == b_i);
        ASSERT_TRUE(b_i == Value::B_FALSE);
        std::string info = "SUB.emailaddress = 'james.polk@qapf1.qalab01.nextlabs.com' AND ACTION.ACTION = 'open' AND TRUE AND UNKNOWN ---- EMAILADDRESS(EMMA.polk@qapf1.qalab01.nextlabs.com),ACTION(OPEN) ----FALSE";
        mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_SPE_3) {
    AstExpr *expr = nullptr;
    {
        AstId *emailaddress = new AstId("emailaddress");
        AstExpr *ref_emailaddress = new AstColumnRef(AstColumnRef::SUB, {emailaddress});
        AstConstantValue *james = new AstConstantValue(AstExpr::C_STRING); james->SetValue("james.polk@qapf1.qalab01.nextlabs.com");
        AstExpr *u = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_emailaddress, james);

        AstId *action = new AstId("ACTION");
        AstExpr *ref_act = new AstColumnRef(AstColumnRef::ACTION,{action});
        AstConstantValue *open = new AstConstantValue(AstExpr::C_STRING); open->SetValue("open");
        AstExpr *a = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_act, open);

        expr = new AstBinaryOpExpr(AstExpr::AND, u, a);
        expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_TRUE));      /* this means without resource component */
        expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_UNKNOWN));   /* this means with an advance condition */
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {
        subject.InsertValue("EMAILADDRESS", "james.polk@qapf1.qalab01.nextlabs.com");
    }
    Value::BOOLEAN b_e = eval_expression(expr, &subject, "EDIT", &resource, &host, &app);
    Value::BOOLEAN b_i = eval_expression(instructions, &subject, "EDIT", &resource, &host, &app);
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == Value::B_FALSE);
    std::string info = "SUB.emailaddress = 'james.polk@qapf1.qalab01.nextlabs.com' AND ACTION.ACTION = 'open' AND TRUE AND UNKNOWN ---- EMAILADDRESS(james.polk@qapf1.qalab01.nextlabs.com),ACTION(EDIT) ----FALSE";
    mt::printOk(info.c_str());
}

TEST(EVAL_EXPRESSION_CASE_SPE_4) {
    AstExpr *expr = nullptr;
    {
        AstId *emailaddress = new AstId("emailaddress");
        AstExpr *ref_emailaddress = new AstColumnRef(AstColumnRef::SUB, {emailaddress});
        AstConstantValue *james = new AstConstantValue(AstExpr::C_STRING); james->SetValue("james.polk@qapf1.qalab01.nextlabs.com");
        AstExpr *u = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_emailaddress, james);

        AstId *action = new AstId("ACTION");
        AstExpr *ref_act = new AstColumnRef(AstColumnRef::ACTION,{action});
        AstConstantValue *open = new AstConstantValue(AstExpr::C_STRING); open->SetValue("open");
        AstExpr *a = new AstBinaryOpExpr(AstExpr::COMP_EQ, ref_act, open);

        expr = new AstBinaryOpExpr(AstExpr::AND, u, a);
        expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_TRUE));      /* this means without resource component */
        expr = new AstBinaryOpExpr(AstExpr::AND, expr, new AstConstantValue(AstExpr::C_UNKNOWN));   /* this means with an advance condition */
    }
    std::vector<Instruction*> instructions;
    {
        gen_code(expr, instructions);
    }
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {

    }
    Value::BOOLEAN b_e = eval_expression(expr, &subject, "EDIT", &resource, &host, &app);
    Value::BOOLEAN b_i = eval_expression(instructions, &subject, "EDIT", &resource, &host, &app);
    delete (expr);
    free_code(instructions);
    ASSERT_TRUE(b_e == b_i);
    ASSERT_TRUE(b_i == Value::B_FALSE);
    std::string info = "SUB.emailaddress = 'james.polk@qapf1.qalab01.nextlabs.com' AND ACTION.ACTION = 'open' AND TRUE AND UNKNOWN ---- EMAILADDRESS(NIL),ACTION(EDIT) ----FALSE";
    mt::printOk(info.c_str());
}

#endif
