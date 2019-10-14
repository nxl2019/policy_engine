#include "translate.h"
#include "policy_expression.h"
#include <assert.h>
#include "Instruction.h"
#include <map>
#include <Instruction.h>

#define make_new_label (_lab++)

void translate(AstExpr *expr, std::vector<Instruction*>& rinstructions, unsigned& _lab);
void proc_lab(const std::vector<Instruction*>& instructions);

void gen_code(AstExpr *expr, std::vector<Instruction*>& rinstructions) {
    rinstructions.clear();
    unsigned _lab = 1;
    translate(expr, rinstructions, _lab);
    proc_lab(rinstructions);
}

void free_code(std::vector<Instruction*>& rinstructions) {
    for (auto it : rinstructions) {
        delete (it);
    }
    rinstructions.clear();
}

void translate(AstExpr *expr, std::vector<Instruction*>& rinstructions, unsigned& _lab) {
    if (expr == nullptr) {
        return;
    }
    switch (expr->GetExprType()) {
        case AstExpr::OR: {
            AstBinaryOpExpr *or_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            translate(or_expr->GetLeft(), rinstructions, _lab);
            unsigned lab1 = make_new_label;
            Instruction *cjump = new Instruction(B_TRUE, lab1);
            rinstructions.push_back(cjump);
            translate(or_expr->GetRight(), rinstructions, _lab);
            rinstructions.push_back(new Instruction(AstExpr::OR));
            rinstructions.push_back(new Instruction(lab1));
        } break;
        case AstExpr::AND: {
            AstBinaryOpExpr *and_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            translate(and_expr->GetLeft(), rinstructions, _lab);
            unsigned lab1 = make_new_label;
            Instruction *cjump = new Instruction(B_FALSE, lab1);
            rinstructions.push_back(cjump);
            translate(and_expr->GetRight(), rinstructions, _lab);
            rinstructions.push_back(new Instruction(AstExpr::AND));
            rinstructions.push_back(new Instruction(lab1));
        } break;
        case AstExpr::COMP_GT: /* go through */
        case AstExpr::COMP_GE: /* go through */
        case AstExpr::COMP_LT: /* go through */
        case AstExpr::COMP_LE: /* go through */
        case AstExpr::COMP_EQ: /* go through */
        case AstExpr::LIKE: /* go through */
        case AstExpr::NOT_LIKE: /* go through */
        case AstExpr::COMP_NEQ: {
            AstBinaryOpExpr *binary_expr = dynamic_cast<AstBinaryOpExpr*>(expr);
            translate(binary_expr->GetLeft(), rinstructions, _lab);
            translate(binary_expr->GetRight(), rinstructions, _lab);
            rinstructions.push_back(new Instruction(expr->GetExprType()));
        } break;
        case AstExpr::NOT: {
            AstUnaryOpExpr *not_expr = dynamic_cast<AstUnaryOpExpr*>(expr);
            translate(not_expr->GetExpr(), rinstructions, _lab);
            rinstructions.push_back(new Instruction(expr->GetExprType()));
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            AstColumnRef *ref= dynamic_cast<AstColumnRef*>(expr);
            const AstIds& ids = ref->GetColumn();
            assert(ids.size() > 0);
            rinstructions.push_back(new Instruction(ids.back()->GetId().c_str(), ref->GetColType()));
        } break;
        case AstExpr::C_TRUE: {
            rinstructions.push_back(new Instruction(B_TRUE));
        } break;
        case AstExpr::C_FALSE: {
            rinstructions.push_back(new Instruction(B_FALSE));
        } break;
        case AstExpr::C_UNKNOWN: {
            rinstructions.push_back(new Instruction(B_UNKNOWN));
        } break;
        case AstExpr::C_NUMBER: {
            assert(false);
        } break;
        case AstExpr::C_PATTERN: /* go through */
        case AstExpr::C_STRING: {
            AstConstantValue *value = dynamic_cast<AstConstantValue*>(expr);
            rinstructions.push_back(new Instruction(value->GetValueAsStr()));
        } break;
        default: { assert(false); }
    }
}

void proc_lab(const std::vector<Instruction*>& instructions) {
    std::map<int, int> labs;    // labid, lineid
    for (int i = instructions.size()-1; i >= 0; --i) {
        if (instructions[i]->_type == Instruction::LAB) {
            assert(labs.find(instructions[i]->u._lab) == labs.end());
            labs[instructions[i]->u._lab] = i;
        }
        else if (instructions[i]->_type == Instruction::COND_JUMP) {
            auto fd = labs.find(instructions[i]->u._cjump._where_lab);
            assert(fd != labs.end());
            instructions[i]->u._cjump._where_line = fd->second;
        }
    }
}