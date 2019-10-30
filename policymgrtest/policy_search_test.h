//
// Created by jeff on 2019/10/27.
//

#ifndef POLICY_ENGINE_POLICY_SEARCH_TEST_H
#define POLICY_ENGINE_POLICY_SEARCH_TEST_H

#include "microtest.h"
#include "TalkWithCC.h"

TalkWithCC * create_talkcc() {
    const std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    const std::string ccport = "443" ;
    const std::string ccuser = "administrator" ;
    const std::string ccpwd = "123blue!" ;
    return  TalkWithCC::MakeTalk(cchost, ccport, ccuser, ccpwd);;
}

TEST(talkwithcc_search_policy_ids) {
    std::vector<std::string> out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchPolicyIDsByTag("pe_test", out);
    ASSERT_TRUE(out.size() == 4);
    for (auto it: out) {
        printf("( %s ), ",(it).c_str());
    }
    printf("\n");
    delete cc;

}

TEST(talkwithcc_search_policy) {
    std::string out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchPolicyByID("11284", out);
    delete cc;
    ASSERT_FALSE(out.empty());
}

TEST(talkwithcc_search_component) {
    std::string out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchComponentByID("11293", out);
    delete cc;
    ASSERT_FALSE(out.empty());
}

TEST(talkwithcc_search_action) {
    std::string out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchComponentByID("11268", out);
    delete cc;
    ASSERT_FALSE(out.empty());
}

TEST(talkwithcc_search_pm_list) {
    std::map<std::string, uint64_t > out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchPolicyModellist(out);
    delete cc;
    for (auto it= out.begin();  it != out.end(); ++it) {
        printf("(%s, %ld)\n",it->first.c_str(), it->second);
    }
}

TEST(talkwithcc_search_pm) {
    std::string out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchPolicyModelByID("74", out);
    delete cc;
    ASSERT_FALSE(out.empty());
}

TEST(talkwithcc_search_pm_preattr) {
    std::string out;
    TalkWithCC * cc = create_talkcc();
    cc->SearchPolicyModelPreAttrByName("Host", out);
    delete cc;
    ASSERT_FALSE(out.empty());
}
















#endif //POLICY_ENGINE_POLICY_SEARCH_TEST_H
