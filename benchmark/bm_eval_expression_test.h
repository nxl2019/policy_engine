#ifndef BM_EVAL_EXPRESSION_TEST_H
#define BM_EVAL_EXPRESSION_TEST_H

#include <benchmark/benchmark.h>
#include <assert.h>
#include <vector>
#include "policy_expression.h"
#include "Handle.h"
#include "eval_expression.h"
#include "translate.h"

static void EVAL_EXPRESSION_CASE_1_A(benchmark::State& state) {
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
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {
        subject.InsertValue("M", "15");
        subject.InsertValue("N", "www.baidu.com");
    }
    for (auto _ : state) {
        BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        assert(b_e == B_FALSE);
    }
    delete (expr);
}
BENCHMARK(EVAL_EXPRESSION_CASE_1_A);

static void EVAL_EXPRESSION_CASE_1_I(benchmark::State& state) {
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
    for (auto _ : state) {
        BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        assert(b_i == B_FALSE);
    }
    delete (expr);
    free_code(instructions);
}
BENCHMARK(EVAL_EXPRESSION_CASE_1_I);

static void EVAL_EXPRESSION_CASE_2_A(benchmark::State& state) {
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
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {
        subject.InsertValue("M", "12");
    }
    for (auto _ : state) {
        BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        assert(b_e == B_TRUE);
    }
    delete (expr);
}
BENCHMARK(EVAL_EXPRESSION_CASE_2_A);

static void EVAL_EXPRESSION_CASE_2_I(benchmark::State& state) {
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
    for (auto _ : state) {
        BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        assert(b_i == B_TRUE);
    }
    delete (expr);
    free_code(instructions);
}
BENCHMARK(EVAL_EXPRESSION_CASE_2_I);

static void EVAL_EXPRESSION_CASE_3_A(benchmark::State& state) {
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
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {
        subject.InsertValue("M", "5");
    }
    for (auto _ : state) {
        BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        assert(b_e == B_UNKNOWN);
    }
    delete (expr);
}
BENCHMARK(EVAL_EXPRESSION_CASE_3_A);

static void EVAL_EXPRESSION_CASE_3_I(benchmark::State& state) {
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
    for (auto _ : state) {
        BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        assert(b_i == B_UNKNOWN);
    }
    delete (expr);
    free_code(instructions);
}
BENCHMARK(EVAL_EXPRESSION_CASE_3_I);

static void EVAL_EXPRESSION_CASE_SPE_1_A(benchmark::State& state) {
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
    Subject subject(PE_SUBJECT), resource(PE_RESOURCE), host(PE_HOST), app(PE_APPLICATION);
    {
        subject.InsertValue("EMAILADDRESS", "james.polk@qapf1.qalab01.nextlabs.com");
    }
    for (auto _ : state) {
        BOOLEAN b_e = eval_expression(expr, &subject, "OPEN", &resource, &host, &app);
        assert(b_e == B_UNKNOWN);
    }
    delete (expr);
}
BENCHMARK(EVAL_EXPRESSION_CASE_SPE_1_A);

static void EVAL_EXPRESSION_CASE_SPE_1_I(benchmark::State& state) {
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
    for (auto _ : state) {
        BOOLEAN b_i = eval_expression(instructions, &subject, "OPEN", &resource, &host, &app);
        assert(b_i == B_UNKNOWN);
    }
    delete (expr);
    free_code(instructions);
}
BENCHMARK(EVAL_EXPRESSION_CASE_SPE_1_I);


#endif