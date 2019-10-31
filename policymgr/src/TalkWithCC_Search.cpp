//
// Created by jeff on 2019/10/26.
//
#include <json/json.h>
#include "TalkWithCC.h"
#include "tool.h"

bool parse_ids_by_json(const std::string &str, std::vector<std::string>& out, bool & have_nextpage);
bool parse_name2ids_by_json(const std::string &str, std::map<std::string, uint64_t, CommonFun::IgnoreCaseCmp> & outmap, bool & have_nextpage);

bool TalkWithCCLower::SearchPolicyIDsByTag(const std::string& tag, std::vector<std::string>& out)  {
     int ipage = 0;
     bool have_nextpage = true;
     while (have_nextpage) {
         auto req = ConstructPolicySearchRequestByTag(tag, ipage);
         std::string temp;
         if ( !GetRequestResult(temp,  req) )
             return false;
         if ( !parse_ids_by_json(temp, out, have_nextpage))
             return false;
         ipage++;
     }
    return true;

}
 bool TalkWithCCLower::SearchPolicyByID(const std::string& id, std::string& out) {
     auto req = ConstructPolicyRequestByID(id);
     //send request
     return GetRequestResult(out,  req);
}
 bool TalkWithCCLower::SearchComponentByID(const std::string& id, std::string& out) {
     auto req = ConstructComponentRequestByID(id);
     //send request
     return GetRequestResult(out,  req);
}
 bool TalkWithCCLower::SearchPolicyModelByID(const std::string& id, std::string& out) {
     auto req = ConstructPolicyModelRequestByID(id);
     //send request
     return GetRequestResult(out,  req);
}
 bool TalkWithCCLower::SearchPolicyModellist( std::map<std::string, uint64_t, CommonFun::IgnoreCaseCmp> & outmap)  {
     //send request
     int ipage = 0;
     bool have_nextpage = true;
     while (have_nextpage) {
         auto req = ConstructPolicyModelListRequest(ipage);
         std::string temp;
         if ( !GetRequestResult(temp,  req) )
             return false;
         if ( !parse_name2ids_by_json(temp, outmap, have_nextpage))
             return false;
         ipage++;
     }
     return true;
}
 bool TalkWithCCLower::SearchPolicyModelPreAttrByName(const std::string& name, std::string& out) {
     auto req = ConstructPolicyModelPreAttrRequestByName(name);
     //send request
     return GetRequestResult(out,  req);
}



bool parse_ids_by_json(const std::string &str, std::vector<std::string>& out, bool & have_nextpage) {

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(str.c_str(), str.c_str() + str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return false;
    }
    delete (pread);
    pread = nullptr;
    assert(root.isMember("data"));
    Json::Value data = root["data"];
    for (auto it = data.begin(); it != data.end(); ++it) {
        assert((*it).isMember("id"));
        out.push_back((*it)["id"].asString());
    }
    int page_now = root["pageNo"].asInt();
    int page_all = root["totalPages"].asInt();
    if (page_all > page_now+1) {
        have_nextpage = true;
    } else {
        have_nextpage = false;
    }
    return true;
}

bool parse_name2ids_by_json(const std::string &str, std::map<std::string, uint64_t, CommonFun::IgnoreCaseCmp> & outmap, bool & have_nextpage) {

    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(str.c_str(), str.c_str() + str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return false;
    }
    delete (pread);
    pread = nullptr;
    assert(root.isMember("data"));
    Json::Value data = root["data"];
    for (auto it = data.begin(); it != data.end(); ++it) {
        assert((*it).isMember("id"));
        uint64_t  id = (*it)["id"].asInt();
        assert((*it).isMember("shortName"));
        std::string name = (*it)["shortName"].asString();
        transform(name.begin(), name.end(), name.begin(), tolower);
        outmap[name] = id;
    }
    int page_now = root["pageNo"].asInt();
    int page_all = root["totalPages"].asInt();
    if (page_all > page_now+1) {
        have_nextpage = true;
    } else {
        have_nextpage = false;
    }
    return true;
}



 bool TalkWithCC::GetRequestResult(std::string& out,  http::request<http::string_body> & req) {
     boost::beast::flat_buffer buffer;
     http::response<http::string_body> res;
     try {
         _http_client->Request(req, buffer, res);
     }
     catch (...) {
         return false;
     }
     //parse response
     if (res.result() == http::status::ok) {
         try {
             std::stringstream  ssout(res.body());
             out = ssout.str();
             return true;
         }
         catch(...) {
             return false;
         }
     } else {
         return false;
     }
}
