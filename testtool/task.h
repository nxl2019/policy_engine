#ifndef TASK_H
#define TASK_H

#include <string>
#include <set>
#include "policy_engine.h"
#include "json/json.h"

class Task{
public:
    Task();
    ~Task();
    bool ParseMatch(const Json::Value& task);
    bool ValidMatch();
public:
    static bool ParseCC(const Json::Value& cc_params);
    static bool ParseExpectRef(const Json::Value& expect_ref);
    static bool ValidAnalyze();
private:
    static std::string _host_name;
    static std::string _port;
    static std::string _user;
    static std::string _pwd;
    static std::string _tag;
    static unsigned int _sync_interval_secs;

    static std::set<std::string> _subject_ref;
    static std::set<std::string> _resource_ref;
    static std::set<std::string> _host_ref;
    static std::set<std::string> _app_ref;
    static std::set<std::string> _action_ref;
private:
    PolicyEngineSubject     _subject;
    PolicyEngineResource    _resource;
    PolicyEngineHost        _host;
    PolicyEngineApplication _app;
    std::string             _action;
    POLICY_ENGINE_MATCH_RESULT _expect;
};

#endif