#include "eval_expression.h"
#include "policy_expression.h"
#include "Handle.h"
#include "Value.h"
#include "tool.h"
#include <assert.h>
#include <stack>


struct RunTimeVars {
    Value GetRuntimeValue(AstColumnRef::COL_TYPE col_type, AstColumnRef::VAL_TYPE val_type, const std::string& attr) {
        Dictionary *dic = nullptr;
        switch (col_type) {
            case AstColumnRef::RES : {
                return Value();        /* not support */
            } break;
            case AstColumnRef::HOST : {
                dic = _host;
            } break;
            case AstColumnRef::APP : { dic = _app; } break;
            case AstColumnRef::SUB : { dic = _subject; } break;
            case AstColumnRef::ACTION : {
                return *_action;
            } break;
            default: return Value();    /* not support */
        }

        switch (val_type) {
            case AstColumnRef::CC_NUMBER : {
                bool r = false;
                return dic->GetValueAsInt(attr, r);
            } break;
            case AstColumnRef::CC_STRING : {
                return dic->GetValueAsString(attr);
            } break;
            default: return Value();
        }
    }
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

Value eval(AstExpr *expr, RunTimeVars *run_time_vars, Value::VALUE_TYPE expect = Value::V_NULL);

Value test_compare(AstExpr *expr, RunTimeVars *run_time_vars);

Value test_or(AstBinaryOpExpr *or_expr, RunTimeVars *run_time_vars) {
    Value l = eval(or_expr->GetLeft(), run_time_vars);
    if (l.GetType() != Value::V_BOOLEAN) {
        return Value(Value::B_UNKNOWN);
    }
    if (l.GetValueAsBoolean() == Value::B_TRUE) {
        return Value(Value::B_TRUE);
    }
    Value r = eval(or_expr->GetRight(), run_time_vars);
    if (r.GetType() != Value::V_BOOLEAN) {
        return Value(Value::B_UNKNOWN);
    }
    if (r.GetValueAsBoolean() == Value::B_TRUE) {
        return Value(Value::B_TRUE);
    }
    if (l.GetValueAsBoolean() == Value::B_UNKNOWN || r.GetValueAsBoolean() == Value::B_UNKNOWN) {
        return Value(Value::B_UNKNOWN);
    }
    return r;   /* B_FALSE */
}

Value test_and(AstBinaryOpExpr *and_expr, RunTimeVars *run_time_vars) {
    Value l = eval(and_expr->GetLeft(), run_time_vars);
    if (l.GetType() != Value::V_BOOLEAN) {
        return Value(Value::B_UNKNOWN);
    }
    if (l.GetValueAsBoolean() == Value::B_FALSE) {
        return Value(Value::B_FALSE);
    }
    Value r = eval(and_expr->GetRight(), run_time_vars);
    if (r.GetType() != Value::V_BOOLEAN) {
        return Value(Value::B_UNKNOWN);
    }
    if (r.GetValueAsBoolean() == Value::B_FALSE) {
        return Value(Value::B_FALSE);
    }
    if (l.GetValueAsBoolean() == Value::B_UNKNOWN || r.GetValueAsBoolean() == Value::B_UNKNOWN) {
        return Value(Value::B_UNKNOWN);
    }
    return r; /* B_TRUE */
}

Value test_not(AstUnaryOpExpr *not_expr, RunTimeVars *run_time_vars) {
    Value v = eval(not_expr->GetExpr(), run_time_vars);
    if (v.GetType() != Value::V_BOOLEAN) {
        return Value(Value::B_UNKNOWN);
    }
    if (v.GetValueAsBoolean() == Value::B_TRUE) {
        return Value(Value::B_FALSE);
    } else if (v.GetValueAsBoolean() == Value::B_FALSE) {
        return Value(Value::B_TRUE);
    } else {
        return v;
    }
}

Value eval(AstExpr *expr, RunTimeVars *run_time_vars, Value::VALUE_TYPE expect) {
    switch (expr->GetExprType()) {
        case AstExpr::OR: {
            auto *or_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            return test_or(or_expr, run_time_vars);
        } break;
        case AstExpr::AND: {
            auto *and_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            return test_and(and_expr, run_time_vars);
        } break;
        case AstExpr::COMP_LE: /* go through */
        case AstExpr::COMP_LT: /* go through */
        case AstExpr::COMP_GE: /* go through */
        case AstExpr::COMP_GT: /* go through */
        case AstExpr::COMP_EQ: /* go through */
        case AstExpr::COMP_NEQ: {
            return test_compare(expr, run_time_vars);
        } break;
        case AstExpr::NOT: {
            auto *not_expr = dynamic_cast<AstUnaryOpExpr*>(expr);
            return test_not(not_expr, run_time_vars);
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            auto *ref = dynamic_cast<AstColumnRef*>(expr);
            return run_time_vars->GetRuntimeValue(ref->GetColType(), ref->GetValType(), ref->GetColumn().back()->GetId());
        } break;
        case AstExpr::C_TRUE: {
            return Value(Value::B_TRUE);
        } break;
        case AstExpr::C_FALSE: {
            return Value(Value::B_FALSE);
        } break;
        case AstExpr::C_UNKNOWN: {
            return Value(Value::B_UNKNOWN);
        } break;
        case AstExpr::C_NUMBER: {
            bool r;
            return Value(dynamic_cast<AstConstantValue*>(expr)->GetValueAsInt(r));
        } break;
        case AstExpr::C_PATTERN: /* go through */
        case AstExpr::C_STRING: {
            // in cc the number literal is like "102" not 102
            if (expect == Value::V_INT) {
                bool r = false;
                int i = dynamic_cast<AstConstantValue*>(expr)->GetValueAsInt(r);
                if (r) {
                    return Value(i);
                } else {
                    return Value();
                }
            } else {
                return Value(dynamic_cast<AstConstantValue*>(expr)->GetValueAsStr());
            }
        } break;
        case AstExpr::C_NULL: {
            return Value();
        } break;
        case AstExpr::LIKE: /* go through */
        case AstExpr::NOT_LIKE: {
            return Value(Value::B_UNKNOWN); /* not support */
        } break;
        case AstExpr::INCLUDES: {
            return Value(Value::B_UNKNOWN); /* not support */
        } break;
        default: {  }
    }
    return Value(Value::B_UNKNOWN);
}

Value bool_to_value(bool b) {
    if (b) {
        return Value(Value::B_TRUE);
    } else {
        return Value(Value::B_FALSE);
    }
}

Value test_compare(AstExpr *expr, RunTimeVars *run_time_vars) {
    auto op = expr->GetExprType();
    auto comp = dynamic_cast<AstBinaryOpExpr*>(expr);

    AstExpr *left = comp->GetLeft(), *right = comp->GetRight();

    Value::VALUE_TYPE expect = Value::V_NULL;
    if (left->GetExprType() == AstExpr::EXPR_COLUMN_REF && right->GetExprType() == AstExpr::EXPR_COLUMN_REF) {
        // check type
        if (dynamic_cast<AstColumnRef*>(left)->GetValType() != dynamic_cast<AstColumnRef*>(right)->GetValType()) {
            return Value(Value::B_UNKNOWN);
        } else if (dynamic_cast<AstColumnRef*>(left)->GetValType() == AstColumnRef::CC_NUMBER) {
            expect = Value::V_INT;
        } else if (dynamic_cast<AstColumnRef*>(left)->GetValType() == AstColumnRef::CC_STRING) {
            expect = Value::V_STRING;
        } else {
            return Value(Value::B_UNKNOWN);
        }
    } else if (left->GetExprType() == AstExpr::EXPR_COLUMN_REF) {
        if (dynamic_cast<AstColumnRef*>(left)->GetValType() == AstColumnRef::CC_NUMBER) {
            expect = Value::V_INT;
        } else if (dynamic_cast<AstColumnRef*>(left)->GetValType() == AstColumnRef::CC_STRING) {
            expect = Value::V_STRING;
        } else {
            return Value(Value::B_UNKNOWN);
        }
    } else if (right->GetExprType() == AstExpr::EXPR_COLUMN_REF) {
        if (dynamic_cast<AstColumnRef*>(right)->GetValType() == AstColumnRef::CC_NUMBER) {
            expect = Value::V_INT;
        } else if (dynamic_cast<AstColumnRef*>(right)->GetValType() == AstColumnRef::CC_STRING) {
            expect = Value::V_STRING;
        } else {
            return Value(Value::B_UNKNOWN);
        }
    } else {
        //  left : <string literal> | <dec number literal> | NULL
        //  right: <string literal> | <dec number literal> | NULL
        if (left->GetExprType() == right->GetExprType()) {
            if (left->GetExprType() == AstExpr::C_NUMBER) {
                expect = Value::V_INT;
            } else if (left->GetExprType() == AstExpr::C_STRING) {
                expect = Value::V_STRING;
            } else {
                return Value(Value::B_UNKNOWN);
            }
        } else {
            return Value(Value::B_UNKNOWN);
        }
    }

    Value l = eval(left, run_time_vars, expect);
    Value r = eval(right, run_time_vars, expect);

    if (l.GetType() != r.GetType()) {
        return Value(Value::B_UNKNOWN);
    }

    if (l.GetType() != Value::V_INT && l.GetType() != Value::V_STRING) {
        return Value(Value::B_UNKNOWN);
    }

    switch (op) {
        case AstExpr::COMP_LE: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() <= r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) <= 0);
            }
        } break;
        case AstExpr::COMP_LT: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() < r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) < 0);
            }
        } break;
        case AstExpr::COMP_GE: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() >= r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) >= 0);
            }
        } break;
        case AstExpr::COMP_GT: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() > r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) >= 0);
            }
        } break;
        case AstExpr::COMP_EQ: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() == r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) == 0);
            }
        } break;
        case AstExpr::COMP_NEQ: {
            if (expect == Value::V_INT) {
                return bool_to_value(l.GetValueAsInt() != r.GetValueAsInt());
            } else {
                return bool_to_value(CommonFun::StrCaseCmp(l.GetValueAsStr().c_str(), r.GetValueAsStr().c_str()) != 0);
            }
        } break;
        default: assert(false);
    }
    return Value(Value::B_UNKNOWN);
}

Value::BOOLEAN eval_expression(AstExpr *expr, Subject *subject, const std::string& action, Resource *res, Host *host, App *app) {
    if (expr == nullptr) {
        return Value::B_UNKNOWN;
    }
    if (subject->size() == 0 && action.length() == 0 && res->size() == 0 && host->size() == 0 && app->size() == 0) {
        return Value::B_UNKNOWN;
    }
    Value *act = make_action(action.c_str());
    RunTimeVars run_time_vars{ subject, act, res, host, app };
    Value v = eval(expr, &run_time_vars);
    delete (act);
    if (v.GetType() == Value::V_BOOLEAN) {
        return v.GetValueAsBoolean();
    } else {
        return Value::B_UNKNOWN;
    }
}

#if 0
Value *test_int(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    assert(op == AstExpr::COMP_LE || op == AstExpr::COMP_LT ||
           op == AstExpr::COMP_GE || op == AstExpr::COMP_GT);
    if ((left->GetType() != Value::V_INT && !left->CanConvertToInt() ) ||
            (right->GetType() != Value::V_INT && !right->CanConvertToInt())) {
        return new Value(Value::B_UNKNOWN);
    }
    if (op == AstExpr::COMP_LE) {
        if (left->GetValueAsInt() <= right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else if (op == AstExpr::COMP_LT) {
        if (left->GetValueAsInt() < right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else if (op == AstExpr::COMP_GE) {
        if (left->GetValueAsInt() >= right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else if (op == AstExpr::COMP_GT) {
        if (left->GetValueAsInt() > right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else {
        assert(false);
    }
    return new Value(Value::B_UNKNOWN);
}

Value *test_int_str(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    assert(op == AstExpr::COMP_EQ || op == AstExpr::COMP_NEQ);
    if ((left->GetType() != Value::V_INT && !left->CanConvertToInt() ) ||
        (right->GetType() != Value::V_INT && !right->CanConvertToInt())) {
        if (left->GetType() == Value::V_STRING && right->GetType() == Value::V_STRING) {
            if (op == AstExpr::COMP_EQ) {
                if (CommonFun::StrCaseCmp(left->GetValueAsStr().c_str(), right->GetValueAsStr().c_str()) == 0) return new Value(Value::B_TRUE);
                else return new Value(Value::B_FALSE);
            } else if (op == AstExpr::COMP_NEQ) {
                if (CommonFun::StrCaseCmp(left->GetValueAsStr().c_str(), right->GetValueAsStr().c_str()) == 0) return new Value(Value::B_FALSE);
                else return new Value(Value::B_TRUE);
            } else {
                assert(false);
            }
        }
        return new Value(Value::B_UNKNOWN);
    }
    if (op == AstExpr::COMP_EQ) {
        if (left->GetValueAsInt() == right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else if (op == AstExpr::COMP_NEQ) {
        if (left->GetValueAsInt() != right->GetValueAsInt()) {
            return new Value(Value::B_TRUE);
        }
        else {
            return new Value(Value::B_FALSE);
        }
    } else {
        assert(false);
    }
    return new Value(Value::B_UNKNOWN);
}

Value *test_or(Value *left, Value *right) {
    if (left->GetType() != Value::V_BOOLEAN || right->GetType() != Value::V_BOOLEAN) {
        return new Value(Value::B_UNKNOWN);
    }
    if (left->GetValueAsBoolean() == Value::B_TRUE || right->GetValueAsBoolean() == Value::B_TRUE) {
        return new Value(Value::B_TRUE);
    }
    if (left->GetValueAsBoolean() == Value::B_UNKNOWN || right->GetValueAsBoolean() == Value::B_UNKNOWN) {
        return new Value(Value::B_UNKNOWN);
    }
    return new Value(Value::B_FALSE);
}

Value *test_and(Value *left, Value *right) {
    if (left->GetType() != Value::V_BOOLEAN || right->GetType() != Value::V_BOOLEAN) {
        return new Value(Value::B_UNKNOWN);
    }
    if (left->GetValueAsBoolean() == Value::B_FALSE || right->GetValueAsBoolean() == Value::B_FALSE) {
        return new Value(Value::B_FALSE);
    }
    if (left->GetValueAsBoolean() == Value::B_UNKNOWN || right->GetValueAsBoolean() == Value::B_UNKNOWN) {
        return new Value(Value::B_UNKNOWN);
    }
    return new Value(Value::B_TRUE);
}

Value *test_like(Value *left, Value *right, AstExpr::EXPR_TYPE op) {
    if (left->GetType() != Value::V_STRING || right->GetType() != Value::V_STRING) {
        return new Value(Value::B_UNKNOWN);
    }
    std::regex pattern = to_regex(right->GetValueAsStr());
    bool matched = std::regex_match(left->GetValueAsStr(), pattern);
    if (op == AstExpr::LIKE) {
        if (matched) return new Value(Value::B_TRUE);
        else return new Value(Value::B_FALSE);
    } else {
        if (matched) return new Value(Value::B_FALSE);
        else return new Value(Value::B_TRUE);
    }
}

Value::BOOLEAN eval_expression(const std::vector<Instruction*>& instructions, Subject *subject, const std::string& action, Resource *res, Host *host, App *app) {
    if (instructions.size() == 0 ) return Value::B_UNKNOWN;
    if (subject->size() == 0 && action.length() == 0 && res->size() == 0 && host->size() == 0 && app->size() == 0) {
        return Value::B_UNKNOWN;
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
                    if (top->GetType() != Value::V_BOOLEAN || top->GetValueAsBoolean() == Value::B_UNKNOWN) {
                        stk.push(new Value(Value::B_UNKNOWN));
                    } else {
                        stk.push(top->GetValueAsBoolean() == Value::B_TRUE ? new Value(Value::B_FALSE) : new Value(Value::B_TRUE));
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
        Value::BOOLEAN bl = r->GetValueAsBoolean();
        delete (r);
        return bl;
    } else {
        return Value::B_UNKNOWN;
    }
}
#endif