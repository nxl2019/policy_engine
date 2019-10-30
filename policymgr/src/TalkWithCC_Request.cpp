//
// Created by jeff on 2019/10/25.
//
#include <string>
#include "TalkWithCC.h"
#include "NXLHttpClient.h"



http::request<http::string_body> TalkWithCCLower::ConstructPolicySearchRequestByTag(const std::string& tag, const int page) {
    //query policy
    char strsearch[2048];
    sprintf(strsearch, "\n"
                       "{\n"
                       "    \"criteria\":{\n"
                       "        \"fields\":[\n"
                       "            {\n"
                       "                \"field\":\"status\",\n"
                       "                \"type\":\"MULTI\",\n"
                       "                \"value\":{\n"
                       "                    \"type\":\"String\",\n"
                       "                    \"value\":[\n"
                       "                        \"APPROVED\"\n"
                       "                    ]\n"
                       "                }\n"
                       "            },\n"
                       "            {\n"
                       "                \"field\":\"tags\",\n"
                       "                \"nestedField\":\"tags.key\",\n"
                       "                \"type\":\"NESTED_MULTI\",\n"
                       "                \"value\":{\n"
                       "                    \"type\":\"String\",\n"
                       "                    \"value\":[\n"
                       "                        \"%s\"\n"
                       "                    ]\n"
                       "                }\n"
                       "            }\n"
                       "        ],\n"
                       "        \"sortFields\":[\n"
                       "            {\n"
                       "                \"field\":\"lastUpdatedDate\",\n"
                       "                \"order\":\"DESC\"\n"
                       "            }\n"
                       "        ],\n"
                       "        \"pageNo\":%d,\n"
                       "        \"pageSize\":20\n"
                       "    }\n"
                       "}", tag.c_str(), page);

    std::string criteria(strsearch);

    //construct request
    http::request<http::string_body> req{http::verb::post, "/console/api/v1/policy/search", 11, criteria};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::content_length, criteria.length());
    req.set(http::field::content_type, "application/json; charset=utf-8");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);

    return req;
}
http::request<http::string_body> TalkWithCCLower::ConstructPolicyRequestByID(const std::string& cid) {
    //query policy
    const static std::string strComponentSearchBaseUrl = "/console/api/v1/policy/mgmt/active/";
    const std::string strComponentSearchUrl = strComponentSearchBaseUrl + cid;

    //construct request
    http::request<http::string_body> req{http::verb::get, strComponentSearchUrl, 11};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);
    return req;
}
http::request<http::string_body> TalkWithCCLower::ConstructComponentRequestByID(const std::string& cid) {
    //query policy
    const static std::string strComponentSearchBaseUrl = "/console/api/v1/component/mgmt/active/";
    const std::string strComponentSearchUrl = strComponentSearchBaseUrl + cid;

    //construct request
    http::request<http::string_body> req{http::verb::get, strComponentSearchUrl, 11};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);
    return req;
}
http::request<http::string_body> TalkWithCCLower::ConstructPolicyModelRequestByID(const std::string& cid) {
    //query policy
    const static std::string strComponentSearchBaseUrl = "/console/api/v1/policyModel/mgmt/active/";
    const std::string strComponentSearchUrl = strComponentSearchBaseUrl + cid;

    //construct request
    http::request<http::string_body> req{http::verb::get, strComponentSearchUrl, 11};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);
    return req;
}
http::request<http::string_body> TalkWithCCLower::ConstructPolicyModelListRequest( const int page) {
    //query
    char strsearch[2048];
    sprintf(strsearch,
            "{\n"
                         "    \"criteria\":{\n"
                         "        \"fields\":[\n"
                         "            {\n"
                         "                \"field\":\"type\",\n"
                         "                \"type\":\"MULTI\",\n"
                         "                \"value\":{\n"
                         "                    \"type\":\"String\",\n"
                         "                    \"value\":[\n"
                         "                        \"RESOURCE\",\n"
                         "                        \"SUBJECT\"\n"
                         "                    ]\n"
                         "                }\n"
                         "            }\n"
                         "        ],\n"
                         "        \"sortFields\":[\n"
                         "            {\n"
                         "                \"field\":\"type\",\n"
                         "                \"order\":\"DESC\"\n"
                         "            },\n"
                         "            {\n"
                         "                \"field\":\"lastUpdatedDate\",\n"
                         "                \"order\":\"DESC\"\n"
                         "            }\n"
                         "        ],\n"
                         "        \"pageNo\":%d,\n"
                         "        \"pageSize\":20\n"
                         "    }\n"
                         "}", page);

    std::string criteria(strsearch);
    //construct request
    http::request<http::string_body> req{http::verb::post, "/console/api/v1/policyModel/search", 11, criteria};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::content_length, criteria.length());
    req.set(http::field::content_type, "application/json; charset=utf-8");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);

    return req;
}
http::request<http::string_body> TalkWithCCLower::ConstructPolicyModelPreAttrRequestByName(const std::string& name) {
    //query policy
    const static std::string strComponentSearchBaseUrl = "/console/api/v1/policyModel/mgmt/extraSubjectAttribs/";
    const std::string strComponentSearchUrl = strComponentSearchBaseUrl + name;
    //construct request
    http::request<http::string_body> req{http::verb::get, strComponentSearchUrl, 11};
    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, _set_cookie);
    return req;
}

 http::request<http::string_body> TalkWithCC91::ConstructPolicySearchRequestByTag(const std::string& tag,const int page) {
     auto req = TalkWithCCLower::ConstructPolicySearchRequestByTag(tag, page);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
}
 http::request<http::string_body> TalkWithCC91::ConstructPolicyRequestByID(const std::string& cid) {
     auto req = TalkWithCCLower::ConstructPolicyRequestByID(cid);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
 }
 http::request<http::string_body> TalkWithCC91::ConstructComponentRequestByID(const std::string& cid) {
     auto req = TalkWithCCLower::ConstructComponentRequestByID(cid);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
 }
 http::request<http::string_body> TalkWithCC91::ConstructPolicyModelRequestByID(const std::string& cid) {
     auto req = TalkWithCCLower::ConstructPolicyModelRequestByID(cid);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
 }
 http::request<http::string_body> TalkWithCC91::ConstructPolicyModelListRequest( const int page) {
     auto req = TalkWithCCLower::ConstructPolicyModelListRequest( page);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
 }
 http::request<http::string_body> TalkWithCC91::ConstructPolicyModelPreAttrRequestByName(const std::string& name) {
     auto req = TalkWithCCLower::ConstructPolicyModelPreAttrRequestByName(name);
     req.set(HEAD_X_CSRF_TOKEN, _csfr_token);
     return req;
 }
