#include "exceptionhandle.h"

void handle_exception(const char* szDesc) {
    try {
        throw;
    } 
    catch (const std::exception &e) {
        Log::WriteLog(log_fatal, "Exception on %s:%s", szDesc, e.what() );
    } 
     catch (...) {
         Log::WriteLog(log_fatal, "Exception on %s", szDesc);
    }
}