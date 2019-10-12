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

#define POLICY_ENGINE_TODO 1
#define POLICY_ENGINE_SUCCESS 0
#define POLICY_ENGINE_FAIL -1
#define POLICY_ENGINE_TYPE_ERROR -2
#define POLICY_ENGINE_MODULE_NOT_INIT -3

/*
 * pe module will parse and analyze the policy's subject component and action component
 * operator support
 * NUMBER  EQ  NEQ  LT  LE  GT  GE
 * STRING  EQ  NEQ
 * todo resource component and advanced expression
 * todo operator LIKE INCLUDE ...
 * */


PolicyEngineReturn policy_engine_module_init(const char *cchost, const char *ccport, const char *ccuser, const char *ccpwd, const char *tag,
                                             unsigned int sync_interval_seconds);

PolicyEngineReturn policy_engine_module_exit();

/*
 * return param psubjects_string_list:  all referenced subject attribute names in the policys
 * return param pactions_string_list:   all referenced action names in the policys
 * */
PolicyEngineReturn policy_engine_subjects_actions_analyze(PolicyEngineStringList *psubjects_string_list, PolicyEngineStringList *pactions_string_list);

PolicyEngineReturn policy_engine_destroy_string_list(PolicyEngineStringList pstring_list);


/*
 * iterator of the string list
 * */
PolicyEngineReturn policy_engine_string_list_current(PolicyEngineStringList pstring_list, const char **string);

PolicyEngineReturn policy_engine_string_list_next(PolicyEngineStringList pstring_list, PolicyEngineStringList *next);


/*
 * constructor of subject k,v pairs
 * */
enum POLICY_ENGINE_HANDLE_TYPE { PE_STRING_LIST, PE_SUBJECT };
PolicyEngineReturn policy_engine_create_subject_handle(PolicyEngineSubject *psubject);

PolicyEngineReturn policy_engine_destroy_subject(PolicyEngineSubject psubject);

PolicyEngineReturn policy_engine_insert_into_subject(PolicyEngineSubject psubject, const char *attribute_name, const char *attribute_value);


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

enum POLICY_ENGINE_MATCH_RESULT { PE_NO_MATCHED = 0, PE_NEED_MORE_WORK /* todo */ };
PolicyEngineReturn policy_engine_match( PolicyEngineSubject subject, const char *action, PolicyEngineResource resource, POLICY_ENGINE_MATCH_RESULT *presult);


#ifdef __cplusplus
}
#endif

#endif