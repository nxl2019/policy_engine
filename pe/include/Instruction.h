#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "policy_expression.h"
#include "Value.h"
#include <string.h>
#include <stdlib.h>
#include "tool.h"

struct Instruction {
    ~Instruction() {
        if (_type == PUSH_VAR) free((void*)u._var._var_name);
        else if (_type == PUSH_CON) delete (u._c);
        else {}
    }
    Instruction(const char *var_name, AstColumnRef::COL_TYPE var_type) : _type(PUSH_VAR) { u._var._var_name = strdup(var_name); u._var._var_type = var_type; }
    explicit Instruction(const char *constant) : _type(PUSH_CON) { u._c = new Value(constant); }
    explicit Instruction(Value::BOOLEAN boolean) : _type(PUSH_CON) { u._c = new Value(boolean); }
    explicit Instruction(AstExpr::EXPR_TYPE op) : _type(EXEC_OP) { u._op = op; }
    Instruction(Value::BOOLEAN when, int where) : _type(COND_JUMP) { u._cjump._when = when; u._cjump._where_lab = where; }
    explicit Instruction(int lab) : _type(LAB) { u._lab = lab; }
    enum { PUSH_VAR, PUSH_CON, EXEC_OP, COND_JUMP, LAB } _type;
    union {
        struct {
            const char *_var_name;
            AstColumnRef::COL_TYPE _var_type;
        } _var;
        Value *_c;
        AstExpr::EXPR_TYPE _op;
        struct {
            Value::BOOLEAN _when;
            int     _where_lab;
            int     _where_line;
        } _cjump;
        int _lab;
    } u;
};

#endif