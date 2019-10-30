#include "parse_expression.h"
#include "policy_expression.h"
#include "policy_regex.h"
#include "PolicyModelList.h"
#include <assert.h>

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

AstColumnRef::COL_TYPE lex_to_col(Token::TOKEN_TYPE tp) {
    switch (tp) {
        case Token::TK_HOST:    return AstColumnRef::HOST;
        case Token::TK_USER:    return AstColumnRef::SUB;
        case Token::TK_APP:     return AstColumnRef::APP;
        case Token::TK_RES:     return AstColumnRef::RES;
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
    AstExpr::EXPR_TYPE op;
    for (; is_binary_logic_op(lex->GetCurrent()->GetType(), op); ) {
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
        AstConstantValue *c = new AstConstantValue(AstExpr::C_NUMBER);
        c->SetValue(atoi(lex->GetCurrent()->GetWord().c_str()));
        lex->Next();
        return c;
    } else if (lex->GetCurrent()->GetType() == Token::TK_NULL) {
        AstConstantValue *c = new AstConstantValue(AstExpr::C_NULL);
        lex->Next();
        return c;
    } else {
        AstExpr *col_ref = parse_column_ref(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return col_ref;
    }
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
    } else if (lex->GetCurrent()->GetType() == Token::TK_NOT) {
        lex->Next();
        AstExpr *predicate = parse_predicate(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return new AstUnaryOpExpr(AstExpr::NOT, predicate);
    } else {
        AstExpr *value_left = parse_value(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        AstExpr::EXPR_TYPE op;
        if (!is_predicate_op(lex->GetCurrent()->GetType(), op)) {
            e->SetFail({/* todo */}, lex);
            delete (value_left);
            return nullptr;
        }
        lex->Next();
        AstExpr *value_right = parse_value(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            delete (value_left);
            return nullptr;
        }
        if (value_right->GetExprType() == AstExpr::C_PATTERN ||
                value_left->GetExprType() == AstExpr::C_PATTERN ) {
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

AstColumnRef::VAL_TYPE to_val_type(AttributeInfo::ATTR_TYPE attr_type) {
    switch (attr_type) {
        case AttributeInfo::A_NUMBER : return AstColumnRef::CC_NUMBER;
        case AttributeInfo::A_STRING : return AstColumnRef::CC_STRING;
        case AttributeInfo::A_MULTI : return AstColumnRef::CC_MULTI;
        default: return AstColumnRef::CC_OTHER;
    }
}

AstExpr *parse_column_ref(Lex *lex, ParseException *e) {
    AstColumnRef::COL_TYPE col_type;
    if (is_type(lex->GetCurrent()->GetType(), col_type)) {
        lex->Next();
        if (lex->GetCurrent()->GetType() != Token::TK_DOT) {
            e->SetFail(Token::TK_DOT, lex);
        }
        lex->Next();
        AstIds ids = parse_ids(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }

        AstColumnRef::VAL_TYPE val_type = AstColumnRef::CC_OTHER;
        if (e->_syms == NULL) { val_type = AstColumnRef::CC_OTHER;
        } else {
            switch (col_type) {
                case AstColumnRef::SUB: {
                    assert(ids.size() == 1);
                    val_type = to_val_type(e->_syms->GetAttrTypeByPmnameAttrName("user", ids.back()->GetId()));
                } break;
                case AstColumnRef::APP: {
                    assert(ids.size() == 1);
                    val_type = to_val_type(e->_syms->GetAttrTypeByPmnameAttrName("application", ids.back()->GetId()));
                } break;
                case AstColumnRef::HOST: {
                    assert(ids.size() == 1);
                    val_type = to_val_type(e->_syms->GetAttrTypeByPmnameAttrName("host", ids.back()->GetId()));
                } break;
                case AstColumnRef::RES: {
                    assert(ids.size() == 2);
                    val_type = to_val_type(e->_syms->GetAttrTypeByPmnameAttrName(ids[1]->GetId(), ids.back()->GetId()));
                } break;
                case AstColumnRef::ACTION: { val_type = AstColumnRef::CC_STRING; } break;
                default: { val_type = AstColumnRef::CC_OTHER; }
            }

        }
        return new AstColumnRef(col_type, val_type, ids);
    } else {
        AstIds ids = parse_ids(lex, e);
        if (e->_code != ParseException::SUCCESS) {
            return nullptr;
        }
        return new AstColumnRef(AstColumnRef::OTHER, AstColumnRef::CC_OTHER, ids);
    }
}

std::vector<AstColumnRef*> parse_oblication(LexOb *lex) {
    std::vector<AstColumnRef*> r;
    for (; lex->GetCurrent()->GetType() != Token::TK_END_P; ) {
        if (lex->GetCurrent()->GetType() == Token::TK_DOLLAR) {
            lex->Next();
            ParseException e;
            AstExpr *column_ref = parse_column_ref(lex, &e);
            if (e._code == ParseException::SUCCESS) {
                r.push_back(dynamic_cast<AstColumnRef*>(column_ref));
            }
        } else {
            lex->Next();
        }
    }
    return r;
}