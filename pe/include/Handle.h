#ifndef HANDLE_H
#define HANDLE_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include "Value.h"
#include "policy_engine.h"
#include "tool.h"


class Handle {
public:
    virtual ~Handle() {}
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const = 0;
};

class StringList : public Handle {
public:
    static StringList *MakeStringList(const std::set<std::string>& strings) {
        StringList *list = nullptr;
        for (auto it : strings) {
            list = new StringList(it, list);
        }
        return list;
    }
    static void FreeStringList(StringList *list) {
        for (StringList *it = list; it != nullptr; ) {
            StringList *tmp = it->_next;
            delete (it);
            it = tmp;
        }
    }
public:
    StringList(const std::string& cur, StringList *next) : _current(cur), _next(next) {}
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const override { return PE_STRING_LIST; }
    const std::string& GetCurrent() { return _current; }
    StringList *GetNext() { return _next; }
private:
    std::string _current;
    StringList *_next;
};

class Dictionary : public Handle {
public:
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const override { return _dictionary_type; }
    Dictionary(POLICY_ENGINE_HANDLE_TYPE tp) : _dictionary_type(tp) {}
    Value GetValueAsInt(const std::string& key, bool& r) {
        auto fd = _data.find(key);
        if (fd == _data.end()) return Value();
        else {
            char *end = nullptr;
            int i = (int)strtol(fd->second.c_str(), &end, 10);
            r = *end == '\0';
            return Value(i);
        }
    }
    Value GetValueAsString(const std::string& key) {
        auto fd = _data.find(key);
        if (fd == _data.end()) return Value();
        else return Value(fd->second.c_str());
    }
    void InsertValue(const std::string& key, const std::string& value) {
        _data[key] = value;
    }
    size_t size() { return _data.size(); }
private:
    POLICY_ENGINE_HANDLE_TYPE                                    _dictionary_type;
    std::map<std::string, std::string, CommonFun::IgnoreCaseCmp> _data;
};

typedef Dictionary Subject;
typedef Dictionary Resource;
typedef Dictionary Host;
typedef Dictionary App;

#endif