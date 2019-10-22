//
// Created by jeff on 2019/10/9.
//

#include "policy_expression.h"
#include <string.h>
#include <assert.h>
#include "tool.h"
//-------------AstExpr----------------
AstExpr::AstExpr(EXPR_TYPE expr_type):_expr_type(expr_type), _expr_parent(NULL){ }
AstExpr::EXPR_TYPE AstExpr::GetExprType(){ return  _expr_type; }
AstExpr* AstExpr::GetParent() { return _expr_parent; }
void AstExpr::SetParent(AstExpr *parent) { assert( _expr_parent == NULL && parent != NULL ); _expr_parent = parent; }

//-------------AstBinaryOpExpr-------------
AstBinaryOpExpr::AstBinaryOpExpr(EXPR_TYPE expr_type, AstExpr *left, AstExpr *right):AstExpr(expr_type), _left(left), _right(right){
    _left->SetParent(this);
    _right->SetParent(this);
}
AstBinaryOpExpr::~AstBinaryOpExpr(){
    if (_left)  delete _left;
    _left = nullptr;
    if (_right) delete _right;
    _right = nullptr;
}
AstExpr* AstBinaryOpExpr::GetLeft() { return _left; }
AstExpr* AstBinaryOpExpr::GetRight() { return _right; }


//-------------AstConstantValue---------------
AstConstantValue::AstConstantValue(EXPR_TYPE expr_type):AstExpr(expr_type){ }
AstConstantValue::~AstConstantValue(){
    if (GetExprType() == C_STRING || GetExprType() == C_NUMBER || GetExprType() == C_PATTERN) {
        free (u._other_data);
        u._other_data = nullptr;
    }
}
void AstConstantValue::SetValue(int data){    u._int_data = data; }
void AstConstantValue::SetValue(const std::string& value){    u._other_data = strdup(value.c_str()); }
int  AstConstantValue::GetValueAsInt(){ return atoi(u._other_data); }

///-------------AstColumnRef-----------------------------------
AstColumnRef::AstColumnRef(COL_TYPE col_type, const AstIds& ids):AstExpr(EXPR_COLUMN_REF), _col_type(col_type), _ids(ids){}
AstColumnRef::~AstColumnRef(){
    for (auto ids:_ids) {
        if (ids) delete ids;
    }
}
AstColumnRef::COL_TYPE AstColumnRef::GetColType() { return _col_type; }
void    AstColumnRef::SetColumn(const AstIds& ids){    _ids = ids; }
const AstIds&   AstColumnRef::GetColumn(){    return  _ids; }

///-------------AstUnaryOpExpr----------------------------------------
AstUnaryOpExpr::AstUnaryOpExpr(EXPR_TYPE expr_type, AstExpr *expr): AstExpr(expr_type), _expr(expr){
    _expr->SetParent(this);
}
AstUnaryOpExpr::~AstUnaryOpExpr(){ if (_expr) delete _expr;}
AstExpr    *AstUnaryOpExpr::GetExpr(){ return _expr; }