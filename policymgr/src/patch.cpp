#include "patch.h"
#include "NXLHttpClient.h"

#define KEY_ACTION_COMPONENTS   "actionComponents"
#define KEY_SUBJECT_COMPONENTS  ""


bool PolicyHelper::SyncPolicy(const std::string& service, const std::string& port,
                       const std::string& session_cookie, const std::string& tag,
                       std::vector<std::string>& rpolicys) {
    NXLHttpClient http_client(service.c_str(), port.c_str());
    std::vector<Json::Value*> policy_roots;
    SearchPolicyByTag(http_client, session_cookie, tag, policy_roots);
    for (auto it : policy_roots) {
        // ? need convert to string
    }
    return true;
}

bool PolicyHelper::SearchPolicyByTag(NXLHttpClient& http_client, const std::string& session_cookie,
                       const std::string& tag, std::vector<Json::Value*>& policy_roots) {
    /* todo */ assert(false);
}

bool PolicyHelper::SearchComponentByID(NXLHttpClient& http_client, CID id, Json::Value*& component) {
    /* todo */ assert(false);
}

bool PolicyHelper::SearchActionByID(NXLHttpClient& http_client, AID id, Json::Value*& action) {
    /* todo */ assert(false);
}

bool PolicyHelper::SearchPolicyModelByID(NXLHttpClient& httpClient, PMID pmid, Json::Value*& policy_model) {
    /* todo */ assert(false);
    // here once the pmid is host | user | application, the extra attribute which is reserved should be queried
    // the extra attributes should be merged into the policy_model
    // pay attention
}

PolicyHelper::CC_COMPONENT_TYPE PolicyHelper::GetPolicyModelType(const std::map<PMID, Json::Value*>& from_http, PMID id) {
    /* todo */ assert(false);
}

PolicyHelper::CC_TYPE PolicyHelper::GetAttributeType(const std::map<PMID, Json::Value*>& from_http, PMID, const std::string& attribute_name) {
    /* todo */ assert(false);
}

bool PolicyHelper::Patch(NXLHttpClient& http_client, const std::vector<Json::Value*>& policy_roots) {
    std::map<CID, std::vector<Json::Value *>> component_patch_list;
    std::map<AID, std::vector<Json::Value *>> action_patch_list;
    PolicyAnalyzePatch(policy_roots, component_patch_list, action_patch_list);

    std::vector<Json::Value*> action_from_http;
    for (auto it : action_patch_list) {
        Json::Value *from_http = nullptr;
        SearchActionByID(http_client, it.first, from_http);
        action_from_http.push_back(from_http);
    }
    assert(action_from_http.size() == action_patch_list.size());
    ActionApplyPatch(action_patch_list, action_from_http);
    for (auto it : action_from_http) delete (it);
    action_from_http.clear();

    std::vector<Json::Value*> component_from_http;
    for (auto it : component_patch_list) {
        Json::Value *from_http = nullptr;
        SearchComponentByID(http_client, it.first, from_http);
        component_from_http.push_back(from_http);
    }

    std::map<PMID, std::vector<Json::Value*>> component_type_patch_list;
    std::map<PmidName, std::vector<Json::Value*>> attribute_type_patch_list;
    ComponentAnalyzePatch(component_from_http, component_type_patch_list, attribute_type_patch_list);

    std::map<PMID, Json::Value*> policy_model_from_http;
    for (auto it : component_type_patch_list) {
        Json::Value *from_http = nullptr;
        SearchPolicyModelByID(http_client, it.first, from_http);
        policy_model_from_http.insert(std::make_pair(it.first, from_http));
    }

    ComponentTypeApplyPatch(component_type_patch_list, policy_model_from_http);
    AttributeTypeApplyPatch(attribute_type_patch_list, policy_model_from_http);
    ComponentApplyPatch(component_patch_list, component_from_http);

    for (auto it : policy_model_from_http) delete (it.second);
    policy_model_from_http.clear();

    for (auto it : component_from_http) delete (it);
    component_from_http.clear();

    return true;
}

bool PolicyAnalyzePatch(const std::vector<Json::Value*>& policy_roots,
                        std::map<CID, std::vector<Json::Value *>> &component_patch_list, std::map<AID, std::vector<Json::Value *>> &action_patch_list) {
    for (auto it : policy_roots) {

    }
}
