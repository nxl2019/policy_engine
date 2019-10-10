#include "Policy.h"
#include "Handle.h"
#include "policy_expression.h"

Policy::~Policy() {
    delete (_expr);
    _expr = nullptr;
}

PolicyEngineReturn Policy::ParseFromJson(const std::string& json_string) {
    /* todo */
    return POLICY_ENGINE_TODO;
}

void Policy::GetAction(std::set<std::string>& ractions) {
    /* todo */
}

void Policy::GetSubjectAttributes(std::set<std::string>& rattributes) {
    /* todo */
}

PolicyEngineReturn Policy::TryMatch(const Subject *subject, const std::string& action, BOOLEAN& rboolean) {
    /* todo */
    return POLICY_ENGINE_TODO;
}

void Policy::Dump() {
    /* todo */
}

