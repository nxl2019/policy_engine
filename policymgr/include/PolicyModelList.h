#ifndef POLICY_MODEL_LIST
#define POLICY_MODEL_LIST

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "tool.h"

#define PM_USER_ID 1
#define PM_HOST_ID 2
#define PM_APPLICATION_ID 3

struct AttributeInfo {
    enum ATTR_TYPE { A_STRING, A_NUMBER, A_MULTI, A_ERR };
    std::string     _attribute;
    ATTR_TYPE       _type;
    /* todo for extension */
};

struct PolicyModel {
    enum PM_TYPE { PM_RES, PM_SUB_USER, PM_SUB_APP, PM_SUB_HOST, PM_ERR };
    PolicyModel() {}
    void ParseFromJson(const std::string& json);
    void AddPreAttribute(const std::string& json);

    AttributeInfo::ATTR_TYPE GetTypeByName(const std::string& name) {
        auto fd = _attributes.find(name);
        if (fd == _attributes.end()) return AttributeInfo::A_ERR;
        return fd->second._type;
    }
    uint64_t                                                          _id;
    std::string                                                       _name;
    PM_TYPE                                                           _type;
    std::map<std::string, AttributeInfo, CommonFun::IgnoreCaseCmp>    _attributes;
};

class TalkWithCC;

class PolicyModelList {
public:
    PolicyModelList() : _talk(nullptr) {}
    PolicyModelList(const std::vector<PolicyModel>& models, TalkWithCC *talk) : _models(models), _talk(talk) {}

    PolicyModel::PM_TYPE GetPMTypeByID(uint64_t pmid);
    PolicyModel::PM_TYPE GetPMTypeByPmname(const std::string & pm_name);
    AttributeInfo::ATTR_TYPE GetAttrTypeByPmidAttrName(uint64_t pmid, const std::string& attr_name);
    AttributeInfo::ATTR_TYPE GetAttrTypeByPmnameAttrName(const std::string& pm_name, const std::string& attr_name);
    TalkWithCC *GetTalk() { return _talk; }
    void ClearTalk() { _talk = nullptr; }
    bool AddPmByID(uint64_t pmid, PolicyModel& out);
protected:
    bool CheckExist(uint64_t pmid, PolicyModel& out);
    bool CheckExist(const std::string& pm_name, PolicyModel& out);
    bool AddPmByName(const std::string& name, PolicyModel& out);
private:
    std::map<std::string, uint64_t, CommonFun::IgnoreCaseCmp> _name2id;
    std::vector<PolicyModel>    _models;
    TalkWithCC *                _talk;
};

#endif