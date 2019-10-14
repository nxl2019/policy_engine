#ifndef PARSE_EXPRESSION_H
#define PARSE_EXPRESSION_H

#include "Lex.h"
#include <vector>


struct ParseException {
    ParseException() : _code(SUCCESS) {}
    void SetFail(Token::TOKEN_TYPE expect, Lex *lex) { _code = FAIL; }
    void SetFail(const std::vector<Token::TOKEN_TYPE >& expects, Lex *lex) { _code = FAIL; }
    enum { SUCCESS, FAIL } _code = SUCCESS;
    std::string _detail;
};

class AstExpr;

AstExpr *parse_boolean_expr(Lex *lex, ParseException *e);

AstExpr *parse_value(Lex *lex, ParseException *e);


#endif