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
private:
    std::string     _json_string;
    AstExpr        *_expr;
};

#endif