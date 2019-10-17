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

#endif