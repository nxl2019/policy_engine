#include "policy_engine.h"
#include "PolicyEngine.h"
#include "Handle.h"

PolicyEngineReturn policy_engine_module_init(const char *cchost, const char *ccport, const char *ccuser, const char *ccpwd, const char *tag,
                                             unsigned int sync_interval_seconds) {
    if (cchost == nullptr || ccport == nullptr || ccuser == nullptr || ccpwd == nullptr || tag == nullptr) return POLICY_ENGINE_FAIL;
    return PolicyEngine::Ins()->Init(cchost, ccport, ccuser, ccpwd, tag, sync_interval_seconds);
}

PolicyEngineReturn policy_engine_module_exit() {
    return PolicyEngine::Ins()->Exit();
}

PolicyEngineReturn policy_engine_analyze(PolicyEngineStringList *psubjects_string_list, PolicyEngineStringList *pactions_string_list,
                                                          PolicyEngineStringList *presource_string_list, PolicyEngineStringList *phost_string_list, PolicyEngineStringList *papp_string_list) {
    StringList *subjects_string_list = nullptr, *actions_string_list = nullptr, *resource_string_list = nullptr,
                *host_string_list = nullptr, *app_string_list = nullptr;

    PolicyEngineReturn r = POLICY_ENGINE_SUCCESS;
    r = PolicyEngine::Ins()->Analyze(&subjects_string_list, &actions_string_list, &resource_string_list, &host_string_list, &app_string_list);

    if (psubjects_string_list != nullptr ) {
        *psubjects_string_list = subjects_string_list;
    } else {
        delete(subjects_string_list);
    }
    if (pactions_string_list != nullptr ) {
        *pactions_string_list = actions_string_list;
    } else {
        delete(actions_string_list);
    }
    if (presource_string_list != nullptr ) {
        *presource_string_list = resource_string_list;
    } else {
        delete(resource_string_list);
    }
    if (phost_string_list != nullptr ) {
        *phost_string_list = host_string_list;
    } else {
        delete(host_string_list);
    }
    if (papp_string_list != nullptr ) {
        *papp_string_list = app_string_list;
    } else {
        delete(app_string_list);
    }

    return r;
}

PolicyEngineReturn policy_engine_destroy_string_list(PolicyEngineStringList pstring_list) {
    if (pstring_list == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    StringList::FreeStringList(dynamic_cast<StringList*>(handle));
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_string_list_current(PolicyEngineStringList pstring_list, const char **string) {
    if (pstring_list == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    if (string == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    *string = dynamic_cast<StringList*>(handle)->GetCurrent().c_str();
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_string_list_next(PolicyEngineStringList pstring_list, PolicyEngineStringList *next) {
    if (pstring_list == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    if (next == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    Handle *handle = (Handle*)pstring_list;
    if (handle->GetHandleType() != PE_STRING_LIST) return POLICY_ENGINE_TYPE_ERROR;
    *next = dynamic_cast<StringList*>(handle)->GetNext();
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE dictionary_type, PolicyEngineHandle *pdictionary) {
    if (pdictionary == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    switch (dictionary_type) {
        case PE_SUBJECT: {
            *pdictionary = new Dictionary(PE_SUBJECT);
            return POLICY_ENGINE_SUCCESS;
        } break;
        case PE_RESOURCE: {
            *pdictionary = new Dictionary(PE_RESOURCE);
            return POLICY_ENGINE_SUCCESS;
        } break;
        case PE_HOST: {
            *pdictionary = new Dictionary(PE_HOST);
            return POLICY_ENGINE_SUCCESS;
        } break;
        case PE_APPLICATION: {
            *pdictionary = new Dictionary(PE_APPLICATION);
            return POLICY_ENGINE_SUCCESS;
        } break;
        default: { return POLICY_ENGINE_FAIL; }
    }
}

PolicyEngineReturn policy_engine_destroy_dictionary(PolicyEngineHandle dictionary) {
    if (dictionary == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    Handle *handle = (Handle*)dictionary;
    switch (handle->GetHandleType()) {
        case PE_SUBJECT: /* go through */
        case PE_RESOURCE: /* go through */
        case PE_HOST: /* go through */
        case PE_APPLICATION: {
            delete (handle);
        } break;
        default: { return POLICY_ENGINE_FAIL; }
    }
    return POLICY_ENGINE_SUCCESS;
}

PolicyEngineReturn policy_engine_insert_into_dictionary(PolicyEngineHandle dictionary, const char *attribute_name, const char *attribute_value) {
    if (dictionary == nullptr) return POLICY_ENGINE_INVALID_HANDLE;
    Handle *handle = (Handle*)dictionary;
    if (attribute_name == nullptr || attribute_value == nullptr) return POLICY_ENGINE_FAIL;
    switch (handle->GetHandleType()) {
        case PE_SUBJECT: /* go through */
        case PE_RESOURCE: /* go through */
        case PE_HOST: /* go through */
        case PE_APPLICATION: {
            dynamic_cast<Dictionary*>(handle)->InsertValue(attribute_name, attribute_value);
        } break;
        default: { return POLICY_ENGINE_TYPE_ERROR; }
    }
    return POLICY_ENGINE_SUCCESS;
}


PolicyEngineReturn policy_engine_match( PolicyEngineSubject subject, const char *action, PolicyEngineResource resource,
                                        PolicyEngineHost host, PolicyEngineApplication application, POLICY_ENGINE_MATCH_RESULT *presult) {
    if (presult == nullptr) return POLICY_ENGINE_FAIL;

    Handle *handle_sub = (Handle*)subject;
    Dictionary h_sub(PE_SUBJECT), h_res(PE_RESOURCE), h_host(PE_HOST), h_app(PE_APPLICATION);
    if (handle_sub == nullptr) {
        handle_sub = &h_sub;
    }
    if (handle_sub->GetHandleType() != PE_SUBJECT) return POLICY_ENGINE_TYPE_ERROR;

    Handle *handle_res = (Handle*)resource;
    if (handle_res == nullptr) {
        handle_res = &h_res;
    }
    if (handle_res->GetHandleType() != PE_RESOURCE) return POLICY_ENGINE_TYPE_ERROR;

    Handle *handle_host = (Handle*)(host);
    if (handle_host == nullptr) {
        handle_host = &h_host;
    }
    if (handle_host->GetHandleType() != PE_HOST) return POLICY_ENGINE_TYPE_ERROR;

    Handle *handle_app = (Handle*)(application);
    if (handle_app == nullptr) {
        handle_app = &h_app;
    }
    if (handle_app->GetHandleType() != PE_APPLICATION) return POLICY_ENGINE_TYPE_ERROR;

    std::string act = action == nullptr ? "" : action;
    return PolicyEngine::Ins()->Match(dynamic_cast<Subject*>(handle_sub), act, dynamic_cast<Resource*>(handle_res),
                                      dynamic_cast<Host*>(handle_host), dynamic_cast<App*>(handle_app), presult);
}

