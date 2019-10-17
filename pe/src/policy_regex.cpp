#include "policy_regex.h"
#include <assert.h>

const std::regex MATCHED_ZERO_OR_MORE_CHARACTERS_NOT_INCLUDING_THE_PATH_SEPARATOR("[^/]*");
const std::regex MATCHED_ZERO_OR_MORE_CHARACTERS_INCLUDING_THE_PATH_SEPARATOR(".*");
/* todo */

bool is_policy_regex_reserved_wild(char c) {
    return c == 'd' || c == 'D' || c == 'a' || c == 'A'
           || c == 'c' || c == 'C' || c == 's' || c == 'S';
}

bool is_cpp_regex_reserved(char c) {
    return c == '^' || c == '$' || c == '\\' || c== '.'
           || c == '*' || c == '+' || c == '?' || c == '('
           || c == ')' || c == '[' || c == ']' || c == '{'
           || c == '}' || c == '|';
}

struct Pattern {
    enum P_TYPE { ORI, ONE_STAR, TWO_STAR,
        MATCH_d, NOT_d, MATCH_D, NOT_D, MATCH_a, NOT_a, MATCH_A, NOT_A,
        MATCH_c, NOT_c, MATCH_C, NOT_C, MATCH_s, NOT_s, MATCH_S, NOT_S } _type;
    std::string _ori;
};

Pattern::P_TYPE to_pattern_type(char ques_or_not, char wild) {
    if (ques_or_not == '?') {
        switch (wild) {
            case 'd' : return Pattern::MATCH_d;
            case 'D' : return Pattern::MATCH_D;
            case 'a' : return Pattern::MATCH_a;
            case 'A' : return Pattern::MATCH_A;
            case 'c' : return Pattern::MATCH_c;
            case 'C' : return Pattern::MATCH_C;
            case 's' : return Pattern::MATCH_s;
            case 'S' : return Pattern::MATCH_S;
            default: { assert(false); }
        }
    } else {
        switch (wild) {
            case 'd' : return Pattern::NOT_d;
            case 'D' : return Pattern::NOT_D;
            case 'a' : return Pattern::NOT_a;
            case 'A' : return Pattern::NOT_A;
            case 'c' : return Pattern::NOT_c;
            case 'C' : return Pattern::NOT_C;
            case 's' : return Pattern::NOT_s;
            case 'S' : return Pattern::NOT_S;
            default: { assert(false); }
        }
    }
}

std::string to_regex(Pattern::P_TYPE tp) {
    assert(tp != Pattern::ORI);
    switch (tp) {
        case Pattern::ONE_STAR: return "([^/]*)";
        case Pattern::TWO_STAR: return "(.*)";
        case Pattern::MATCH_d:  return "(\\d)";
        case Pattern::MATCH_D:  return "(\\d+)";
        case Pattern::MATCH_a:  return "([[:alpha:]])";
        case Pattern::MATCH_A:  return "([[:alpha:]]+)";
        case Pattern::MATCH_c:  return "(\\w)";
        case Pattern::MATCH_C:  return "(\\w+)";
        case Pattern::MATCH_s:  return "(\\s)";
        case Pattern::MATCH_S:  return "(\\s+)";
        /* NOT_XXX need some semantic confirm */
        case Pattern::NOT_d:    return "(^\\d)";
        case Pattern::NOT_D:    return "((\\d*(\\D+\\d*)+)|(q{0}))";
        case Pattern::NOT_a:    return "([^[:alpha:]])";
        case Pattern::NOT_A:    return "(([[:alpha:]]*([^[:alpha:]]+[[:alpha:]]*)+)|(q{0}))";
        case Pattern::NOT_c:    return "(^\\w)";
        case Pattern::NOT_C:    return "((\\w*(\\W+\\w*)+)|(q{0}))";
        case Pattern::NOT_s:    return "(^\\s)";
        case Pattern::NOT_S:    return "((\\s*(\\S+\\s*)+)|(q{0}))";
        default: { assert(false); }
    }
}

bool is_policy_regex(const std::string& text) {
    bool r = false;
    for (auto c : text) {
        if (c == '*' || c == '?' || c == '!') {
            r = true;
            break;
        }
    }
    return r;
}

bool is_policy_regex(const std::string& text, std::regex& rpolicy_regex) {
    /* we can convert the text to the std::regex if this is a policy regex */
    bool r = false;
    std::vector<Pattern> patterns;
    for (size_t i = 0; i < text.length(); ) {
        char c = text[i];
        int start = i;
        if (c == '*') {
            r = true;
            if ((i+1) < text.length() && text[i+1] == '*') {
                i += 2;
                patterns.push_back({Pattern::TWO_STAR, ""});
            } else {
                i += 1;
                patterns.push_back({Pattern::ONE_STAR, ""});
            }
        } else if (c == '?' || c == '!') {
            if ((i+1) < text.length() && is_policy_regex_reserved_wild(text[i+1])) {
                r = true;
                patterns.push_back({to_pattern_type(c, text[i+1]), ""});
            } else if ((i+1) < text.length()) {
                patterns.push_back({Pattern::ORI, text.substr(i+1, 1)});
            } else {
                /* ? ! error format */
            }
            i += 2;
        } else {
            while (i < text.length() && text[i] != '*' && text[i] != '?' && text[i] != '!') {
                ++i;
            }
            patterns.push_back({Pattern::ORI, text.substr(start, i-start)});
        }
    }

    if (r) {
        std::stringstream buf;
        for (auto& it : patterns) {
            if (it._type == Pattern::ORI) {
                for (auto c : it._ori) {
                    if (is_cpp_regex_reserved(c)) {
                        buf << "\\";
                        buf << c;
                    } else {
                        buf << c;
                    }
                }
            } else {

            }
        }
    }
    return r;
}
