#ifndef HANDLE_H
#define HANDLE_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Value.h"
#include "policy_engine.h"

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
            StringList *tmp = it;
            delete (tmp);
            it = it->_next;
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

class Subject : public Handle {
public:
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const override { return PE_SUBJECT; }
    Value *GetValueAsInt(const std::string& key) {
        auto fd = _data.find(key);
        if (fd == _data.end()) return new Value;
        else return new Value(atoi(fd->second.c_str()));
    }
    Value *GetValueAsString(const std::string& key) {
        auto fd = _data.find(key);
        if (fd == _data.end()) return new Value;
        else return new Value(fd->second.c_str());
    }
    void InsertValue(const std::string& key, const std::string& value) {
        _data[key] = value;
    }
private:
    std::map<std::string, std::string> _data;
};



#endif