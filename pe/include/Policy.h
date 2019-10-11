#ifndef POLICY_H
#define POLICY_H

#include <string>
#include <set>
#include "Value.h"
#include "policy_engine.h"

class Subject;

class AstExpr;

class Policy {
public:
    Policy() : _expr(nullptr) {}
    ~Policy();
    PolicyEngineReturn ParseFromJson(const std::string& json_string);
    void GetAction(std::set<std::string>& ractions);
    void GetSubjectAttributes(std::set<std::string>& rattributes);
    PolicyEngineReturn TryMatch(const Subject *subject, const std::string& action, BOOLEAN& rboolean);
    void Dump();    /* print the policy for debug */
    AstExpr * GetAst() { return  _expr; }
private:
    bool QueryActionsAndAttributes();

    std::string     _json_string;
    AstExpr        *    _expr;

    std::set<std::string> _actions;
    std::set<std::string> _attributes;

};

#endif