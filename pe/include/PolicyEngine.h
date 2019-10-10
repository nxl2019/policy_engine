#ifndef CLASS_POLICY_ENGINE_H
#define CLASS_POLICY_ENGINE_H

#include <vector>
#include <string>
#include "policy_engine.h"

class StringList;
class Subject;
class Policy;

class PolicyEngine {
public:
    static PolicyEngine *Ins();
    static PolicyEngineReturn Init(const std::string& cchost, const std::string& ccport, const std::string& ccuser, const std::string& ccpwd, const std::string& tag,
                            unsigned int sync_interval_seconds);
    static PolicyEngineReturn Exit();
    PolicyEngineReturn Analyze(StringList **psubjects_string_list, StringList **pactions_string_list);  /* thread safe */
    PolicyEngineReturn Match(Subject *subject, const std::string& action, POLICY_ENGINE_MATCH_RESULT *presult); /* thread safe */
private:
    void Update();
    static void Sync();
private:
    std::string _cchost;
    std::string _ccport;
    std::string _ccuser;
    std::string _ccpwd;
    std::string _tag;
    unsigned int _sync_interval_seconds;
    std::vector<Policy*> _policys;
    static PolicyEngine *_ins;
    static bool _running_flag;
};


#endif