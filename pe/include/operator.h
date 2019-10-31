#ifndef OPERATOR_H
#define OPERATOR_H

#include <string>
#include <map>
#include "policy_expression.h"

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > BINARY_LOGIC_OP{
        {Token::TK_OR, AstExpr::OR}, {Token::TK_AND, AstExpr::AND}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > UNARY_LOGIC_OP{
        {Token::TK_NOT, AstExpr::NOT}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > COMPARE_OP{
        {Token::TK_EQ, AstExpr::COMP_EQ}, {Token::TK_NEQ, AstExpr::COMP_NEQ},
        {Token::TK_LT, AstExpr::COMP_LT}, {Token::TK_LTEQ, AstExpr::COMP_LE},
        {Token::TK_GT, AstExpr::COMP_GT}, {Token::TK_GTEQ, AstExpr::COMP_GE}
};

const std::map<Token::TOKEN_TYPE, AstExpr::EXPR_TYPE > SPECIAL_OP{
        {Token::TK_INCLUDES, AstExpr::INCLUDES}, {Token::TK_EQUALS_UNORDERED, AstExpr::EQUALS_UNORDERED}
};

const std::map<Token::TOKEN_TYPE , AstColumnRef::COL_TYPE > TYPES{
        {Token::TK_HOST, AstColumnRef::HOST}, {Token::TK_USER, AstColumnRef::SUB},
        {Token::TK_APP, AstColumnRef::APP}, {Token::TK_RES, AstColumnRef::RES}
};

bool is_type(Token::TOKEN_TYPE tp, AstColumnRef::COL_TYPE& out) {
    auto fd = TYPES.find(tp);
    if (fd != TYPES.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_binary_logic_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = BINARY_LOGIC_OP.find(op);
    if (fd != BINARY_LOGIC_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_unary_logic_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = UNARY_LOGIC_OP.find(op);
    if (fd != UNARY_LOGIC_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_comp_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = COMPARE_OP.find(op);
    if (fd != COMPARE_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_special_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    auto fd = SPECIAL_OP.find(op);
    if (fd != SPECIAL_OP.end()) {
        out = fd->second;
        return true;
    } else return false;
}

bool is_predicate_op(Token::TOKEN_TYPE op, AstExpr::EXPR_TYPE& out) {
    return is_comp_op(op, out) || is_special_op(op, out);
}

#endif