#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <vector>

class AstExpr;
class Instruction;

void gen_code(AstExpr *expr, std::vector<Instruction*>& rinstructions);
void free_code(std::vector<Instruction*>& rinstructions);

#endif