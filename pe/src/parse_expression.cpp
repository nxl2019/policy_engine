#include "parse_expression.h"
#include "policy_expression.h"
#include "policy_regex.h"
#include <assert.h>

const std::vector<Token::TOKEN_TYPE > LOGIC_OP{ Token::TK_OR, Token::TK_AND };
const std::vector<Token::TOKEN_TYPE > COMP_OP{ Token::TK_EQ ,Token::TK_NEQ , Token::TK_LT ,
                                               Token::TK_LTEQ , Token::TK_GT , Token::TK_GTEQ };

bool is_logic_op(Token::TOKEN_TYPE op);

bool is_comp_op(Token::TOKEN_TYPE op);

AstExpr::EXPR_TYPE lexop_to_astop(Token::TOKEN_TYPE op) {
    switch (op) {
        case Token::TK_AND:     return AstExpr::AND;
        case Token::TK_OR:      return AstExpr::OR;
        case Token::TK_EQ:      return AstExpr::COMP_EQ;
        case Token::TK_NEQ:     return AstExpr::COMP_NEQ;
        case Token::TK_GT:      return AstExpr::COMP_GT;
        case Token::TK_GTEQ:    return AstExpr::COMP_GE;
        case Token::TK_LT:      return AstExpr::COMP_LT;
        case Token::TK_LTEQ:    return AstExpr::COMP_LE;
        default: { assert(false); }
    }
}

AstId   *parse_id(Lex *lex, ParseException *e);

AstIds   parse_ids(Lex *lex, ParseException *e);

AstExpr *parse_column_ref(Lex *lex, ParseException *e);

AstExpr *parse_predicate(Lex *lex, ParseException *e);

AstExpr *parse_boolean_expr(Lex *lex, ParseException *e) {
    AstExpr *expr = parse_predicate(lex, e);
    if (e->_code != ParseException::SUCCESS) {
        return nullptr;
    }
    for (; is_logic_op(lex->GetCurrent()->GetType()); ) {
        AstExpr::EXPR_TYPE op = lexop_to_astop(lex->GetCurrent()->GetType());
        lex->Next();
        AstExpr *left = expr;
        AstExpr *right = parse_predicate(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            delete (left);
            return nullptr;
        }
        expr = new AstBinaryOpExpr(op, left, right);
    }
    return expr;
}

AstExpr *parse_value(Lex *lex, ParseException *e) {
    if (lex->GetCurrent()->GetType() == Token::TK_STR_LITERAL) {
        //if (lex->GetCurrent()->GetWord().find("**") != std::string::npos) {
        if (is_policy_regex(lex->GetCurrent()->GetWord())) {
            AstConstantValue *c = new AstConstantValue(AstExpr::C_PATTERN);
            /* todo parse the pattern */
            c->SetValue(lex->GetCurrent()->GetWord());
            lex->Next();
            return c;
        } else {
            AstConstantValue *c = new AstConstantValue(AstExpr::C_STRING);
            c->SetValue(lex->GetCurrent()->GetWord());
            lex->Next();
            return c;
        }
    } else if (lex->GetCurrent()->GetType() == Token::TK_DEC_NUMBER) {
        AstConstantValue *c = new AstConstantValue(AstExpr::C_STRING);  /* todo */
        c->SetValue(lex->GetCurrent()->GetWord());
        return c;
    } else {
        AstExpr *col_ref = parse_column_ref(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return col_ref;
    }
}

bool is_logic_op(Token::TOKEN_TYPE op) {
    return op == Token::TK_OR || op == Token::TK_AND;
}

bool is_comp_op(Token::TOKEN_TYPE op) {
    return op == Token::TK_EQ || op == Token::TK_NEQ || op == Token::TK_LT ||
        op == Token::TK_LTEQ || op == Token::TK_GT || op == Token::TK_GTEQ;
}

AstId   *parse_id(Lex *lex, ParseException *e) {
    if (lex->GetCurrent()->GetType() != Token::TK_ID && lex->GetCurrent()->GetType() != Token::TK_STR_LITERAL) {
        e->SetFail({Token::TK_ID, Token::TK_STR_LITERAL}, lex);
        return nullptr;
    }
    AstId *id = new AstId(lex->GetCurrent()->GetWord());
    lex->Next();
    return id;
}

AstIds   parse_ids(Lex *lex, ParseException *e) {
    std::vector<AstId*> ids;
    AstId *id = parse_id(lex, e);
    if (e->_code != ParseException::SUCCESS) {
        return  ids;
    }
    ids.push_back(id);
    while (lex->GetCurrent()->GetType() == Token::TK_DOT) {
        lex->Next();
        id = parse_id(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            for (auto it : ids) delete (it);
            ids.clear();
            return ids;
        }
        ids.push_back(id);
    }
    return ids;
}

AstExpr *parse_predicate(Lex *lex, ParseException *e) {
    if (lex->GetCurrent()->GetType() == Token::TK_L_PAREN) {
        lex->Next();
        AstExpr *boolean_expr = parse_boolean_expr(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        if (lex->GetCurrent()->GetType() != Token::TK_R_PAREN) {
            e->SetFail(Token::TK_R_PAREN, lex);
            delete (boolean_expr);
            return nullptr;
        }
        lex->Next();
        return boolean_expr;
    } else {
        AstExpr *value_left = parse_value(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        if (!is_comp_op(lex->GetCurrent()->GetType())) {
            e->SetFail(COMP_OP, lex);
            delete (value_left);
            return nullptr;
        }
        AstExpr::EXPR_TYPE op = lexop_to_astop(lex->GetCurrent()->GetType());
        lex->Next();
        AstExpr *value_right = parse_value(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            delete (value_left);
            return nullptr;
        }
        if (value_right->GetExprType() == AstExpr::C_PATTERN ) {
            if (op != AstExpr::COMP_EQ && op != AstExpr::COMP_NEQ) {
                e->_code = ParseException::FAIL;
                delete (value_left); delete (value_right);
                return nullptr;
            }
            op = op == AstExpr::COMP_EQ ? AstExpr::LIKE : AstExpr::NOT_LIKE;    /* operator will be overload */
        }
        return new AstBinaryOpExpr(op, value_left, value_right);
    }
}

bool is_type(Token::TOKEN_TYPE tp) {
    return tp == Token::TK_HOST || tp == Token::TK_USER || tp == Token::TK_APP || tp == Token::TK_RES;
}

AstColumnRef::COL_TYPE lex_to_col(Token::TOKEN_TYPE tp) {
    switch (tp) {
        case Token::TK_HOST:    return AstColumnRef::HOST;
        case Token::TK_USER:    return AstColumnRef::SUB;
        case Token::TK_APP:     return AstColumnRef::APP;
        case Token::TK_RES:     return AstColumnRef::RES;
        default: { assert(false); }
    }
}

AstExpr *parse_column_ref(Lex *lex, ParseException *e) {
    if (is_type(lex->GetCurrent()->GetType())) {
        AstColumnRef::COL_TYPE col_type = lex_to_col(lex->GetCurrent()->GetType());
        lex->Next();
        if (lex->GetCurrent()->GetType() != Token::TK_DOT) {
            e->SetFail(Token::TK_DOT, lex);
        }
        lex->Next();
        AstIds ids = parse_ids(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return new AstColumnRef(col_type, ids);
    } else {
        AstIds ids = parse_ids(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return new AstColumnRef(AstColumnRef::OTHER, ids);
    }
}