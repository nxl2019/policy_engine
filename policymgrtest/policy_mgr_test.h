#ifndef POLICY_MGR_TEST_H
#define POLICY_MGR_TEST_H

#include "microtest.h"
#include "patch.h"
#include "PolicyModelList.h"
#include "json/json.h"
#include "Lex.h"
#include "parse_expression.h"

TEST(POLICY_MGR_TEST_87_CASE_0) {
    PolicyModelList syms;
    std::vector<Json::Value> policys;
    PolicyHelper::DownloadPolicys(
            "https://cc87-console.qapf1.qalab01.nextlabs.com",
            "443",
            "administrator",
            "123blue!",
            "pe_test", policys, syms);
    ASSERT_TRUE(policys.size() == 4);
    std::sort(policys.begin(), policys.end(), [](const Json::Value& l, const Json::Value& r){
        auto left_id = l["id"].asInt();
        auto right_id = r["id"].asInt();
        return left_id < right_id;
    });
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, policys[0]);
    mt::printOk(json_file.c_str());
    ASSERT_TRUE(policys[0]["id"].asInt() == 11273);
    ASSERT_TRUE(policys[1]["id"].asInt() == 11274);
    ASSERT_TRUE(policys[2]["id"].asInt() == 11282);
    ASSERT_TRUE(policys[3]["id"].asInt() == 11284);

    std::string expression = policys[0]["expression"].asString();
    ASSERT_TRUE(expression.length() > 0);
    Lex lex(expression);
    lex.Next();
    ParseException e;
    e._syms = &syms;
    AstExpr *expr = parse_boolean_expr(&lex, &e);
    ASSERT_TRUE(e._code == ParseException::SUCCESS && expr != nullptr);
}

#endif