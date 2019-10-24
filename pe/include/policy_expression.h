//
// Created by jeff on 2019/10/9.
//

#ifndef POLICY_EXPRESSION_H
#define POLICY_EXPRESSION_H

#include <string>
#include <vector>


class AstExpr  {
public:
    enum EXPR_TYPE {
        /* BINARY OPERATOR */
                OR, AND, COMP_LE, COMP_LT, COMP_GE, COMP_GT, COMP_EQ, COMP_NEQ,
                LIKE, NOT_LIKE, INCLUDES, EQUALS_UNORDERED,
        /* UNARY */
                NOT,
        /* CONSTANT */
                 C_TRUE, C_FALSE, C_UNKNOWN, C_NULL, C_NUMBER, C_STRING, C_PATTERN,
        /* EXPR_COLUMN_REF */
                EXPR_COLUMN_REF,
        /* EXPER_NOT_SUPPORT */
                EXPER_NOT_SUPPORT

    };
    AstExpr(EXPR_TYPE expr_type);
    EXPR_TYPE GetExprType();
    AstExpr * GetParent();
    virtual ~AstExpr() {}
    void SetParent(AstExpr *parent);
private:
    EXPR_TYPE _expr_type;
    AstExpr * _expr_parent;
};




class AstBinaryOpExpr : public AstExpr {
public:
    AstBinaryOpExpr(EXPR_TYPE expr_type, AstExpr *left, AstExpr *right);
    ~AstBinaryOpExpr();
    AstExpr    *GetLeft();
    AstExpr    *GetRight();
private:
    AstExpr   *_left;
    AstExpr   *_right;
};


class AstConstantValue : public AstExpr {
public:
    static bool ConvertToInt(const std::string& src, int& rint);
    AstConstantValue(EXPR_TYPE expr_type);
    ~AstConstantValue();
    void SetValue(int data);
    void SetValue(const std::string& value);
    int  GetValueAsInt();
    const char *GetValueAsStr() { return u._other_data; }
private:
    union {
        int _int_data;
        char *_other_data;
    } u;
};

class AstId {
public:
    AstId(const std::string& id) : _id(id) {}
    void SetId(const std::string& id) { _id = id; }
    const std::string& GetId() { return _id; }
private:
    std::string     _id;
};

typedef std::vector<AstId*>                 AstIds;

class AstColumnRef : public AstExpr {
public:
    enum COL_TYPE { RES, SUB, APP, HOST, ACTION, OTHER };
    AstColumnRef(COL_TYPE col_type, const AstIds& ids);
    ~AstColumnRef();
    COL_TYPE                    GetColType();
    void                        SetColumn(const AstIds& ids);
    const AstIds&               GetColumn();
private:
    COL_TYPE    _col_type;
    AstIds      _ids;
};



class AstUnaryOpExpr : public AstExpr {
public:
    AstUnaryOpExpr(EXPR_TYPE expr_type, AstExpr *expr);
    ~AstUnaryOpExpr();
    AstExpr    *GetExpr();
private:
    AstExpr     *_expr;
};



#endif //POLICY_EXPRESSION_H
