#ifndef POLICY_REGEX
#define POLICY_REGEX

#include <regex>

bool is_policy_regex(const std::string& text);

bool is_policy_regex(const std::string& text, std::regex& rpolicy_regex);

#endif