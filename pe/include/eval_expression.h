#ifndef EVAL_EXPRESSION_H
#define EVAL_EXPRESSION_H

#include <vector>
#include "Value.h"
#include "Handle.h"

class AstExpr;
class Instruction;

Value::BOOLEAN eval_expression(AstExpr *expr, Subject *subject, const std::string& action, Resource *res, Host *host, App *app);

Value::BOOLEAN eval_expression(const std::vector<Instruction*>& instructions, Subject *subject, const std::string& action, Resource *res, Host *host, App *app);


#endif