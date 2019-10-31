#ifndef PARSE_EXPRESSION_TEST_H
#define PARSE_EXPRESSION_TEST_H

#include "microtest.h"
#include "parse_expression.h"
#include "Lex.h"
#include "policy_expression.h"

TEST(PARSE_EXPRESSION_CASE_1) {
    Lex lex("resource.spe.url = \"sharepoint://**tagging**\"");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::LIKE);
    auto like = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(like->GetLeft()->GetExprType() == AstExpr::EXPR_COLUMN_REF);
    ASSERT_TRUE(dynamic_cast<AstColumnRef*>(like->GetLeft())->GetColType() == AstColumnRef::RES);
    ASSERT_TRUE(like->GetRight()->GetExprType() == AstExpr::C_PATTERN);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_2) {
    Lex lex("host.location != LakeHouse1");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::COMP_NEQ);
    auto neq = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(neq->GetLeft()->GetExprType() == AstExpr::EXPR_COLUMN_REF);
    ASSERT_TRUE(dynamic_cast<AstColumnRef*>(neq->GetLeft())->GetColType() == AstColumnRef::HOST);
    ASSERT_TRUE(neq->GetRight()->GetExprType() == AstExpr::EXPR_COLUMN_REF);
    ASSERT_TRUE(dynamic_cast<AstColumnRef*>(neq->GetRight())->GetColType() == AstColumnRef::OTHER);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_3) {
    Lex lex("(user.username = \"abraham.lincoln\" OR user.username = \"andrew.jackson\" OR user.username = \"barack.obama\" OR user.username = \"herbert.hoover\" OR user.username = \"john.tyler\" OR user.username = \"james.polk\" OR user.username = \"jimmy.carter\" OR user.username = \"bill.clinton\")");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    /* todo check some children node */
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_4) {
    Lex lex("(resource.spe.url = \"sharepoint://**aclinv.aspx\" OR resource.spe.url = \"sharepoint://**managefeatures.aspx\" OR resource.spe.url = \"sharepoint://**projectsite**flat.aspx\" OR resource.spe.url = \"sharpoint://**searchAndAddToHold.aspx\" OR resource.spe.url = \"sharepoint://**reghost.aspx\" OR resource.spe.url = \"sharepoint://**user.aspx\")");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    /* todo check some children node */
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_5) {
    Lex lex("(resource.spe.url = \"sharepoint://**/qct/Architecture/QCT Chipset Architecture/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/QCT Chipset Architecture\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMSW/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMSW\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/SysArch/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/SysArch\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/SoC-Security/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/SoC-Security\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/WTPower/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/WTPower\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Display/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Display\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Power/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Power\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/QIMMIP/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/QIMMIP\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Multimedia Departments Finance and CWF Tracking/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Multimedia Departments Finance and CWF Tracking\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/RICA/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/RICA\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Graphics/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/Graphics\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/MMIPPCdashboard/**\" OR resource.spe.url = \"sharepoint://**/qct/Architecture/MMCore/MMIPPCdashboard\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/APT/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/APT\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/buses/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/buses\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/architecture/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/architecture\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/iodevices/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/iodevices\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/power/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/power\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/DebugTrace/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/DebugTrace\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Drivers/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Drivers\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Kernel/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Kernel\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/RelEng/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/RelEng\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Storage/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/Storage\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TaskManagementServices/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TaskManagementServices\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TimeTracking/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TimeTracking\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TUNA/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/TUNA\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/usb/**\" OR resource.spe.url = \"sharepoint://**/qct/CoreBSP/usb\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Architecture/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Architecture\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Coex/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Coex\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Modems/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Modems\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Roadmap/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Roadmap\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/RFA/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/RFA\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/modemswlineorg/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/modemswlineorg\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/Modem1X/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/Modem1X\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/modemHDR/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/modemHDR\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/UIM/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/UIM\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/WCDMA/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/WCDMA\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/MPSSINT/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/MPSSINT\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/MPSS-QRD/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/MPSS-QRD\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/TSTS/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/TSTS\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/SD-GERAN/**\" OR resource.spe.url = \"sharepoint://**/qct/ModemSW/Tech/SD-GERAN\")");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    /* todo check some children node */
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_6) {
    Lex lex("(resource.spe.url = user.username OR user.emailaddress != \"james@nextlabs.com\" AND user.department = \"R&D\")");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::AND);
    auto and_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(and_expr->GetLeft()->GetExprType() == AstExpr::OR);
    ASSERT_TRUE(and_expr->GetRight()->GetExprType() == AstExpr::COMP_EQ);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_7) {
    Lex lex("(resource.spe.url = user.username OR (user.emailaddress != \"james@nextlabs.com\" AND user.department = \"R&D\"))");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::OR);
    auto and_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(and_expr->GetLeft()->GetExprType() == AstExpr::COMP_EQ);
    ASSERT_TRUE(and_expr->GetRight()->GetExprType() == AstExpr::AND);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_8) {
    Lex lex("((resource.hana_poc.\"abc_}def}_}\" = \"6\" AND resource.hana_poc.\"a.b\" = \"6\") OR resource.hana_poc.\"abc{efg}\" = user.\"timestamp@\")");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::OR);
    auto or_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(or_expr->GetLeft()->GetExprType() == AstExpr::AND);
    ASSERT_TRUE(or_expr->GetRight()->GetExprType() == AstExpr::COMP_EQ);
    delete (expr);
}
TEST(PARSE_EXPRESSION_CASE_9) {
    Lex lex("resource.spe.level > 10 AND USER.Level < 8 ");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::AND);
    auto or_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(or_expr->GetLeft()->GetExprType() == AstExpr::COMP_GT);
    ASSERT_TRUE(or_expr->GetRight()->GetExprType() == AstExpr::COMP_LT);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_10) {
    Lex lex("user.department includes \"R&D\"");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::INCLUDES);
    auto includes_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_11) {
    Lex lex("user.department EQUALS_UNORDERED \"R&D,HR\"");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::EQUALS_UNORDERED);
    auto equal_unordered = dynamic_cast<AstBinaryOpExpr*>(expr);
    delete (expr);
}

TEST(PARSE_EXPRESSION_CASE_12) {
    Lex lex("user.uid = null");
    lex.Next();
    ParseException e;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && lex.GetCurrent()->GetType() == Token::TK_END_P);
    ASSERT_TRUE(expr->GetExprType() == AstExpr::COMP_EQ);
    auto eq_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    ASSERT_TRUE(eq_expr->GetRight()->GetExprType() == AstExpr::C_NULL);
    delete (expr);
}


TEST(PARSE_OBLICATION_CASE_1) {
    LexOb lex("$user.location");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 1);
    auto ref = refs[0];
    ASSERT_TRUE(ref->GetColType() == AstColumnRef::SUB);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

TEST(PARSE_OBLICATION_CASE_2) {
    LexOb lex("12");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 0);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

TEST(PARSE_OBLICATION_CASE_3) {
    LexOb lex("$a.b.c");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 1);
    ASSERT_TRUE(refs[0]->GetColType() == AstColumnRef::OTHER);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

TEST(PARSE_OBLICATION_CASE_4) {
    LexOb lex("$user.location DATA $USER.address");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 2);
    auto ref = refs[0];
    ASSERT_TRUE(ref->GetColType() == AstColumnRef::SUB);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

TEST(PARSE_OBLICATION_CASE_5) {
    LexOb lex("$user.location DATA* $USER.address $$ $$HOST.abc");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 3);
    auto ref = refs[0];
    ASSERT_TRUE(ref->GetColType() == AstColumnRef::SUB);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}


TEST(PARSE_OBLICATION_CASE_6) {
    LexOb lex("$user.\"loc^ation\" DATA $USER.address $$ $$HOST.abc");
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 3);
    auto ref = refs[0];
    ASSERT_TRUE(ref->GetColType() == AstColumnRef::SUB);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

TEST(PARSE_OBLICATION_CASE_7) {
    LexOb lex("$user.\"loc^$user.ation\" DATA $USER.address $$ $$HOST.abc $$HOST.mn. ");  // incomplete id list
    lex.Next();
    std::vector<AstColumnRef*> refs = parse_oblication(&lex);
    ASSERT_TRUE(refs.size() == 3);
    auto ref = refs[0];
    ASSERT_TRUE(ref->GetColType() == AstColumnRef::SUB);
    for (auto it : refs) {
        delete (it);
    }
    refs.clear();
}

#endif