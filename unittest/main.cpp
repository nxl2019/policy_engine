#include <stdio.h>
#include "policy_engine.h"
#include "Policy.h"
#include "Handle.h"
#include "Value.h"
#include "PolicyEngine.h"

int main() {
    /* Module init */
    PolicyEngineReturn r = policy_engine_module_init("", "", "", "", "", 86400);
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    PolicyEngineStringList subjects = nullptr;
    PolicyEngineStringList actions = nullptr;

    /*  */
    r = policy_engine_subjects_actions_analyze(&subjects, &actions);
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    r = POLICY_ENGINE_SUCCESS;
    for (PolicyEngineStringList it = subjects; it != nullptr && r == POLICY_ENGINE_SUCCESS; r = policy_engine_string_list_next(it, &it)) {
        const char *str = nullptr;
        r = policy_engine_string_list_current(it, &str);
        if (r != POLICY_ENGINE_SUCCESS) {
            /* error occured */
            break;
        }
        printf("%s\n", str);
    }

    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    r = POLICY_ENGINE_SUCCESS;
    for (PolicyEngineStringList it = actions; it != nullptr && r == POLICY_ENGINE_SUCCESS; r = policy_engine_string_list_next(it, &it)) {
        const char *str = nullptr;
        r = policy_engine_string_list_current(it, &str);
        if (r != POLICY_ENGINE_SUCCESS) {
            /* error occured */
            break;
        }
        printf("%s\n", str);
    }

    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    PolicyEngineHandle subject_attr = nullptr;
    r = policy_engine_create_subject_handle(&subject_attr);
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    r = policy_engine_insert_into_subject(subject_attr, "username", "JOKE");
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    POLICY_ENGINE_MATCH_RESULT result;
    r = policy_engine_match(subject_attr, "open", nullptr, &result);
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }

    if (result == PE_NO_MATCHED) {
        /* blabla */
    } else {
        /* blabla */
    }


    r = policy_engine_module_exit();
    if (r != POLICY_ENGINE_SUCCESS) {
        /* error occured */
        return -1;
    }
    return 0;
}