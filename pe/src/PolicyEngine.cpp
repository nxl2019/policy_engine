#include "PolicyEngine.h"
#include "Policy.h"
#include "Handle.h"
#include <shared_mutex>
#include <thread>
#include "patch.h"
#include "PolicyModelList.h"
#include "TalkWithCC.h"

PolicyEngine *PolicyEngine::_ins = nullptr;
bool PolicyEngine::_running_flag = false;

std::shared_timed_mutex __mutex;

PolicyEngine *PolicyEngine::Ins() {
    if (_ins == nullptr) _ins = new PolicyEngine;
    return _ins;
}

PolicyEngineReturn PolicyEngine::Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& tag,
                        unsigned int sync_interval_seconds) {
    if (Ins()->_running_flag) return POLICY_ENGINE_SUCCESS;
    Ins()->_cchost = cchost; Ins()->_ccport = ccport; Ins()->_ccuser = ccuser; Ins()->_ccpwd = ccpwd; Ins()->_tag = tag;
    if (sync_interval_seconds < 60) Ins()->_sync_interval_seconds = 60;
    else Ins()->_sync_interval_seconds = sync_interval_seconds;
    if (POLICY_ENGINE_SUCCESS != Ins()->Update()) return POLICY_ENGINE_CCCONNECT_ERROR;
    Ins()->_running_flag = true;
    std::thread thread(PolicyEngine::Sync);
    thread.detach();
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn PolicyEngine::Exit() {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    Ins()->_running_flag = false;
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngine::~PolicyEngine() {
    for (auto it : _policys) {
        delete (it);
    }
    _policys.clear();
}

PolicyEngineReturn PolicyEngine::Analyze(StringList **psubjects_strlist, StringList **pactions_strlist, StringList **presource_strlist, StringList **phost_strlist, StringList **papp_strlist) {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    std::set<std::string> subs, acts, ress, hosts, apps;
    {
        std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
        for (auto it : _policys) {
            it->GetAction(acts);
            it->GetSubjectAttributes(subs);
            it->GetResourceAttributes(ress);
            it->GetHost(hosts);
            it->GetApp(apps);
        }
    }
    *psubjects_strlist = StringList::MakeStringList(subs);
    *pactions_strlist = StringList::MakeStringList(acts);
    *presource_strlist = StringList::MakeStringList(ress);
    *phost_strlist = StringList::MakeStringList(hosts);
    *papp_strlist = StringList::MakeStringList(apps);

    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn PolicyEngine::Match(Subject *subject, const std::string& action, Resource *res, Host *host, App *app, POLICY_ENGINE_MATCH_RESULT *presult) {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    std::vector<Value::BOOLEAN > booleans;
    {
        std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
        for (auto it : _policys) {
            Value::BOOLEAN bl = Value::B_UNKNOWN;
            if (it->TryMatch(subject, action, res, host, app, bl) != POLICY_ENGINE_SUCCESS) {
                booleans.push_back(Value::B_UNKNOWN);
            } else {
                booleans.push_back(bl);
            }
        }
    }
    *presult = PE_NO_MATCHED;
    for (auto it : booleans) {
        if (it != Value::B_FALSE) {
            *presult = PE_NEED_MORE_WORK;
            break;
        }
    }
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn PolicyEngine::Update() {
    std::vector<Json::Value> jsons;
    PolicyModelList syms;
    bool r = PolicyHelper::DownloadPolicys(_cchost, _ccport, _ccuser, _ccpwd, _tag, jsons, syms);
    if (!r) return POLICY_ENGINE_CCCONNECT_ERROR;
    std::vector<Policy*> tmp;
    for (auto& it : jsons) {
        Policy *policy = new Policy;
        PolicyEngineReturn r  = policy->ParseFromJson(it, &syms);
        if (r != POLICY_ENGINE_SUCCESS) {
            delete (policy);
            continue;
        } else {
            tmp.push_back(policy);
        }
    }
    delete (syms.GetTalk()); syms.ClearTalk();
    {
        std::lock_guard<std::shared_timed_mutex> writerLock(__mutex);
        _policys.swap(tmp);
    }
    for (auto it : tmp) delete (it);
    tmp.clear();
    return POLICY_ENGINE_SUCCESS;
}

void PolicyEngine::Sync() {
    while (_running_flag) {
        std::this_thread::sleep_for(std::chrono::seconds(Ins()->_sync_interval_seconds));
        Ins()->Update();
    }
    delete (_ins);
    _ins = nullptr;
}




