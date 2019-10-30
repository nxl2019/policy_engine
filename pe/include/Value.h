#ifndef VALUE_H
#define VALUE_H

#include <string.h>
#include <stdlib.h>
#include <string>

class Value {
public:
    enum BOOLEAN { B_TRUE, B_FALSE, B_UNKNOWN };
    enum VALUE_TYPE { V_INT, V_STRING, V_BOOLEAN, V_NULL };
    explicit Value(int data) : _value_type(V_INT) { u._int_data = data; }
    explicit Value(const char *data) : _value_type(V_STRING) {
        _str_data = data;
    }
    explicit Value(BOOLEAN boolean) : _value_type(V_BOOLEAN) { u._boolean = boolean; }
    Value() : _value_type(V_NULL) {}
    VALUE_TYPE GetType() { return _value_type; }
    BOOLEAN GetValueAsBoolean() { return u._boolean; }
    int GetValueAsInt() { return u._int_data; }
    const std::string& GetValueAsStr() { return _str_data; }
private:

private:
    VALUE_TYPE _value_type;
    union {
        int _int_data;
        BOOLEAN _boolean;
    } u;
    std::string _str_data;
};


#endif