#include <time.h>
#include "microtest.h"
#include "TalkWithCC.h"
#include "NXLHttpClient.h"

#include "policy_mgr_test.h"
#include "policy_str2js_test.h"
#include "policy_search_test.h"



TEST(POLICYMGR_CC87) {

    std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;

    TalkWithCC *talk = nullptr;
    bool r = TalkWithCC::GetCASLoginParameters(cchost, ccport, ccuser, ccpwd, talk);
    ASSERT_TRUE(r);
    r = talk->LoginToCAS();
    ASSERT_TRUE(r);
    http::response<http::string_body> res;
    r = talk->CasSecurityCheck(res);
    ASSERT_TRUE(r);
    std::string c;
    r = talk->SearchComponentByID("11307", c);
    ASSERT_TRUE(r);
    delete (talk);
}

TEST(_POLICYMGR_CC87) {
    std::string cchost = "https://cc87-console.qapf1.qalab01.nextlabs.com";
    std::string ccport = "443" ;
    std::string ccuser = "administrator" ;
    std::string ccpwd = "123blue!" ;


    TalkWithCC *talk = TalkWithCC::MakeTalk(cchost, ccport, ccuser, ccpwd);
    assert(talk != nullptr);
    std::string value;
    bool r = talk->SearchComponentByID("11307", value);
    ASSERT_TRUE(r);
    delete (talk);
}

TEST(POLICYMGR_CC91) {

    std::string cchost = "https://cc91-console.qapf1.qalab01.nextlabs.com";
    std::string ccport = "443" ;
    std::string ccuser = "Administrator" ;
    std::string ccpwd = "123Blue!123" ;


    TalkWithCC *talk = TalkWithCC::MakeTalk(cchost, ccport, ccuser, ccpwd);
    assert(talk != nullptr);
    std::string value;
    bool r = talk->SearchComponentByID("149", value);
    ASSERT_TRUE(r);
    delete (talk);
}



TEST_MAIN()












