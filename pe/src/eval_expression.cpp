#include "eval_expression.h"
#include "policy_expression.h"
#include "Handle.h"
#include "Value.h"
#include "tool.h"
#include "Instruction.h"
#include <assert.h>
#include <stack>
#include <Instruction.h>
#include <regex>

struct RunTimeVars {
    Subject     *_subject;
    Value       *_action;
    Resource    *_resource;
    Host        *_host;
    App         *_app;
};

Value *make_action(const char *action) {
    Value *act = nullptr;
    if (action == nullptr || strlen(action) == 0) {
        act = new Value;
    } else {
        act = new Value(action);
    }
    return act;
}
Value eval(AstExpr *expr, RunTimeVars *run_time_vars);
Value test_int(AstExpr *expr, RunTimeVars *run_time_vars);
Value test_int_str(AstExpr *expr, RunTimeVars *run_time_vars);
Value test_like(AstExpr *expr, RunTimeVars *run_time_vars);

Value eval(AstExpr *expr, RunTimeVars *run_time_vars) {
    switch (expr->GetExprType()) {
        case AstExpr::OR: {
            AstBinaryOpExpr *or_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            Value l = eval(or_expr->GetLeft(), run_time_vars);
            if (l.GetType() != Value::V_BOOLEAN) {
                return Value(B_UNKNOWN);
            }
            if (l.GetValueAsBoolean() == B_TRUE) {
                return Value(B_TRUE);
            }
            Value r = eval(or_expr->GetRight(), run_time_vars);
            if (r.GetType() != Value::V_BOOLEAN) {
                return Value(B_UNKNOWN);
            }
            if (r.GetValueAsBoolean() == B_TRUE) {
                return Value(B_TRUE);
            }
            if (l.GetValueAsBoolean() == B_UNKNOWN || r.GetValueAsBoolean() == B_UNKNOWN) {
                return Value(B_UNKNOWN);
            }
            return r;   /* B_FALSE */
        } break;
        case AstExpr::AND: {
            AstBinaryOpExpr *and_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            Value l = eval(and_expr->GetLeft(), run_time_vars);
            if (l.GetType() != Value::V_BOOLEAN) {
                return Value(B_UNKNOWN);
            }
            if (l.GetValueAsBoolean() == B_FALSE) {
                return Value(B_FALSE);
            }
            Value r = eval(and_expr->GetRight(), run_time_vars);
            if (r.GetType() != Value::V_BOOLEAN) {
                return Value(B_UNKNOWN);
            }
            if (r.GetValueAsBoolean() == B_FALSE) {
                return Value(B_FALSE);
            }
            if (l.GetValueAsBoolean() == B_UNKNOWN || r.GetValueAsBoolean() == B_UNKNOWN) {
                return Value(B_UNKNOWN);
            }
            return r; /* B_TRUE */
        } break;
        case AstExpr::COMP_LE: /* go through */
        case AstExpr::COMP_LT: /* go through */
        case AstExpr::COMP_GE: /* go through */
        case AstExpr::COMP_GT: {
            return test_int(expr, run_time_vars);
        } break;
        case AstExpr::COMP_EQ: /* go through */
        case AstExpr::COMP_NEQ: {
            return test_int_str(expr, run_time_vars);
        } break;
        case AstExpr::LIKE: /* go through */
        case AstExpr::NOT_LIKE: {
            return test_like(expr, run_time_vars);
        } break;
        case AstExpr::NOT: {
            AstUnaryOpExpr *not_expr = dynamic_cast<AstUnaryOpExpr*>(expr);
            Value v = eval(not_expr->GetExpr(), run_time_vars);
            if (v.GetType() != Value::V_BOOLEAN) {
                return Value(B_UNKNOWN);
            }
            if (v.GetValueAsBoolean() == B_TRUE) {
                return Value(B_FALSE);
            } else if (v.GetValueAsBoolean() == B_FALSE) {
                return Value(B_TRUE);
            } else {
                return v;
            }
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            AstColumnRef *ref = dynamic_cast<AstColumnRef*>(expr);
            if (ref->GetColType() == AstColumnRef::RES) {
                return Value(B_UNKNOWN);    /* not support */
            } else if (ref->GetColType() == AstColumnRef::ACTION) {
                return *(run_time_vars->_action);
            } else if (ref->GetColType() == AstColumnRef::RES) {
                auto ids = ref->GetColumn();
                assert(ids.size() > 0);
                return run_time_vars->_resource->GetValueAsString(ids.back()->GetId());
            } else if (ref->GetColType() == AstColumnRef::HOST) {
                auto ids = ref->GetColumn();
                assert(ids.size() > 0);
                return run_time_vars->_host->GetValueAsString(ids.back()->GetId());
            } else if (ref->GetColType() == AstColumnRef::APP) {
                auto ids = ref->GetColumn();
                assert(ids.size() > 0);
                return run_time_vars->_app->GetValueAsString(ids.back()->GetId());
            } else if (ref->GetColType() == AstColumnRef::SUB) {
                auto ids = ref->GetColumn();
                assert(ids.size() > 0);
                return run_time_vars->_subject->GetValueAsString(ids.back()->GetId());
            } else {
                assert(false);
            }
        } break;
        case AstExpr::C_TRUE: {
            return Value(B_TRUE);
        } break;
        case AstExpr::C_FALSE: {
            return Value(B_FALSE);
        } break;
        case AstExpr::C_UNKNOWN: {
            return Value(B_UNKNOWN);
        } break;
        case AstExpr::C_NUMBER: {
            return Value(dynamic_cast<AstConstantValue*>(expr)->GetValueAsInt());
        } break;
        case AstExpr::C_PATTERN: /* go through */
        case AstExpr::C_STRING: {
            return Value(dynamic_cast<AstConstantValue*>(expr)->GetValueAsStr());
        } break;
        default: {  }
    }
    return Value(B_UNKNOWN);
}

Value test_int(AstExpr *expr, RunTimeVars *run_time_vars) {
    AstExpr::EXPR_TYPE op = expr->GetExprType();
    assert(op == AstExpr::COMP_LE || op == AstExpr::COMP_LT ||
    op == AstExpr::COMP_GE || op == AstExpr::COMP_GT);
    AstBinaryOpExpr *le_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    Value r = eval(le_expr->GetRight(), run_time_vars);
    if (r.GetType() != Value::V_INT && !r.CanConvertToInt()) {
        return Value(B_UNKNOWN);
    }
    Value l = eval(le_expr->GetLeft(), run_time_vars);
    if (l.GetType() != Value::V_INT && !l.CanConvertToInt()) {
        return Value(B_UNKNOWN);
    }
    if (op == AstExpr::COMP_LE) {
        if (l.GetValueAsInt() <= r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_LT) {
        if (l.GetValueAsInt() < r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_GE) {
        if (l.GetValueAsInt() >= r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_GT) {
        if (l.GetValueAsInt() > r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    } else {
        assert(false);
    }
    return Value(B_UNKNOWN);
}

Value test_int_str(AstExpr *expr, RunTimeVars *run_time_vars) {
    AstExpr::EXPR_TYPE op = expr->GetExprType();
    assert(op == AstExpr::COMP_EQ || op == AstExpr::COMP_NEQ);
    AstBinaryOpExpr *le_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    Value r = eval(le_expr->GetRight(), run_time_vars);
    if (r.GetType() != Value::V_INT && !r.CanConvertToInt()) {
        if (r.GetType() == Value::V_STRING) {
            Value l = eval(le_expr->GetLeft(), run_time_vars);
            if (l.GetType() != Value::V_STRING) {
                return Value(B_UNKNOWN);
            }
            if (op == AstExpr::COMP_EQ) {
                if (CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) == 0) return Value(B_TRUE);
                else return Value(B_FALSE);
            } else if (op == AstExpr::COMP_NEQ) {
                if (CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) == 0) return Value(B_FALSE);
                else return Value(B_TRUE);
            } else {
                assert(false);
            }
        }
        return Value(B_UNKNOWN);
    }
    Value l = eval(le_expr->GetLeft(), run_time_vars);
    if (l.GetType() != Value::V_INT && !l.CanConvertToInt()) {
        return Value(B_UNKNOWN);
    }
    if (op == AstExpr::COMP_EQ) {
        if (l.GetValueAsInt() == r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_NEQ) {
        if (l.GetValueAsInt() != r.GetValueAsInt()) {
            return Value(B_TRUE);
        }
        else {
            return Value(B_FALSE);
        }
    }  else {
        assert(false);
    }
    return Value(B_UNKNOWN);
}

std::regex to_regex(const std::string& v) {
    /* todo */
    std::stringstream buf{};
    for (size_t i = 0; i < v.length(); ++i) {
        if (v.at(i) == '*' && (i+1 < v.length()) && v.at(i+1) == '*') {
            buf << ".*";
            ++i;    /* skip one '*' */
        } else {
            buf << v.at(i);
        }
    }
    std::string ttt = buf.str();
    return std::regex(buf.str());
}

Value test_like(AstExpr *expr, RunTimeVars *run_time_vars) {
#if 0
    AstExpr::EXPR_TYPE op = expr->GetExprType();
    assert(op == AstExpr::LIKE || op == AstExpr::NOT_LIKE);
    AstBinaryOpExpr *binary_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
    Value r = eval(binary_expr->GetRight(), run_time_vars);
    if (r.GetType() != Value::V_STRING) {
        return Value(B_UNKNOWN);
    }
    std::regex pattern = to_regex(r.GetValueAsStr());
    Value l = eval(binary_expr->GetLeft(), run_time_vars);
    if (l.GetType() != Value::V_STRING) {
        return Value(B_UNKNOWN);
    }
    bool matched = std::regex_match(l.GetValueAsStr(), pattern);
    if (op == AstExpr::LIKE) {
        if (matched) return Value(B_TRUE);
        else return Value(B_FALSE);
    } else {
        if (matched) return Value(B_FALSE);
        else return Value(B_TRUE);
    }
#else
    return Value(B_UNKNOWN);
#endif
}


BOOLEAN eval_expression(AstExpr *expr, Subject *subject, const std::string& action, Resource *res, Host *host, App *app) {
    if (expr == nullptr) {
        return B_UNKNOWN;
    }
    if (subject->size() == 0 && action.length() == 0 && res->size() == 0 && host->size() == 0 && app->size() == 0) {
        return B_UNKNOWN;
    }
    Value *act = make_action(action.c_str());
    RunTimeVars run_time_vars{ subject, act, res, host, app };
    Value v = eval(expr, &run_time_vars);
    delete (act);
    if (v.GetType() == Value::V_BOOLEAN) {
        return v.GetValueAsBoolean();
    } else {
        return B_UNKNOWN;
    }
}

Value *test_int(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    assert(op == AstExpr::COMP_LE || op == AstExpr::COMP_LT ||
           op == AstExpr::COMP_GE || op == AstExpr::COMP_GT);
    if ((left->GetType() != Value::V_INT && !left->CanConvertToInt() ) ||
            (right->GetType() != Value::V_INT && !right->CanConvertToInt())) {
        return new Value(B_UNKNOWN);
    }
    if (op == AstExpr::COMP_LE) {
        if (left->GetValueAsInt() <= right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_LT) {
        if (left->GetValueAsInt() < right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_GE) {
        if (left->GetValueAsInt() >= right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_GT) {
        if (left->GetValueAsInt() > right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else {
        assert(false);
    }
    return new Value(B_UNKNOWN);
}

Value *test_int_str(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    assert(op == AstExpr::COMP_EQ || op == AstExpr::COMP_NEQ);
    if ((left->GetType() != Value::V_INT && !left->CanConvertToInt() ) ||
        (right->GetType() != Value::V_INT && !right->CanConvertToInt())) {
        if (left->GetType() == Value::V_STRING && right->GetType() == Value::V_STRING) {
            if (op == AstExpr::COMP_EQ) {
                if (CommonFun::StrCaseCmp(left->GetValueAsStr().c_str(), right->GetValueAsStr().c_str()) == 0) return new Value(B_TRUE);
                else return new Value(B_FALSE);
            } else if (op == AstExpr::COMP_NEQ) {
                if (CommonFun::StrCaseCmp(left->GetValueAsStr().c_str(), right->GetValueAsStr().c_str()) == 0) return new Value(B_FALSE);
                else return new Value(B_TRUE);
            } else {
                assert(false);
            }
        }
        return new Value(B_UNKNOWN);
    }
    if (op == AstExpr::COMP_EQ) {
        if (left->GetValueAsInt() == right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else if (op == AstExpr::COMP_NEQ) {
        if (left->GetValueAsInt() != right->GetValueAsInt()) {
            return new Value(B_TRUE);
        }
        else {
            return new Value(B_FALSE);
        }
    } else {
        assert(false);
    }
    return new Value(B_UNKNOWN);
}

Value *test_or(Value *left, Value *right) {
    if (left->GetType() != Value::V_BOOLEAN || right->GetType() != Value::V_BOOLEAN) {
        return new Value(B_UNKNOWN);
    }
    if (left->GetValueAsBoolean() == B_TRUE || right->GetValueAsBoolean() == B_TRUE) {
        return new Value(B_TRUE);
    }
    if (left->GetValueAsBoolean() == B_UNKNOWN || right->GetValueAsBoolean() == B_UNKNOWN) {
        return new Value(B_UNKNOWN);
    }
    return new Value(B_FALSE);
}

Value *test_and(Value *left, Value *right) {
    if (left->GetType() != Value::V_BOOLEAN || right->GetType() != Value::V_BOOLEAN) {
        return new Value(B_UNKNOWN);
    }
    if (left->GetValueAsBoolean() == B_FALSE || right->GetValueAsBoolean() == B_FALSE) {
        return new Value(B_FALSE);
    }
    if (left->GetValueAsBoolean() == B_UNKNOWN || right->GetValueAsBoolean() == B_UNKNOWN) {
        return new Value(B_UNKNOWN);
    }
    return new Value(B_TRUE);
}

Value *test_like(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    if (left->GetType() != Value::V_STRING || right->GetType() != Value::V_STRING) {
        return new Value(B_UNKNOWN);
    }
    std::regex pattern = to_regex(right->GetValueAsStr());
    bool matched = std::regex_match(left->GetValueAsStr(), pattern);
    if (op == AstExpr::LIKE) {
        if (matched) return new Value(B_TRUE);
        else return new Value(B_FALSE);
    } else {
        if (matched) return new Value(B_FALSE);
        else return new Value(B_TRUE);
    }
}

BOOLEAN eval_expression(const std::vector<Instruction*>& instructions, Subject *subject, const std::string& action, Resource *res, Host *host, App *app) {
    if (instructions.size() == 0 ) return B_UNKNOWN;
    if (subject->size() == 0 && action.length() == 0 && res->size() == 0 && host->size() == 0 && app->size() == 0) {
        return B_UNKNOWN;
    }
    std::stack<Value*> stk;
    for (size_t i = 0; i < instructions.size();) {
        Instruction *it = instructions[i];
        switch (it->_type) {
            case Instruction::LAB: { ++i; } break;
            case Instruction::PUSH_VAR: {
                if (it->u._var._var_type == AstColumnRef::RES) {
                    stk.push(new Value(res->GetValueAsString(it->u._var._var_name)));
                } else if (it->u._var._var_type == AstColumnRef::HOST) {
                    stk.push(new Value(host->GetValueAsString(it->u._var._var_name)));
                } else if (it->u._var._var_type == AstColumnRef::APP) {
                    stk.push(new Value(app->GetValueAsString(it->u._var._var_name)));
                } else if (it->u._var._var_type == AstColumnRef::ACTION) {
                    stk.push(make_action(action.c_str()));
                } else if (it->u._var._var_type == AstColumnRef::SUB) {
                    stk.push(new Value(subject->GetValueAsString(it->u._var._var_name)));
                } else {
                    assert(false);
                }
                ++i;
            } break;
            case Instruction::PUSH_CON: {
                stk.push(new Value(*(it->u._c)));
                ++i;
            } break;
            case Instruction::COND_JUMP: {
                Value *top = stk.top();
                assert(top->GetType() == Value::V_BOOLEAN);
                if (top->GetValueAsBoolean() == it->u._cjump._when) {
                    i = it->u._cjump._where_line;
                } else {
                    ++i;
                }
            } break;
            case Instruction::EXEC_OP: {
                if (it->u._op == AstExpr::NOT) {
                    assert(stk.size() >= 1);
                    Value *top = stk.top(); stk.pop();
                    if (top->GetType() != Value::V_BOOLEAN || top->GetValueAsBoolean() == B_UNKNOWN) {
                        stk.push(new Value(B_UNKNOWN));
                    } else {
                        stk.push(top->GetValueAsBoolean() == B_TRUE ? new Value(B_FALSE) : new Value(B_TRUE));
                    }
                    delete (top);
                    ++i;
                } else {
                    assert(stk.size() >= 2);
                    Value *right = stk.top(); stk.pop();
                    Value *left = stk.top(); stk.pop();
                    switch (it->u._op) {
                        case AstExpr::COMP_LT: /* go through */
                        case AstExpr::COMP_LE: /* go through */
                        case AstExpr::COMP_GT: /* go through */
                        case AstExpr::COMP_GE: {
                            Value *r = test_int(left, right, it->u._op);
                            stk.push(r);
                            delete (left); left = nullptr;
                            delete (right); right = nullptr;
                            ++i;
                        } break;
                        case AstExpr::COMP_EQ: /* go through */
                        case AstExpr::COMP_NEQ: {
                            Value *r = test_int_str(left, right, it->u._op);
                            stk.push(r);
                            delete (left); left = nullptr;
                            delete (right); right = nullptr;
                            ++i;
                        } break;
                        case AstExpr::OR: {
                            Value *r = test_or(left, right);
                            stk.push(r);
                            delete (left); left = nullptr;
                            delete (right); right = nullptr;
                            ++i;
                        } break;
                        case AstExpr::AND: {
                            Value *r = test_and(left, right);
                            stk.push(r);
                            delete (left); left = nullptr;
                            delete (right); right = nullptr;
                            ++i;
                        } break;
                        case AstExpr::LIKE: /* go through */
                        case AstExpr::NOT_LIKE: {
                            Value *r = test_like(left, right, it->u._op);
                            stk.push(r);
                            delete (left); left = nullptr;
                            delete (right); right = nullptr;
                            ++i;
                        } break;
                        default: {
                            assert(false);
                        }
                    }
                }
            } break;
            default: {
                assert(false);
            }
        }
    }
    assert(stk.size() == 1);
    Value *r = stk.top(); stk.pop();
    if (r->GetType() == Value::V_BOOLEAN) {
        BOOLEAN bl = r->GetValueAsBoolean();
        delete (r);
        return bl;
    } else {
        return B_UNKNOWN;
    }
}
