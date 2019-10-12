#ifndef LEX_H
#define LEX_H

#include <string>

class Token {
public:
    enum TOKEN_TYPE {
        TK_END_P,
        TK_ERR,
        TK_AND,
        TK_OR,
        TK_DOT,
        TK_L_PAREN,
        TK_R_PAREN,
        TK_EQ,
        TK_NEQ,
        TK_LT,
        TK_LTEQ,
        TK_GT,
        TK_GTEQ,
        TK_ID,
        TK_STR_LITERAL,
        TK_DEC_NUMBER,
        TK_HOST,
        TK_USER,
        TK_APP,
        TK_RES,
        /* todo extension */
    };
    Token();
    Token(TOKEN_TYPE token_type, const std::string& word);
    TOKEN_TYPE GetType() const;
    const std::string& GetWord() const;
    void Set(TOKEN_TYPE token_type, const std::string& word);
private:
    TOKEN_TYPE      _token_type;
    std::string     _word;
};

class Lex {
public:
    explicit Lex(const std::string& condition);
    const Token *GetCurrent() const;
    void Next();
private:
    char CharAt(unsigned pos);
    std::string Sub(unsigned start, unsigned end);
    unsigned PosInc(unsigned inc);
    unsigned Pos() const;
    void ScanfNumber();
    void ScanfIdentifier();
    void ScanfStrLiteral();
    void ScanfOperator();
    void ScanfWhite();
private:
    Token _cur_tk;
    std::string _condition;
    unsigned _pos;
};

#endif