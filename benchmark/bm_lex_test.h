#ifndef BM_LEX_TEST_H
#define BM_LEX_TEST_H

#include <benchmark/benchmark.h>
#include "Lex.h"
#include <assert.h>

static void LEX_CASE_1(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("USER.emailaddress = \"james.polk@qapf1.qalab01.nextlabs.com\"");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_USER);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_END_P);
    }
}
BENCHMARK(LEX_CASE_1);

static void LEX_CASE_2(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("resource.spe.url = \"sharepoint://**tagging**\"");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_RES);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_END_P);
    }
}
BENCHMARK(LEX_CASE_2);

static void LEX_CASE_3(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("host.location != LakeHouse1");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_HOST);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() ==  Token::TK_NEQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_END_P);
    }
}
BENCHMARK(LEX_CASE_3);

static void LEX_CASE_4(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("(user.username = \"abraham.lincoln\" OR user.username = \"andrew.jackson\" OR user.username = \"barack.obama\")");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_L_PAREN);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_USER);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_OR);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_USER);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_OR);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_USER);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_R_PAREN);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_END_P);
    }
}
BENCHMARK(LEX_CASE_4);

static void LEX_CASE_5(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("(resource.spe.url = \"sharepoint://**aclinv.aspx\" AND resource.spe.url = \"sharepoint://**managefeatures.aspx\" )");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_L_PAREN);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_RES);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_AND);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_RES);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ID);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_EQ);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_R_PAREN);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_END_P);
    }
}
BENCHMARK(LEX_CASE_5);

static void LEX_CASE_6(benchmark::State& state) {
    for (auto _ : state) {
        Lex lex("USER.\"email address");
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_USER);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_DOT);
        lex.Next();
        assert(lex.GetCurrent()->GetType() == Token::TK_ERR);
    }
}
BENCHMARK(LEX_CASE_6);

#endif