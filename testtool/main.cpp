#include "task.h"
#include "json/json.h"
#include "microtest.h"

std::string read_from_file(const std::string& path) {
    return "{\n"
           "  \"cc_params\": {\n"
           "    \"host\": \"https://cc87-console.qapf1.qalab01.nextlabs.com\",\n"
           "    \"port\": \"443\",\n"
           "    \"user\": \"administrator\",\n"
           "    \"pwd\": \"123blue!\",\n"
           "    \"tag\": \"pe_test\",\n"
           "    \"sync_interval_secs\":3600\n"
           "  },\n"
           "  \"expect_ref\": {\n"
           "    \"subject\": [\"windowssid\", \"username\", \"pe_nums\", \"level\", \"company\", \"location\", \"co\", \"cn\", \"city\"],\n"
           "    \"resource\": [\"url\", \"owner\", \"level\"],\n"
           "    \"host\": [\"windowssid\"],\n"
           "    \"application\": [\"uniquename\", \"displayname\"],\n"
           "    \"action\": [\"VIEW\", \"OPEN\", \"EDIT\", \"DELETE\"]\n"
           "  },\n"
           "  \"tasks\": [\n"
           "    {\n"
           "      \"subject\": {\"username\": \"xxx\", \"level\": \"4\"},\n"
           "      \"resource\": {},\n"
           "      \"host\": {},\n"
           "      \"application\": {},\n"
           "      \"action\": \"SELECT\",\n"
           "      \"expect_result\": \"PE_NO_MATCHED\"\n"
           "    },\n"
           "    {\n"
           "      \"subject\": {\"username\": \"xxx\", \"level\": \"4\"},\n"
           "      \"resource\": {},\n"
           "      \"host\": {},\n"
           "      \"application\": {},\n"
           "      \"action\": \"\",\n"
           "      \"expect_result\": \"PE_NEED_MORE_WORK\"\n"
           "    }\n"
           "  ]\n"
           "}";
}


TEST(TOOL) {
    std::string json_string = read_from_file("");
    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(json_string.c_str(), json_string.c_str() + json_string.length(), &root, nullptr)) {
        mt::printFailed("json format error");
        ASSERT_TRUE(false);
    }
    delete (pread); pread = nullptr;
    if (!Task::ParseCC(root["cc_params"])) {
        mt::printFailed("cc connect error");
        ASSERT_TRUE(false);
    }
    if (!Task::ParseExpectRef(root["expect_ref"])) {
        mt::printFailed("read expect_ref error");
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(Task::ValidAnalyze());
    mt::printOk("AnalyzeRef");

    if (root["tasks"].type() != Json::arrayValue) {
        mt::printFailed("read tasks error");
        ASSERT_TRUE(false);
    }
    for (unsigned i = 0; i < root["tasks"].size(); ++i) {
        Task *task = new Task;
        if (task->ParseMatch(root["tasks"][i])) {
            if(task->ValidMatch()) {
                std::string info = "task " + std::to_string(i) + " execute";
                mt::printOk(info.c_str());
            } else {
                std::string info = "task " + std::to_string(i) + " execute";
                mt::printFailed(info.c_str());
            }
            delete (task);
        } else {
            delete (task);
            std::string info = "read task " + std::to_string(i) + " error skip this test";
            mt::printFailed(info.c_str());
        }
    }
}


TEST_MAIN();