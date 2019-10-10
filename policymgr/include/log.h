#ifndef POLICY_MGR_LOG_H
#define POLICY_MGR_LOG_H

typedef int(*WriteLog_Fun)(int nLogLevel, const char* szMsg);


enum emLogLevel
{
    log_trace = 0,
    log_debug ,
    log_info,
    log_warning,
    log_error,
    log_fatal
};

class Log
{

    public:
     //log callback
     static WriteLog_Fun m_logFun;

     public:
     static int  WriteLog(int lvl, const char* fmt, ...);

};

#endif 
