//
// Created by jeff on 2019/10/17.
//

#ifndef POLICY_ENGINE_HANDLE_TEST_H
#define POLICY_ENGINE_HANDLE_TEST_H

#include "microtest.h"
#include "Handle.h"


TEST(handle_case_1) {
//    std::set<std::string> sets{"aaa", "bbb", "ccc"};
//    StringList * ps = StringList::MakeStringList(sets);
//
//    StringList::FreeStringList(ps);


    const  char * p = "你好";
    int ilenc = strlen(p);
    printf("CHAR--%d\n", ilenc);
    const  char16_t * p1 = u"你好";
    int ilen = 0;
    for (auto it = p1; *it != u'\0'; ++it ) {
        ilen++;
    }
    printf("CHAR16--%d\n", ilen);

}

#endif //POLICY_ENGINE_HANDLE_TEST_H
