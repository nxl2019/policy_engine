//
// Created by jeff on 2019/10/27.
//

#ifndef POLICY_ENGINE_POLICY_STR2JS_TEST_H
#define POLICY_ENGINE_POLICY_STR2JS_TEST_H
#include "microtest.h"
#include "policy_analyze.h"
#include <fstream>
#include "microtest.h"
#include "PolicyModelList.h"


std::string read_from_file(const std::string& file_path)
{
    std::ifstream t(file_path);
    std::string text((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return text;
}

const std::string strpath = "/home/jeff/policy_engine/policymgrtest/test_json/";
const std::string action11268 = "action11268.txt";
const std::string  component11293 =  "component11293.txt";
const std::string  policy11284 =  "policy11284.txt";
const std::string  policy_model2 =  "policy_model2.txt";
const std::string  search_host_preattr =  "search_host_preattr.txt";
const std::string  search_pm_list =  "search_pm_list.txt";
const std::string  search_policy_ids =  "search_policy_ids.txt";


TEST(str2js_policy) {
    std::string str = read_from_file(strpath+policy11284);
    Json::Value value;
    get_policy_jsvalue_from_string(str, value);

    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, value);
    printf("%s\n", json_file.c_str());
}
TEST(str2js_component) {
    std::string str = read_from_file(strpath+component11293);
    Json::Value value;
    get_component_jsvalue_from_string(str, value);

    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, value);
    printf("%s\n", json_file.c_str());
}
TEST(str2js_action) {
    std::string str = read_from_file(strpath+action11268);
    Json::Value value;
    get_action_jsvalue_from_string(str, value);

    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, value);
    printf("%s\n", json_file.c_str());
}
TEST(str2policymodel) {
    PolicyModel model;
    std::string str = read_from_file(strpath+policy_model2);
    model.ParseFromJson(str);
    std::string strattr = read_from_file(strpath+search_host_preattr);
    model.AddPreAttribute(strattr);

    ASSERT_TRUE(model._type == PolicyModel::PM_SUB_HOST);
    ASSERT_TRUE(model._attributes.size() == 6);

}












#endif //POLICY_ENGINE_POLICY_STR2JS_TEST_H
