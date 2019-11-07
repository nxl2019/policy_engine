#include "Lex.h"
#include <assert.h>
#include <map>
#include "tool.h"


#define EOI (-1)

const std::map<std::string, Token::TOKEN_TYPE, CommonFun::IgnoreCaseCmp > KEYWORDS {
        {"HOST",        Token::TK_HOST},    {"USER",       Token::TK_USER},
        {"APPLICATION", Token::TK_APP },    {"RESOURCE",   Token::TK_RES },
        {"OR",          Token::TK_OR  },    {"AND",        Token::TK_AND},
        {"NOT",         Token::TK_NOT},     {"NULL",       Token::TK_NULL},
        {"INCLUDES",    Token::TK_INCLUDES},{"EQUALS_UNORDERED", Token::TK_EQUALS_UNORDERED}
};

bool check_reserved_keyword(const std::string &word, Token& tk) {
    auto it = KEYWORDS.find(word);
    if (it != KEYWORDS.end()) {
        tk.Set(it->second, word);
        return true;
    }
    return false;
}

bool is_white(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == 'f';
}

bool is_identifier_begin(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

bool is_identifier_body(char c) {
    return is_identifier_begin(c) || ('0' <= c && c <= '9');
}

bool is_dec_body(char c) {
    return '0' <= c && c <= '9';
}

bool is_escaped(char c, char& escaped) {
    //return c == '\\' || c == '"' || c == 'r' || c == 'n';
    switch (c) {
        case '\\' : {
            escaped = '\\';
            return true;
        } break;
        case '"' : {
            escaped = '"';
            return true;
        } break;
        case 'r' : {
            escaped = '\r';
            return true;
        } break;
        case 'n' : {
            escaped = '\n';
            return true;
        } break;
        default: {
            return false;
        }
    }
}

Token::Token() : _token_type(TK_ERR) {}

Token::TOKEN_TYPE Token::GetType() const { return _token_type; }

const std::string& Token::GetWord() const { return _word; }

void Token::Set(TOKEN_TYPE token_type, const std::string& word) {
    _token_type = token_type;
    _word = word;
}

Lex::Lex(const std::string& condition) : _condition(condition), _pos(0) {}

const Token *Lex::GetCurrent() const { return &_cur_tk; }

void Lex::Next() {
    if (_cur_tk.GetType() == Token::TK_END_P) return;
    while (true) {
        switch (CharAt(Pos())) {
            case ' ':  /* go through */
            case '\t': /* go through */
            case '\n': /* go through */
            case '\r': /* go through */
            case '\f': {
                ScanfWhite();
            } break;
            case '0' : /* go through */
            case '1' : /* go through */
            case '2' : /* go through */
            case '3' : /* go through */
            case '4' : /* go through */
            case '5' : /* go through */
            case '6' : /* go through */
            case '7' : /* go through */
            case '8' : /* go through */
            case '9' : {
                return ScanfNumber();
            } break;
            case '[' : /* go through */
            case ']' : /* go through */
            case ',' : /* go through */
            case '.' : /* go through */
            case '(' : /* go through */
            case ')' : /* go through */
            case '=' : /* go through */
            case '!' : /* go through */
            case '>' : /* go through */
            case '<' : {
                return ScanfOperator();
            } break;
            case '"' : {
                return ScanfStrLiteral();
            } break;
            case EOI : {
                _cur_tk.Set(Token::TK_END_P, "");
                return;
            }
            default: {
                if (is_identifier_begin(CharAt(Pos()))) {
                    return ScanfIdentifier();
                } else {
                    /* Skip The Unknown Char */
                    PosInc(1);
                }
            }
        }
    }
}

char Lex::CharAt(unsigned pos) {
    if (pos < _condition.length()) return _condition[pos];
    else return EOI;
}

std::string Lex::Sub(unsigned start, unsigned end) {
    assert(end > start);
    return _condition.substr(start, end-start);
}

unsigned Lex::PosInc(unsigned inc) {
    _pos += inc;
    if (_pos > _condition.length()) _cur_tk.Set(Token::TK_END_P, "");
    return _pos;
}

unsigned Lex::Pos() const {
    return _pos;
}

void Lex::ScanfNumber() {
    unsigned start = Pos();
    char c = CharAt(start);
    assert(is_dec_body(c));
    while (is_dec_body(c)) c = CharAt(PosInc(1));
    _cur_tk.Set(Token::TK_DEC_NUMBER, Sub(start, Pos()));
}

void Lex::ScanfIdentifier() {
    unsigned start = Pos();
    char c = CharAt(start);
    assert(is_identifier_begin(c));
    PosInc(1);
    c = CharAt(Pos());
    while (is_identifier_body(c)) c = CharAt(PosInc(1));
    std::string s = Sub(start, Pos());
    if (!check_reserved_keyword(s, _cur_tk)) _cur_tk.Set(Token::TK_ID, s);
}

#if 0
void Lex::ScanfStrLiteral() {
    assert(CharAt(Pos()) == '"');
    PosInc(1);
    std::stringstream buf{};
    char c = CharAt(Pos());
    while (c != '"' && c != EOI) {
        if (c == '\\') {
            char next = CharAt(Pos()+1);
            char escaped = 0;
            if (is_escaped(next, escaped)) {
                buf << escaped;
                PosInc(1);
            } else {
                _cur_tk.Set(Token::TK_ERR, "Unexpected Escape Char");
                return;
            }
        } else {
            buf << c;
        }
        c = CharAt(PosInc(1));
    }
    if (c == '"') {
        PosInc(1);
        _cur_tk.Set(Token::TK_STR_LITERAL,buf.str());
    } else {
        _cur_tk.Set(Token::TK_ERR, "Unterminated String Literal");
    }
}
#else
void Lex::ScanfStrLiteral() {
    assert(CharAt(Pos()) == '"');
    PosInc(1);
    std::string buf;
    char c = CharAt(Pos());
    while (c != '"' && c != EOI) {
        if (c == '\\') {
            char next = CharAt(Pos()+1);
            char escaped = 0;
            if (is_escaped(next, escaped)) {
                buf += escaped;
                PosInc(1);
            } else {
                _cur_tk.Set(Token::TK_ERR, "Unexpected Escape Char");
                return;
            }
        } else {
            buf += c;
        }
        c = CharAt(PosInc(1));
    }
    if (c == '"') {
        PosInc(1);
        _cur_tk.Set(Token::TK_STR_LITERAL, buf);
    } else {
        _cur_tk.Set(Token::TK_ERR, "Unterminated String Literal");
    }
}
#endif

void Lex::ScanfOperator() {
    switch (CharAt(Pos())) {
        case '[' : { _cur_tk.Set(Token::TK_L_BRACKET, "["); PosInc(1); } break;
        case ']' : { _cur_tk.Set(Token::TK_R_BRACKET, "]"); PosInc(1); } break;
        case ',' : { _cur_tk.Set(Token::TK_COMMA, ","); PosInc(1); } break;
        case '.' : { _cur_tk.Set(Token::TK_DOT, "."); PosInc(1); } break;
        case '(' : { _cur_tk.Set(Token::TK_L_PAREN, "("); PosInc(1); } break;
        case ')' : { _cur_tk.Set(Token::TK_R_PAREN, ")"); PosInc(1); } break;
        case '=' : { _cur_tk.Set(Token::TK_EQ, "="); PosInc(1); } break;
        case '!' : {
            PosInc(1);
            if (CharAt(Pos()) == '=') {
                _cur_tk.Set(Token::TK_NEQ, "!="); PosInc(1);
            } else {
                _cur_tk.Set(Token::TK_ERR, "Expect =");
            }
        } break;
        case '>' : {
            PosInc(1);
            if (CharAt(Pos()) == '=') {
                _cur_tk.Set(Token::TK_GTEQ, ">="); PosInc(1);
            } else {
                _cur_tk.Set(Token::TK_GT, ">");
            }
        } break;
        case '<' : {
            PosInc(1);
            if (CharAt(Pos()) == '=') {
                _cur_tk.Set(Token::TK_LTEQ, "<="); PosInc(1);
            } else {
                _cur_tk.Set(Token::TK_LT, "<");
            }
        }
    }
}

void Lex::ScanfWhite() {
    char c = CharAt(Pos());
    while (is_white(c)) c = CharAt(PosInc(1));
}


////////////////////////////////////////////////
LexOb::LexOb(const std::string& oblication) : Lex(oblication) { }

void LexOb::Next() {
    if (_cur_tk.GetType() == Token::TK_END_P) return;
    while (true) {
        switch (CharAt(Pos())) {
            case ' ':  /* go through */
            case '\t': /* go through */
            case '\n': /* go through */
            case '\r': /* go through */
            case '\f': {
                ScanfWhite();
            } break;
            case '.' : { _cur_tk.Set(Token::TK_DOT, "."); PosInc(1); return; } break;
            case '$' : { _cur_tk.Set(Token::TK_DOLLAR, "$"); PosInc(1); return; } break;
            case '"' : {
                return ScanfStrLiteral();
            } break;
            case EOI : {
                _cur_tk.Set(Token::TK_END_P, "");
                return;
            }
            default: {
                if (is_identifier_begin(CharAt(Pos()))) {
                    return ScanfIdentifier();
                } else {
                    /* Skip The Unknown Char */
                    PosInc(1);
                }
            }
        }
    }
}





