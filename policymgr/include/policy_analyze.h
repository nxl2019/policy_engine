//
// Created by jeff on 2019/10/24.
//
#ifndef POLICY_ANALYZE_LIST
#define POLICY_ANALYZE_LIST

#include <json/json.h>




void get_policy_jsvalue_from_string(const std::string & str, Json::Value & out) {

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(str.c_str(), str.c_str() + str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return ;
    }
    delete (pread);
    pread = nullptr;


    Json::Value &jsdata = root["data"];
    out["id"] = jsdata["id"];
    out["name"] = jsdata["name"];
    {///actionComponents
        Json::Value &jsactioncomp = jsdata["actionComponents"];
        Json::Value jsactioncomps_w;
        for (auto it = jsactioncomp.begin();it != jsactioncomp.end(); ++it) {
            assert((*it).isMember("operator"));
            Json::Value jscomps_w;
            jscomps_w["operator"] = (*it)["operator"];

            assert((*it).isMember("components"));
            Json::Value &jscomps = (*it)["components"];
            Json::Value jscomps_arr;
            for (auto itc = jscomps.begin(); itc != jscomps.end(); ++itc) {
                Json::Value jsaction_w;
                assert((*itc)["id"]);
                jsaction_w["id"] = (*itc)["id"];
                jsaction_w["actions"] =  Json::Value::null;//Json::Value(NULL);
                jscomps_arr.append(jsaction_w);
            }
            jscomps_w["components"] = jscomps_arr;
            jsactioncomps_w.append(jscomps_w);
        }
        out["actionComponents"] = jsactioncomps_w;
    }

    {///subjectComponents
        Json::Value &jssubcomps = jsdata["subjectComponents"];
        Json::Value jssubcomps_arr;
        for (auto it = jssubcomps.begin();it != jssubcomps.end(); ++it) {
            assert((*it).isMember("operator"));
            Json::Value jscomps_w;
            jscomps_w["operator"] = (*it)["operator"];

            assert((*it).isMember("components"));
            Json::Value &jscomps = (*it)["components"];
            Json::Value jscomps_arr;
            for (auto itc = jscomps.begin(); itc != jscomps.end(); ++itc) {
                Json::Value jsconditions_w;
                assert((*itc)["id"]);
                jsconditions_w["id"] = (*itc)["id"];
                jsconditions_w["conditions"] = Json::Value::null;//Json::Value(NULL);
                jscomps_arr.append(jsconditions_w);
            }
            jscomps_w["components"] = jscomps_arr;
            jssubcomps_arr.append(jscomps_w);
        }
        out["subjectComponents"] = jssubcomps_arr;
    }

    {///fromResourceComponents
        Json::Value &jsrescomps = jsdata["fromResourceComponents"];
        Json::Value jsrescomps_arr;
        for (auto it = jsrescomps.begin();it != jsrescomps.end(); ++it) {
            assert((*it).isMember("operator"));
            Json::Value jscomps_w;
            jscomps_w["operator"] = (*it)["operator"];

            assert((*it).isMember("components"));
            Json::Value &jscomps = (*it)["components"];
            Json::Value jscomps_arr;
            for (auto itc = jscomps.begin(); itc != jscomps.end(); ++itc) {
                Json::Value jsconditions_w;
                assert((*itc)["id"]);
                jsconditions_w["id"] = (*itc)["id"];
                jsconditions_w["conditions"] = Json::Value::null;//Json::Value(NULL);
                jscomps_arr.append(jsconditions_w);
            }
            jscomps_w["components"] = jscomps_arr;
            jsrescomps_arr.append(jscomps_w);
        }
        out["fromResourceComponents"] = jsrescomps_arr;
    }

    ///denyObligations
    out["denyObligations"] = jsdata["denyObligations"];
    ///allowObligations
    out["allowObligations"] = jsdata["allowObligations"];
    ///expression
    out["expression"] = jsdata["expression"];

    {
        Json::StreamWriterBuilder builder;
        const std::string json_file = Json::writeString(builder, out);
    }

    return ;
}

void get_component_jsvalue_from_string(const std::string & str, Json::Value & out) {

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(str.c_str(), str.c_str() + str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return ;
    }
    delete (pread);
    pread = nullptr;

    Json::Value &jsdata = root["data"];
    out["id"] = jsdata["id"];
    out["policy_model_id"] = jsdata["policyModel"]["id"].asInt();
    out["conditions"] = jsdata["conditions"];

    return ;
}

void get_action_jsvalue_from_string(const std::string & str, Json::Value & out) {

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(str.c_str(), str.c_str() + str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return ;
    }
    delete (pread);
    pread = nullptr;

    Json::Value &jsdata = root["data"];
    out["id"] = jsdata["id"];
    out["policy_model_id"] = jsdata["policyModel"]["id"].asInt();
    out["actions"] = jsdata["actions"];

    return ;
}

// SearchPolicyByTag support func
//bool PolicyHelper::SearchPolicyByTag(NXLHttpClient& http_client, const std::string& session_cookie,
//                                     const std::string& tag, std::vector<Json::Value*>& policy_roots) {
//    std::list<std::string> pids;
//    search_policy(http_client, session_cookie, tag, pids);
//    std::list<std::string>::iterator it = pids.begin();
//    while (it != pids.end())////////////////query policy
//    {
//        const static std::string str_baseurl = "/console/api/v1/policy/mgmt/active/";
//        const std::string str_py_url = str_baseurl + (*it);
//        //construct request
//        http::request<http::string_body> req{http::verb::get, str_py_url, 11};
//        //set headers
//        req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
//        req.set(http::field::connection, "keep-alive");
//        req.set(http::field::accept, "application/json, text/plain, */*");
//        req.set(http::field::cookie, session_cookie);
//        //send request
//        boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
//        http::response<http::string_body> res;
//        try {
//            http_client.Request(req, buffer, res);
//        } catch (...) {
//            handle_exception(" CCLoginHelper::GetPolicyDetails ");
//            return false;
//        }
//
//        if (res.result() == http::status::ok) {
//            std::stringstream ss(res.body());
//            Json::Value * pv = create_policy_jsvalue_from_string(ss.str());
//            policy_roots.push_back(pv);
//        }
//    }
//
//
//}


//bool PolicyHelper::SearchComponentByID(NXLHttpClient& http_client, CID id, Json::Value*& component) {
////
////     http ....api/v1/component/mgmt/active/<cid>
////    {
////        "data":{
////            "policyModel":{
////                "id":1,
////            },
////            "conditions":[
////            {
////                "attribute":"pe_nums",
////                        "operator":"!=",
////                        "value":"66",
////                        "rhsType":"CONSTANT",
////                        "rhsvalue":"66"
////            },
////            {
////                "attribute":"pe_nums",
////                        "operator":"!=",
////                        "value":"68",
////                        "rhsType":"CONSTANT",
////                        "rhsvalue":"68"
////            }
////            ]
////        }
////    }
//}
//
//bool PolicyHelper::SearchActionByID(NXLHttpClient& http_client, AID id, Json::Value*& action) {
//
//    /*http ....api/v1/component/mgmt/active/<cid>
//    {
//        "data":{
//            "id":11268,
//                    "type":"ACTION",
//                    "policyModel":{
//                "id":74
//            },
//            "actions":[
//            "VIEW",
//                    "OPEN"
//            ]
//        }
//    }*/
//}
//
//bool PolicyHelper::SearchPolicyModelByID(NXLHttpClient& http_client, PMID pmid, Json::Value*& policy_model) {
//
//    // here once the pmid is host | user | application, the extra attribute which is reserved should be queried
//    // the extra attributes should be merged into the policy_model
//    // pay attention
//
//    /* http:...api/v1/policyModel/mgmt/active/
//    {
//        "data":{
//            "id":1,
//                    "shortName":"user",
//                    "attributes":[
//            {
//                "shortName":"user_var",
//                        "dataType":"STRING"
//            }
//            ]
//        }
//    }
//     */
//    // when user/host/applications
//    /* http: ...api/v1/policyModel/mgmt/extraSubjectAttribs/<policyModelName>
//     {
//        "data":[
//            {
//
//                "name":"license",
//                "shortName":"license",
//                "dataType":"STRING",
//
//            }
//        ]
//    }
//    */
//}
//
//PolicyHelper::CC_COMPONENT_TYPE PolicyHelper::GetPolicyModelType(const std::map<PMID, Json::Value*>& from_http, PMID id) {
//
//}
//
//PolicyHelper::CC_TYPE PolicyHelper::GetAttributeType(const std::map<PMID, Json::Value*>& from_http, PMID, const std::string& attribute_name) {
//
//}
//

















//void search_policy(NXLHttpClient &http_client, const std::string &session_cookie,
//                                  const std::string &str_tag, std::list<std::string> &pids)
//{
//    const std::string criteria_template ="{\n"
//                                         "    \"criteria\":{\n"
//                                         "        \"fields\":[\n"
//                                         "            {\n"
//                                         "                \"field\":\"status\",\n"
//                                         "                \"type\":\"MULTI\",\n"
//                                         "                \"value\":{\n"
//                                         "                    \"type\":\"String\",\n"
//                                         "                    \"value\":[\n"
//                                         "                        \"APPROVED\"\n"
//                                         "                    ]\n"
//                                         "                }\n"
//                                         "            },\n"
//                                         "            {\n"
//                                         "                \"field\":\"tags\",\n"
//                                         "                \"nestedField\":\"tags.key\",\n"
//                                         "                \"type\":\"NESTED_MULTI\",\n"
//                                         "                \"value\":{\n"
//                                         "                    \"type\":\"String\",\n"
//                                         "                    \"value\":[\n"
//                                         "                        \"sql\"\n"
//                                         "                    ]\n"
//                                         "                }\n"
//                                         "            }\n"
//                                         "        ],\n"
//                                         "        \"sortFields\":[\n"
//                                         "            {\n"
//                                         "                \"field\":\"lastUpdatedDate\",\n"
//                                         "                \"order\":\"DESC\"\n"
//                                         "            }\n"
//                                         "        ],\n"
//                                         "        \"pageNo\":0,\n"
//                                         "        \"pageSize\":20\n"
//                                         "    }\n"
//                                         "}";
//    //set policy tag;
//    std::string str_criteria;
//    bool bSetTag = set_policy_tag(criteria_template, str_tag, str_criteria);
//    if (!bSetTag) {
//        Log::WriteLog(log_fatal, "CCPolicyHelper::SearchPolicy set policy tag failed.");
//        return;
//    }
//    //request policy one page a time
//    int nPageNo = 0;
//    while (true)
//    {
//        //construct request
//        http::request<http::string_body> req{http::verb::post, "/console/api/v1/policy/search", 11, str_criteria};
//        //set headers
//        req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
//        req.set(http::field::connection, "keep-alive");
//        req.set(http::field::content_length, str_criteria.length());
//        req.set(http::field::content_type, "application/json; charset=utf-8");
//        req.set(http::field::accept, "application/json, text/plain, */*");
//        req.set(http::field::cookie, session_cookie);
//        //send request
//        boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
//        http::response<http::string_body> res;
//        try {
//            http_client.Request(req, buffer, res);
//        } catch (...) {
//            handle_exception(" CCLoginHelper::SearchPolicy ");
//            return;
//        }
//
//        //parse response
//        if (res.result() == http::status::ok) {
//            std::stringstream ss(res.body());
//            pt::ptree pt;
//            try {
//                pt::read_json<pt::ptree>(ss, pt);
//                std::string status_code = pt.get<std::string>("statusCode");
//                if (status_code == "1004") {
//                    pt::ptree &ptDataArray = pt.get_child("data");
//                    for (auto itData = ptDataArray.begin(); itData != ptDataArray.end(); itData++) {
//                        std::string strid = itData->second.get<std::string>("id");
//                        pids.push_back(strid);
//                    }
//                    //next page
//                    int nTotalPage = pt.get<int>("totalPages");
//                    if (nTotalPage - 1 > nPageNo) {
//                        nPageNo++;
//                        set_page_no_criteria(str_criteria, nPageNo, str_criteria);
//                    } else {
//                        break;
//                    }
//                } else {
//                    Log::WriteLog(log_error, "CCPolicyHelper::SearchPolicy data status is not success, status=%s, body=%s",
//                            status_code.c_str(), res.body().c_str());
//                    break;
//                }
//            } catch (...) {
//                handle_exception(" CCLoginHelper::SearchPolicy ");
//                return;
//            }
//        } else {
//            Log::WriteLog(log_error, "CCPolicyHelper::SearchPolicy http status is not ok, status=%d, body=%s",
//                          res.result(), res.body().c_str());
//            break;
//        }
//    }
//}
//
//bool set_policy_tag(const std::string &str_old_c, const std::string &tag,
//                                  std::string &str_new_c)
//{
//    pt::ptree pt_criteria_obj;
//    try
//    {
//        std::stringstream ss(str_old_c);
//        pt::read_json<pt::ptree>(ss, pt_criteria_obj);
//        pt::ptree &ptCriteria = pt_criteria_obj.get_child("criteria");
//        pt::ptree &pt_fst_arr = ptCriteria.get_child("fields");
//        for (auto ptField = pt_fst_arr.begin(); ptField != pt_fst_arr.end(); ptField++) {
//            std::string strField = ptField->second.get<std::string>("field");
//            if (CommonFun::StrCaseCmp(strField.c_str(), "tags") == 0) {
//                pt::ptree &ptValue = ptField->second.get_child("value");
//                //remove old value
//                ptValue.erase("value");
//
//                //added new value
//                pt::ptree ptreeTagValue;
//                ptreeTagValue.put("", tag);
//
//                pt::ptree arrayValue;
//                arrayValue.push_back(std::make_pair("", ptreeTagValue));
//                ptValue.push_back(std::make_pair("value", arrayValue));
//
//                //write to json
//                std::stringstream jsonstream;
//                pt::write_json(jsonstream, pt_criteria_obj, false);
//                str_new_c = jsonstream.str();
//
//                break;
//            }
//        }
//    } catch (...) {
//        handle_exception("set policy tag to criteria");
//        return false;
//    }
//
//    return true;
//}
//
//bool set_page_no_criteria(const std::string &str_old_c, int pageno, std::string &str_new_c)
//{
//    pt::ptree pt_criteria_obj;
//    try
//    {
//        std::stringstream ss(str_old_c);
//        pt::read_json<pt::ptree>(ss, pt_criteria_obj);
//        pt::ptree &ptCriteria = pt_criteria_obj.get_child("criteria");
//        //remove pageNo
//        ptCriteria.erase("pageNo");
//        //add new value
//        ptCriteria.put<int>("pageNo", pageno);
//        //write to json
//        std::stringstream jsonstream;
//        pt::write_json(jsonstream, pt_criteria_obj, false);
//        str_new_c = jsonstream.str();
//    } catch (...) {
//        handle_exception("set policy tag to criteria");
//        return false;
//    }
//
//    return true;
//}

#endif