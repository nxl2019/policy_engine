#include "task.h"
#include "tool.h"
#include <algorithm>

std::string Task::_host_name;
std::string Task::_port;
std::string Task::_user;
std::string Task::_pwd;
std::string Task::_tag;
unsigned int Task::_sync_interval_secs;

std::set<std::string> Task::_subject_ref;
std::set<std::string> Task::_resource_ref;
std::set<std::string> Task::_host_ref;
std::set<std::string> Task::_app_ref;
std::set<std::string> Task::_action_ref;

bool to_set(const Json::Value& array, std::set<std::string>& r) {
    if (array.type() != Json::arrayValue) return false;
    for (unsigned int i = 0; i < array.size(); ++i) {
        std::string v = array[i].asString();
        std::transform(v.begin(), v.end(), v.begin(), ::tolower);
        r.insert(v);
    }
    return true;
}

bool to_set(PolicyEngineStringList string_list, std::set<std::string>& r) {
    for (PolicyEngineStringList it = string_list; it != NULL;  policy_engine_string_list_next(it, &it)) {
        const char *string = nullptr;
        auto ret = policy_engine_string_list_current(it, &string);
        assert(ret == POLICY_ENGINE_SUCCESS);
        std::string v = string;
        std::transform(v.begin(), v.end(), v.begin(), ::tolower);
        r.insert(v);
    }
    return true;
}

bool to_dictionary(const Json::Value& obj, PolicyEngineHandle dictionary) {
    if (obj.type() != Json::objectValue) return false;
    auto members = obj.getMemberNames();
    for (auto key : members) {
        policy_engine_insert_into_dictionary(dictionary, key.c_str(), obj[key].asCString());
    }
    return true;
}

bool Task::ParseCC(const Json::Value& cc_params) {
    if (cc_params.type() != Json::objectValue) return false;
    _host_name = cc_params["host"].asCString();
    _port = cc_params["port"].asString();
    _user = cc_params["user"].asString();
    _pwd = cc_params["pwd"].asString();
    _tag = cc_params["tag"].asString();
    _sync_interval_secs = (unsigned)cc_params["sync_interval_secs"].asInt();
    auto ret = policy_engine_module_init(_host_name.c_str(), _port.c_str(), _user.c_str(), _pwd.c_str(), _tag.c_str(), _sync_interval_secs);
    return ret == POLICY_ENGINE_SUCCESS;
}

bool Task::ParseExpectRef(const Json::Value& expect_ref) {
    bool r = to_set(expect_ref["subject"], _subject_ref);
    if (!r) return false;
    r = to_set(expect_ref["resource"], _resource_ref);
    if (!r) return false;
    r = to_set(expect_ref["host"], _host_ref);
    if (!r) return false;
    r = to_set(expect_ref["application"], _app_ref);
    if (!r) return false;
    r = to_set(expect_ref["action"], _action_ref);
    if (!r) return false;
    return true;
}

bool Task::ValidAnalyze() {
    PolicyEngineStringList sublist = NULL;
    PolicyEngineStringList actlist = NULL;
    PolicyEngineStringList resourcelist = NULL;
    PolicyEngineStringList host = NULL;
    PolicyEngineStringList app = NULL;
    auto ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);
    if (ret != POLICY_ENGINE_SUCCESS) return false;
    std::set<std::string> set_sublist, set_actlist, set_resourcelist, set_hostlist, set_applist;
    to_set(sublist, set_sublist);
    to_set(actlist, set_actlist);
    to_set(resourcelist, set_resourcelist);
    to_set(host, set_hostlist);
    to_set(app, set_applist);

    policy_engine_destroy_string_list(sublist);
    policy_engine_destroy_string_list(actlist);
    policy_engine_destroy_string_list(resourcelist);
    policy_engine_destroy_string_list(host);
    policy_engine_destroy_string_list(app);

    return set_sublist == _subject_ref && set_actlist == _action_ref &&
        set_resourcelist == _resource_ref && set_hostlist == _host_ref && set_applist == _app_ref;
}

Task::Task() : _subject(nullptr), _resource(nullptr), _host(nullptr), _app(nullptr) { }

Task::~Task() {
    policy_engine_destroy_dictionary(_subject);
    policy_engine_destroy_dictionary(_resource);
    policy_engine_destroy_dictionary(_host);
    policy_engine_destroy_dictionary(_app);
}

bool Task::ParseMatch(const Json::Value& task) {
    policy_engine_create_dictionary_handle(PE_SUBJECT, &_subject);
    bool r = to_dictionary(task["subject"], _subject);
    if (!r) return false;
    policy_engine_create_dictionary_handle(PE_RESOURCE, &_resource);
    r = to_dictionary(task["resource"], _resource);
    if (!r) return false;
    policy_engine_create_dictionary_handle(PE_HOST, &_host);
    r = to_dictionary(task["host"], _host);
    if (!r) return false;
    policy_engine_create_dictionary_handle(PE_APPLICATION, &_app);
    r = to_dictionary(task["application"], _app);
    if (!r) return false;
    _action = task["action"].asString();
    if (0 == CommonFun::StrCaseCmp(task["expect_result"].asCString(), "PE_NO_MATCHED")) {
        _expect = PE_NO_MATCHED;
    } else {
        _expect = PE_NEED_MORE_WORK;
    }
    return true;
}

bool Task::ValidMatch() {
    POLICY_ENGINE_MATCH_RESULT result;
    auto r = policy_engine_match(_subject, _action.c_str(), _resource, _host, _app, &result);
    if (r != POLICY_ENGINE_SUCCESS) return false;
    return result == _expect;
}

