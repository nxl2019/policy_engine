#include "PolicyModelList.h"
#include "NXLHttpClient.h"
#include "TalkWithCC.h"

PolicyModel::PM_TYPE PolicyModelList::GetPMTypeByID(uint64_t pmid) {
    PolicyModel pm1;
    if (!CheckExist(pmid, pm1)) {
        PolicyModel pm;
        bool r = AddPmByID(pmid, pm);
        if (!r) {
            return PolicyModel::PM_ERR;
        }
        return pm._type;
    } else return pm1._type;
}

AttributeInfo::ATTR_TYPE PolicyModelList::GetAttrTypeByPmidAttrName(uint64_t pmid, const std::string& attr_name) {
    PolicyModel pm1;
    if (!CheckExist(pmid, pm1)) {
        PolicyModel pm;
        bool r = AddPmByID(pmid, pm);
        if (!r) {
            return AttributeInfo::A_ERR;
        }
        return pm.GetTypeByName(attr_name);
    } else return pm1.GetTypeByName(attr_name);
}

AttributeInfo::ATTR_TYPE PolicyModelList::GetAttrTypeByPmnameAttrName(const std::string& pm_name, const std::string& attr_name) {
    PolicyModel pm1;
    if (!CheckExist(pm_name, pm1)) {
        PolicyModel pm;
        bool r = AddPmByName(pm_name, pm);
        if (!r) {
            return AttributeInfo::A_ERR;
        }
        return pm.GetTypeByName(attr_name);
    } else return pm1.GetTypeByName(attr_name);
}

bool PolicyModelList::CheckExist(uint64_t pmid, PolicyModel& out) {
    for (auto it : _models) {
        if (it._id == pmid) {
            out = it;
            return true;
        }
    }
    return false;
}

bool PolicyModelList::CheckExist(const std::string& pm_name, PolicyModel& out) {
    for (auto it : _models) {
        if (it._name == pm_name) {
            out = it;
            return true;
        }
    }
    return false;
}

bool PolicyModelList::AddPmByID(uint64_t pmid, PolicyModel& out) {
    std::string value;
    bool r = _talk->SearchPolicyModelByID(std::to_string(pmid), value);
    if (!r) return false;
    PolicyModel pm;
    pm.ParseFromJson(value);
    _models.push_back(pm);
    return true;
}

bool PolicyModelList::AddPmByName(const std::string& name, PolicyModel& out) {
    std::string value;
    bool r = _talk->SearchPolicyModelByName(name, value);
    if (!r) return false;
    PolicyModel pm;
    pm.ParseFromJson(value);
    _models.push_back(pm);
    return true;
}

