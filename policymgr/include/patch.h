#ifndef PATCH_H
#define PATCH_H

#include "json/json.h"
#include <map>
#include <vector>

typedef uint64_t PID;       /* Policy ID */
typedef uint64_t CID;       /* Component ID */
typedef uint64_t AID;       /* Action ID */
typedef uint64_t PMID;      /* Policy Model ID */

struct PmidName {
    PMID _pmid;
    std::string _attribute_name;
    bool operator<(const PmidName& rhs) const;
};

class NXLHttpClient;

class PolicyHelper {
public:
    static bool SyncPolicy(const std::string& service, const std::string& ort,
                           const std::string& session_cookie, const std::string& tag,
                           std::vector<std::string>& rpolicys);

private:
    // this api should return json like policy_template.txt
    static bool SearchPolicyByTag(NXLHttpClient& http_client, const std::string& session_cookie,
                              const std::string& tag, std::vector<Json::Value*>& policy_roots);
    // this api should return json like component.txt
    static bool SearchComponentByID(NXLHttpClient& http_client, CID id, Json::Value*& component);
    static bool SearchActionByID(NXLHttpClient& http_client, AID id, Json::Value*& action);
    static bool SearchPolicyModelByID(NXLHttpClient& httpClient, PMID pmid, Json::Value*& policy_model);
private:
    enum CC_COMPONENT_TYPE {HOST, APP, USER, RES};
    CC_COMPONENT_TYPE GetPolicyModelType(const std::map<PMID, Json::Value*>& from_http, PMID id);
    enum CC_TYPE {CC_NUMBER, CC_STRING, CC_MULTI};
    CC_TYPE GetAttributeType(const std::map<PMID, Json::Value*>& from_http, PMID, const std::string& attribute_name);
private:
    static bool Patch(NXLHttpClient& http_client, const std::vector<Json::Value*>& policy_roots);
    static bool PolicyAnalyzePatch(const std::vector<Json::Value*>& policy_roots,
            std::map<CID, std::vector<Json::Value *>> &component_patch_list, std::map<AID, std::vector<Json::Value *>> &action_patch_list);
    static bool ComponentApplyPatch(const std::map<CID, std::vector<Json::Value *>> &component_patch_list, const std::vector<Json::Value *>& from_http);
    static bool ActionApplyPatch(const std::map<AID, std::vector<Json::Value *>> &action_patch_list, const std::vector<Json::Value*>& from_http);
    static bool ComponentAnalyzePatch(const std::vector<Json::Value *>& component_roots,
            std::map<PMID, std::vector<Json::Value*>> &component_type_patch_list, std::map<PmidName, std::vector<Json::Value*>>& attribute_type_patch_list);
    static bool ComponentTypeApplyPatch(const std::map<PMID, std::vector<Json::Value*>> &component_type_patch_list, const std::map<PMID, Json::Value*>& from_http);
    static bool AttributeTypeApplyPatch(const std::map<PmidName, std::vector<Json::Value*>>& attribute_type_patch_list, const std::map<PMID, Json::Value*>& from_http);

};

#endif