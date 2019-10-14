#ifndef EVAL_EXPRESSION_H
#define EVAL_EXPRESSION_H

#include <vector>
#include "Value.h"
#include "Handle.h"

class AstExpr;
class Instruction;

BOOLEAN eval_expression(AstExpr *expr, Subject *subject, const char *action);

BOOLEAN eval_expression(const std::vector<Instruction*>& instructions, Subject *subject, const char *action);


#endif