#ifndef LEX_TEST_H
#define LEX_TEST_H

#include "microtest.h"
#include "Lex.h"

TEST(LEX_CASE_1) {
    Lex lex("USER.emailaddress = \"james.polk@qapf1.qalab01.nextlabs.com\"");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEX_CASE_2) {
    Lex lex("resource.spe.url = \"sharepoint://**tagging**\"");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_RES);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEX_CASE_3) {
    Lex lex("host.location != LakeHouse1");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_HOST);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() ==  Token::TK_NEQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEX_CASE_4) {
    Lex lex("(user.username = \"abraham.lincoln\" OR user.username = \"andrew.jackson\" OR user.username = \"barack.obama\")");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_L_PAREN);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_OR);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_OR);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_R_PAREN);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEX_CASE_5) {
    Lex lex("(resource.spe.url = \"sharepoint://**aclinv.aspx\" AND resource.spe.url = \"sharepoint://**managefeatures.aspx\" )");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_L_PAREN);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_RES);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_AND);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_RES);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_EQ);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_R_PAREN);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

/* this case contain an unterminated string literal and cause an error */
TEST(LEX_CASE_6) {
    Lex lex("USER.\"email address");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ERR);
    mt::printOk(lex.GetCurrent()->GetWord().c_str());
}

TEST(LEX_CASE_7) {
    Lex lex("USER.\"pe_nums\" != 123 \n \t \r "
            "USER.\"pe_nums\" >= 59");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_STR_LITERAL);
    lex.Next();
    ASSERT_TRUE(Token::TK_NEQ == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_DEC_NUMBER == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_USER == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_DOT == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_STR_LITERAL == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_GTEQ    == lex.GetCurrent()->GetType());
    lex.Next();
    ASSERT_TRUE(Token::TK_DEC_NUMBER == lex.GetCurrent()->GetType());
    mt::printOk(lex.GetCurrent()->GetWord().c_str());
}

TEST(LEXOB_TEST_CASE_1) {
    LexOb lex("$user.location");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOLLAR);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEXOB_TEST_CASE_2) {
    LexOb lex("12");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}

TEST(LEXOB_TEST_CASE_3) {
    LexOb lex("$user.location Data");
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOLLAR);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_USER);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_DOT);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_ID);
    lex.Next();
    ASSERT_TRUE(lex.GetCurrent()->GetType() == Token::TK_END_P);
}



#endif