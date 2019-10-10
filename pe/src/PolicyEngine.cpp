#include "PolicyEngine.h"
#include "Policy.h"
#include "Handle.h"
#include <shared_mutex>
#include <thread>

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
    Ins()->Update();
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

PolicyEngineReturn PolicyEngine::Analyze(StringList **psubjects_string_list, StringList **pactions_string_list) {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    std::set<std::string> subs, acts;
    {
        std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
        for (auto it : _policys) {
            it->GetAction(acts);
            it->GetSubjectAttributes(subs);
        }
    }
    *psubjects_string_list = StringList::MakeStringList(subs);
    *pactions_string_list = StringList::MakeStringList(acts);
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn PolicyEngine::Match(Subject *subject, const std::string& action, POLICY_ENGINE_MATCH_RESULT *presult) {
    if (!_running_flag) return POLICY_ENGINE_MODULE_NOT_INIT;
    std::vector<BOOLEAN > booleans;
    {
        std::shared_lock<std::shared_timed_mutex> readerLock(__mutex);
        for (auto it : _policys) {
            BOOLEAN bl = B_UNKNOWN;
            if (it->TryMatch(subject, action, bl) != POLICY_ENGINE_SUCCESS) {
                booleans.push_back(B_UNKNOWN);
            } else {
                booleans.push_back(bl);
            }
        }
    }
    *presult = PE_NO_MATCHED;
    for (auto it : booleans) {
        if (it != B_FALSE) {
            *presult = PE_NEED_MORE_WORK;
            break;
        }
    }
    return POLICY_ENGINE_SUCCESS;
}

void PolicyEngine::Update() {
    std::vector<Policy*> tmp;
    std::vector<std::string> jsons;
    /* todo fecth policy jsons */
    for (auto it : jsons) {
        Policy *policy = new Policy;
        PolicyEngineReturn r = policy->ParseFromJson(it);
        if (r != POLICY_ENGINE_SUCCESS) {
            delete (policy);
            continue;
        } else {
            tmp.push_back(policy);
        }
    }
    {
        std::lock_guard<std::shared_timed_mutex> writerLock(__mutex);
        _policys.swap(tmp);
    }
    for (auto it : tmp) delete (it);
    tmp.clear();
}

void PolicyEngine::Sync() {
    while (_running_flag) {
        std::this_thread::sleep_for(std::chrono::seconds(Ins()->_sync_interval_seconds));
        Ins()->Update();
    }
    delete (_ins);
    _ins = nullptr;
}




