#ifndef CCLOGIN_HELPER_H
#define CCLOGIN_HELPER_H
#include <string>
#include "NXLHttpClient.h"
class CCLoginHelper
{
  public:
  static std::string Login(const std::string& strCCService, const std::string& strCCPort, 
                           const std::string& strUser, const std::string& strPwd);


   private:
   static std::string GetCASLoginUrl(NXLHttpClient& httpClient, const std::string& strCCService,
                                      const std::string& strCCPort);

    static bool  GetCASLoginParameters(NXLHttpClient& httpClient, const std::string&  strCASLoginUrl,
     std::string&  strLoginSubmitPath, 
      std::string&   strLoginParaLt, std::string&  strLoginParaExecution);

	static std::string LoginToCAS(NXLHttpClient&httpClient, const std::string&  strLoginSubmitPath, const std::string&  strLoginParaLt,
		const std::string& strLoginParaExecution, const std::string& strUser, const std::string&  strPwd);


	static std::string CASSecurityCheck(NXLHttpClient& httpClient,const std::string& strCASSecurityCheckUrl);
};

#endif 