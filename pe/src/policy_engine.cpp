#include "policy_engine.h"
#include "PolicyEngine.h"
#include "Handle.h"

PolicyEngineReturn policy_engine_module_init(const char *cchost, const char *ccport, const char *ccuser, const char *ccpwd, const char *tag,
                                             unsigned int sync_interval_seconds) {
    return PolicyEngine::Ins()->Init(cchost, ccport, ccuser, ccpwd, tag, sync_interval_seconds);
}

PolicyEngineReturn policy_engine_module_exit() {
    return PolicyEngine::Ins()->Exit();
}

PolicyEngineReturn policy_engine_subjects_actions_analyze(PolicyEngineStringList *psubjects_string_list, PolicyEngineStringList *pactions_string_list) {
    StringList *subjects_string_list = nullptr, *actions_string_list = nullptr;
    PolicyEngineReturn r = POLICY_ENGINE_SUCCESS;
    r = PolicyEngine::Ins()->Analyze(&subjects_string_list, &actions_string_list);
    if (r != POLICY_ENGINE_SUCCESS) return r;
    *psubjects_string_list = subjects_string_list;
    *pactions_string_list = actions_string_list;
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_destroy_string_list(PolicyEngineStringList pstring_list) {
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    StringList::FreeStringList(dynamic_cast<StringList*>(handle));
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_string_list_current(PolicyEngineStringList pstring_list, const char **string) {
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    *string = dynamic_cast<StringList*>(handle)->GetCurrent().c_str();
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_string_list_next(PolicyEngineStringList pstring_list, PolicyEngineStringList *next) {
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    *next = dynamic_cast<StringList*>(handle)->GetNext();
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_create_subject_handle(PolicyEngineSubject *psubject) {
    *psubject = new Subject;
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_destroy_subject(PolicyEngineSubject subject) {
    Handle *handle = (Handle*)subject;
    if (handle->GetHandleType() != PE_SUBJECT) return POLICY_ENGINE_TYPE_ERROR;
    delete (handle);
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_insert_into_subject(PolicyEngineSubject subject, const char *attribute_name, const char *attribute_value) {
    Handle *handle = (Handle*)subject;
    if (handle->GetHandleType() != PE_SUBJECT) return POLICY_ENGINE_TYPE_ERROR;
    dynamic_cast<Subject*>(handle)->InsertValue(attribute_name, attribute_value);
    return POLICY_ENGINE_SUCCESS;
}


PolicyEngineReturn policy_engine_match( PolicyEngineSubject subject, const char *action, PolicyEngineResource resource, POLICY_ENGINE_MATCH_RESULT *presult) {
    Handle *handle = (Handle*)subject;
    if (handle->GetHandleType() != PE_SUBJECT) return POLICY_ENGINE_TYPE_ERROR;
    if (resource != nullptr) return POLICY_ENGINE_FAIL;
    return PolicyEngine::Ins()->Match(dynamic_cast<Subject*>(handle), action, presult);
}

