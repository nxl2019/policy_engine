//
// Created by jeff on 2019/10/18.
//

#ifndef POLICY_ENGINE_BM_API_TEST_H
#define POLICY_ENGINE_BM_API_TEST_H

#include <benchmark/benchmark.h>
#include "policy_engine.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Handle.h"

static void Init(benchmark::State& state) {
    const char* cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    const char* tag = "pe_test";
    const char* ccport = "443" ;
    const char* ccuser = "administrator" ;
    const char* ccpwd = "123blue!" ;
    for (auto _: state) {
        PolicyEngineReturn ret = policy_engine_module_init(cchost, ccport, ccuser, ccpwd, tag,
                                                           3600);
    }
}
BENCHMARK(Init);

static void Analyze(benchmark::State &state) {
    for (auto _:state) {
        PolicyEngineStringList sublist = NULL;
        PolicyEngineStringList actlist = NULL;
        PolicyEngineStringList resourcelist = NULL;
        PolicyEngineStringList host = NULL;
        PolicyEngineStringList app = NULL;
        PolicyEngineReturn ret = policy_engine_analyze(&sublist, &actlist, &resourcelist, &host, &app);
        assert(ret == POLICY_ENGINE_SUCCESS);

        state.PauseTiming();
        policy_engine_destroy_string_list(sublist);
        policy_engine_destroy_string_list(actlist);
        policy_engine_destroy_string_list(resourcelist);
        policy_engine_destroy_string_list(host);
        policy_engine_destroy_string_list(app);
        state.ResumeTiming();
    }
}
BENCHMARK(Analyze)->Threads(10);

void StringList_test(benchmark::State &state) {
    for (auto _:state) {
        StringList * p = StringList::MakeStringList({"emailAddress","company","country","level", "department"});
        state.PauseTiming();
        StringList::FreeStringList(p);
        p = NULL;
        state.ResumeTiming();
    }
}
BENCHMARK(StringList_test);

static void StringlistTraversal(benchmark::State &state) {
    PolicyEngineStringList sublist = NULL;
    PolicyEngineReturn ret = policy_engine_analyze(&sublist, NULL, NULL, NULL, NULL);
    assert(ret == POLICY_ENGINE_SUCCESS);
    for (auto _: state) {
        PolicyEngineStringList it = sublist;
        ret = policy_engine_string_list_next(it, &it);
        assert(ret == POLICY_ENGINE_SUCCESS);
        const char * sub_v = nullptr;
        ret = policy_engine_string_list_current(it, &sub_v);
        assert(ret == POLICY_ENGINE_SUCCESS);
    }
//    for (PolicyEngineStringList it = sublist; it != NULL;  policy_engine_string_list_next(it, &it)) {
//        const char * sub_v = nullptr;
//        ret = policy_engine_string_list_current(it, &sub_v);
//        assert(ret == POLICY_ENGINE_SUCCESS);
//        printf("%s, ",sub_v);
//    }
    ret = policy_engine_destroy_string_list(sublist);
    assert(ret == POLICY_ENGINE_SUCCESS);
}
BENCHMARK(StringlistTraversal);

static void StringlistDestroy(benchmark::State & state) {
    for (auto _:state) {
        state.PauseTiming();
        PolicyEngineStringList sublist = NULL;
        PolicyEngineReturn ret = policy_engine_analyze(&sublist, NULL, NULL, NULL, NULL);
        state.ResumeTiming();
        //content--------------------------------
        ret = policy_engine_destroy_string_list(sublist);
        assert(ret == POLICY_ENGINE_SUCCESS);
    }
}
BENCHMARK(StringlistDestroy);

static void CreateDictionary(benchmark::State &state) {
    for (auto _:state) {
        PolicyEngineHandle  handle = NULL;
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE::PE_SUBJECT, &handle);
        assert(ret == POLICY_ENGINE_SUCCESS);
        state.PauseTiming();
        ret = policy_engine_destroy_dictionary(handle);
        assert(ret == POLICY_ENGINE_SUCCESS);
        state.ResumeTiming();
    }
}
BENCHMARK(CreateDictionary);
static void DestroyDictionary(benchmark::State &state) {
    for (auto _:state) {
        state.PauseTiming();
        PolicyEngineHandle  handle = NULL;
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE::PE_SUBJECT, &handle);
        ret = policy_engine_insert_into_dictionary(handle, "16952", "value_16952");
        state.ResumeTiming();
        ret = policy_engine_destroy_dictionary(handle);
        assert(ret == POLICY_ENGINE_SUCCESS);
    }
}
BENCHMARK(DestroyDictionary);

//static void DictionaryInsert(benchmark::State &state) {
//    PolicyEngineHandle  handle = NULL;
//    PolicyEngineReturn ret = policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE::PE_SUBJECT, &handle);
//    for (auto _:state) {
//        state.PauseTiming();
//        char a[16];
//        snprintf(a, 16, "%d", state.range(0));
//        state.ResumeTiming();
//
//        ret =  policy_engine_insert_into_dictionary(handle, a, a);
//        assert(ret == POLICY_ENGINE_SUCCESS);
//    }
//}
//BENCHMARK(DictionaryInsert)->Arg(1<<256*256);
//static void DictionaryInsert(benchmark::State &state) {
//    PolicyEngineHandle  handle = NULL;
//    PolicyEngineReturn ret = policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE::PE_SUBJECT, &handle);
//    for (auto _:state) {
//        state.PauseTiming();
//        char a[16];
//        snprintf(a, 16, "%d", rand());
//        state.ResumeTiming();
//
//        ret =  policy_engine_insert_into_dictionary(handle, a, a);
//        assert(ret == POLICY_ENGINE_SUCCESS);
//    }
//}
//BENCHMARK(DictionaryInsert);

static void DictionaryInsert(benchmark::State &state) {

    for (auto _:state) {
        PolicyEngineHandle  handle = NULL;
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(POLICY_ENGINE_HANDLE_TYPE::PE_SUBJECT, &handle);
        ret =  policy_engine_insert_into_dictionary(handle, "emailAdress", "james.polk@qapf1.qalab01.nextlabs.com");
        ret =  policy_engine_insert_into_dictionary(handle, "department", "R&D");
        ret =  policy_engine_insert_into_dictionary(handle, "level", "5");
        ret =  policy_engine_insert_into_dictionary(handle, "company", "Nextlabs");
        ret =  policy_engine_insert_into_dictionary(handle, "country", "China");
        policy_engine_destroy_dictionary(handle);
    }
}
BENCHMARK(DictionaryInsert);

static void Match(benchmark::State &state) {
    PolicyEngineHandle pdic_subjects = NULL;
    {
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_SUBJECT, &pdic_subjects);
        assert(ret == POLICY_ENGINE_SUCCESS);
        ret = policy_engine_insert_into_dictionary(pdic_subjects, "username", "rekkles");
        ret = policy_engine_insert_into_dictionary(pdic_subjects, "level", "4");
        ret = policy_engine_insert_into_dictionary(pdic_subjects, "pe_nums", "101");
        ret = policy_engine_insert_into_dictionary(pdic_subjects, "windowssid", "123");
        assert(ret == POLICY_ENGINE_SUCCESS);
    }
    PolicyEngineHandle pdic_host = NULL;
    {
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_HOST, &pdic_host);
        assert(ret == POLICY_ENGINE_SUCCESS);
        ret = policy_engine_insert_into_dictionary(pdic_host, "windowssid", "1234");
        assert(ret == POLICY_ENGINE_SUCCESS);
    }
    PolicyEngineHandle pdic_app = NULL;
    {
        PolicyEngineReturn ret = policy_engine_create_dictionary_handle(PE_APPLICATION, &pdic_app);
        assert(ret == POLICY_ENGINE_SUCCESS);
        ret = policy_engine_insert_into_dictionary(pdic_app, "uniquename", "pc-00000");
        ret = policy_engine_insert_into_dictionary(pdic_app, "displayname", "PC-00000");
        assert(ret == POLICY_ENGINE_SUCCESS);
    }

    for (auto _: state) {
        POLICY_ENGINE_MATCH_RESULT result;
        PolicyEngineReturn ret =  policy_engine_match( pdic_subjects, "VIEW", NULL,pdic_host, pdic_app , &result);
        assert(ret == POLICY_ENGINE_SUCCESS);
        assert(result == PE_NO_MATCHED);
    }

    policy_engine_destroy_dictionary(pdic_subjects);
    policy_engine_destroy_dictionary(pdic_host);
    policy_engine_destroy_dictionary(pdic_app);
}
BENCHMARK(Match);






//static void BM_SetInsert(benchmark::State &state) {
//    for (auto _:state) {
//
//    }
//}
//static void CustomArguments(benchmark::internal::Benchmark* b) {
//    for (int i = 0; i <= 10; ++i)
//        for (int j = 32; j <= 1024*1024; j *= 8)
//            b->Args({i, j});
//}
//BENCHMARK(BM_SetInsert)->Apply(CustomArguments);






#endif //POLICY_ENGINE_BM_API_TEST_H
