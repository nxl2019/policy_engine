#ifndef VALUE_H
#define VALUE_H

#include <string.h>
#include <stdlib.h>
#include <string>

enum BOOLEAN { B_TRUE, B_FALSE, B_UNKNOWN };

class Value {
public:
    enum VALUE_TYPE { V_INT, V_STRING, V_BOOLEAN, V_NULL };
    explicit Value(int data) : _value_type(V_INT), _can_cvt_to_int(false) { u._int_data = data; }
    explicit Value(const char *data) : _value_type(V_STRING), _can_cvt_to_int(false) {
        _str_data = data;
        char *end = nullptr;
        auto i = strtol(_str_data.c_str(), &end, 10);
        if (*end == '\0') {
            _can_cvt_to_int = true;
            u._int_data = (int)i;
        }
    }
    explicit Value(BOOLEAN boolean) : _value_type(V_BOOLEAN), _can_cvt_to_int(false) { u._boolean = boolean; }
    Value() : _value_type(V_NULL), _can_cvt_to_int(false) {}
    VALUE_TYPE GetType() { return _value_type; }
    BOOLEAN GetValueAsBoolean() { return u._boolean; }
    int GetValueAsInt() { return u._int_data; }
    const std::string& GetValueAsStr() { return _str_data; }
    bool CanConvertToInt() {
        return _can_cvt_to_int;
    }
private:

private:
    VALUE_TYPE _value_type;
    union {
        int _int_data;
        BOOLEAN _boolean;
    } u;
    std::string _str_data;
    bool _can_cvt_to_int;
};


#endif