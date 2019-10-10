#include "log.h"
#include <stdarg.h>
#include <stdio.h>

WriteLog_Fun Log::m_logFun = NULL;

int Log::WriteLog(int nlevel, const char* szFmt, ...)
{
    int nLog = 0;
   if(m_logFun!=NULL)
   {
    const int nMaxMsgSize = 10240;
    char szLog[nMaxMsgSize];

	//format log content
	va_list args;
	va_start(args, szFmt);
	nLog = vsnprintf(szLog, nMaxMsgSize-1, szFmt, args);
	va_end(args);

    m_logFun(nlevel, szLog);
   }
	return nLog;
}