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
    Policy() : _expr(nullptr) ,_pres_expr(nullptr) {}
    ~Policy();
    PolicyEngineReturn ParseFromJson(const std::string& json_string);
    void GetAction(std::set<std::string>& ractions);
    void GetSubjectAttributes(std::set<std::string>& subjectattrs);
    void GetResourceAttributes(std::set<std::string>& resourceattrs);
    PolicyEngineReturn TryMatch(const Subject *subject, const std::string& action, BOOLEAN& rboolean);
    void Dump();    /* print the policy for debug */
    AstExpr * GetAst() { return  _expr; }
    AstExpr * GetResAst() { return  _pres_expr; }
private:
    bool AnalyzeReference();

    std::string     _json_string;
    AstExpr        *    _expr;
    AstExpr         *   _pres_expr;

    std::set<std::string> _actions;
    std::set<std::string> _subjectattrs;
    std::set<std::string> _resourceattrs;

};

#endif