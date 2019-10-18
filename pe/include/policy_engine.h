#ifndef POLICY_ENGINE_H
#define POLICY_ENGINE_H

#ifndef WIN32
#define POLICY_ENGINE_PUBLIC_API    __attribute__((visibility("default")))
#define POLICY_ENGINE_LOCAL_API     __attribute__((visibility("hidden")))
#else
#ifdef POLICY_ENGINE_EXPORT
#define POLICY_ENGINE_PUBLIC_API    __declspec(dllexport)
#else
#deifne POLICY_ENGINE_PUBLIC_API    __declspec(dllimport)
#endif //POLICY_ENGINE_EXPORT
#endif //WIN32

#ifdef __cplusplus
extern "C" {
#endif

typedef short PolicyEngineReturn;

typedef void* PolicyEngineHandle;

typedef PolicyEngineHandle PolicyEngineStringList;
typedef PolicyEngineHandle PolicyEngineSubject;
typedef PolicyEngineHandle PolicyEngineResource;
typedef PolicyEngineHandle PolicyEngineHost;
typedef PolicyEngineHandle PolicyEngineApplication;

#define POLICY_ENGINE_TODO 1
#define POLICY_ENGINE_SUCCESS 0
#define POLICY_ENGINE_FAIL -1
#define POLICY_ENGINE_TYPE_ERROR -2
#define POLICY_ENGINE_MODULE_NOT_INIT -3
#define POLICY_ENGINE_INVALID_HANDLE -4
#define POLICY_ENGINE_CCCONNECT_ERROR -5


/*
 * pe module will parse and analyze the policy's all components
 * SUBJECT COMPONENTS
 * ACTION
 * RESOURCE COMPONENTS
 * ADVANCED CONDITION
 * */
enum POLICY_ENGINE_HANDLE_TYPE { PE_STRING_LIST, PE_SUBJECT, PE_HOST, PE_APPLICATION, PE_RESOURCE };


PolicyEngineReturn policy_engine_module_init(const char *cchost, const char *ccport, const char *ccuser, const char *ccpwd, const char *tag,
                                             unsigned int sync_interval_seconds);

PolicyEngineReturn policy_engine_module_exit();

/*
 * param psubjects_string_list:  return all referenced subject attribute names in the policys
 * param pactions_string_list:   return all referenced action names in the policys
 * param presource_string_list:  return all referenced resource attribute names in the policys
 * param phost_string_list:      return all referenced host attribute names in the policys
 * param papp_string_list:       return all referenced application attributes names in the policys
 *
 * input NULL means you don't want to collect this type of attribute names
 * */
PolicyEngineReturn policy_engine_analyze(PolicyEngineStringList *psubjects_string_list, PolicyEngineStringList *pactions_string_list,
        PolicyEngineStringList *presource_string_list, PolicyEngineStringList *phost_string_list, PolicyEngineStringList *papp_string_list);

PolicyEngineReturn policy_engine_destroy_string_list(PolicyEngineStringList pstring_list);


/*
 * iterator of the string list
 * */
PolicyEngineReturn policy_engine_string_list_current(PolicyEngineStringList pstring_list, const char **string);

PolicyEngineReturn policy_engine_string_list_next(PolicyEngineStringList pstring_list, PolicyEngineStringList *next);


/*
 * constructor of k,v pairs like a dictionary
 * param pdictionary:           return the specific dictionary handle
 * */
PolicyEngineReturn policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE dictionary_type, PolicyEngineHandle *pdictionary);

PolicyEngineReturn policy_engine_destroy_dictionary(PolicyEngineHandle dictionary);

PolicyEngineReturn policy_engine_insert_into_dictionary(PolicyEngineHandle dictionary, const char *attribute_name, const char *attribute_value);


/*
 * return param presult:    PE_NO_MATCHED means all of the policys are not matched
 * for example, Policy List is
 * Policy 01:  SUB.GROUP_ID = 123 AND ACTION = 'VIEW' AND { RES ... } AND { ADVANCE_EXPRESSION ... }
 * Policy 02:  SUB.GROUP_ID = 234 AND ACTION = 'EDIT' AND { RES ... } AND { ADVANCE_EXPRESSION ... }
 * When Input:
 *          GROUP_ID(456)                   ====> PE_NO_MATCHED
 *          GROUP_ID(123)                   ====> PE_NEED_MORE_WORK
 *          ACTION('OPEN')                  ====> PE_NO_MATCHED
 *          ACTION('EDIT')                  ====> PE_NEED_MORE_WORK
 *          GROUP_ID(123),ACTION('EDIT')    ====> PE_NO_MATCHED
 *                                          ====> PE_NEED_MORE_WORK
 * */


/*
 * param subject, action, resource, host, application NULL means you an empty dictionary
 * */
enum POLICY_ENGINE_MATCH_RESULT { PE_NO_MATCHED = 0, PE_NEED_MORE_WORK /* todo */ };
PolicyEngineReturn policy_engine_match( PolicyEngineSubject subject, const char *action, PolicyEngineResource resource,
        PolicyEngineHost host, PolicyEngineApplication application, POLICY_ENGINE_MATCH_RESULT *presult);


#ifdef __cplusplus
}
#endif

#endif