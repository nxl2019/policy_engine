#ifndef POLICY_H
#define POLICY_H

#include <string>
#include <set>
#include "Value.h"
#include "policy_engine.h"
#include "Handle.h"
#include "Policy.h"

class AstExpr;
class AstColumnRef;

class Policy {
public:
    Policy() : _expr(nullptr) ,_pres_expr(nullptr) {}
    ~Policy();
    PolicyEngineReturn ParseFromJson(const std::string& json_string);
    void GetAction(std::set<std::string>& ractions);
    void GetSubjectAttributes(std::set<std::string>& subjectattrs);
    void GetResourceAttributes(std::set<std::string>& resourceattrs);
    void GetHost(std::set<std::string>& host);
    void GetApp(std::set<std::string>& app);
    void GetAllowObligation(std::set<std::string>& allow_obgs);
    void GetDenyObligation(std::set<std::string>& deny_obgs);
    PolicyEngineReturn TryMatch(const Subject *subject, const std::string& action, const Resource *res, const Host *host, const App *app , BOOLEAN & rboolean);
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
    std::set<std::string> _host;
    std::set<std::string> _app;
    std::set<std::string> _allow_obgs;
    std::set<std::string> _deny_obgs;

    std::vector<AstColumnRef*> _obg_cols;

};

#endif