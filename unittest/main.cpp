#include <time.h>
#include "microtest.h"
#ifndef WIN32
//#include "eval_expression_test.h"
//#include "lex_test.h"
//#include "parse_expression_test.h"
//#include "policy_parse_test.h"
//#include "handle_test.h"
#endif

#include "c_api_test3.h"
//#include "c_api_test.h"
//#include "c_api_test2.h"




TEST_MAIN()


void UINTToString(uint64_t value, char*& current) {
    *--current = 0;
    do {
        *--current = static_cast<char>(value % 10 + static_cast<unsigned>('0'));
        value /= 10;
    } while (value != 0);
}
TEST(uint2string) {
    char aaa[5]{0};
    char * aab = aaa;
    uint64_t  value = 10234;
    UINTToString(value, aab);

}








