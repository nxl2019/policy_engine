#ifndef VALUE_H
#define VALUE_H

#include <string.h>

enum BOOLEAN { B_TRUE, B_FALSE, B_UNKNOWN };

class Value {
public:
    enum VALUE_TYPE { V_INT, V_STRING, V_BOOLEAN, V_NULL };
    Value(int data) : _value_type(V_INT) { u._int_data = data; }
    Value(const char *data) : _value_type(V_STRING) { u._str_data = strdup(data); }
    Value(BOOLEAN boolean) : _value_type(V_BOOLEAN) { u._boolean = boolean; }
    Value() : _value_type(V_NULL) {}
    ~Value() {
        if (_value_type == V_STRING) {
            free(u._str_data);
            u._str_data = nullptr;
        }
    }
private:
    VALUE_TYPE _value_type;
    union {
        int _int_data;
        char * _str_data;
        BOOLEAN _boolean;
    } u;
};


#endif