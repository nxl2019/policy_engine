#include "CCLoginHelper.h"
#include "log.h"
#include <memory>
#include <cctype>
#include <iomanip>
#include <sstream>
#include "tool.h"


std::string CCLoginHelper::Login(const std::string& strCCService, const std::string& strCCPort, 
                           const std::string& strUser, const std::string& strPwd)
{
    //create Connection to CCService
    NXLHttpClient httpClient(strCCService.c_str(), strCCPort.c_str());

    //step1: open ccserver/console, it will redirect us to cas login page
    std::string strCASLoginUrl = GetCASLoginUrl(httpClient, strCCService, strCCPort);
    if(strCASLoginUrl.empty())
    {
        Log::WriteLog(log_fatal, "CCLoginHelper::Login can't get cas login url.\n");
        return "";
    }


    //step2: open cas login page, we get login parameter 
    std::string  strLoginSubmitPath, strLoginParaLt, strLoginParaExecution;
    GetCASLoginParameters(httpClient, strCASLoginUrl, strLoginSubmitPath, strLoginParaLt, strLoginParaExecution);
    if(strLoginSubmitPath.empty() || strLoginParaLt.empty() || strLoginParaExecution.empty() )
    {
        Log::WriteLog(log_fatal, "CCLoginHelper::Login can't get login paramater.\n");
        return "";
    }



    //step3: login to cas, it will redirect us to CAS security check url
    std::string strCASSecurityCheckUrl = LoginToCAS(httpClient, strLoginSubmitPath, strLoginParaLt,
		                     strLoginParaExecution, strUser, strPwd );
     if(strCASSecurityCheckUrl.empty())
     {
         Log::WriteLog(log_fatal, "CCLoginHelper::Login log to cas failed.\n");
         return "";
     }



    //step4: open CAS security check url, it will redirect us to /console and set login cookie
    //       we return this cookie
    std::string strSessionCookie = CASSecurityCheck(httpClient, strCASSecurityCheckUrl);


    return strSessionCookie;

}

std::string CCLoginHelper::GetCASLoginUrl(NXLHttpClient& httpClient, const std::string& strCCService,
                                          const std::string& strCCPort)
{
    http::request<http::string_body> req{ http::verb::get, "/console/", 11 };

	//set headers
	req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
	req.set(http::field::connection, "keep-alive");
 
    //send request	
	boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
	http::response<http::string_body> res;
    try
    {
        httpClient.Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::GetCASLoginUrl:%s", e.what() );
        return "";
	} 
	catch (...)
	{
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::GetCASLoginUrl" );
        return "";
	}

    //get response header
    if (res.result() == http::status::found)
	{
        auto& headers = res.base();
		auto LocationHdr = headers.find(http::field::location);
        if(LocationHdr != headers.end())
        {
            std::string strLocationUrl = LocationHdr->value().to_string();
			Log::WriteLog(log_info, "CCLoginHelper::GetCASLoginUrl:%s", strLocationUrl.c_str());
            return strLocationUrl;
        }
	}
    
    return "";
}

bool  CCLoginHelper::GetCASLoginParameters(NXLHttpClient& httpClient, const std::string&  strCASLoginUrl,
     std::string&  strLoginSubmitPath,   std::string&   strLoginParaLt,
     std::string&  strLoginParaExecution)
{
     http::request<http::string_body> req{ http::verb::get, strCASLoginUrl, 11 };

	//set headers
	req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
	req.set(http::field::connection, "keep-alive");
 
    //send request	
	boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
	http::response<http::string_body> res;
    try
    {
        httpClient.Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::GetCASLoginParameters:%s", e.what() );
        return "";
	} 
	catch (...)
	{
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::GetCASLoginParameters" );
        return "";
	}

    //get response header
    if (res.result() == http::status::ok)
	{
       //parse body
       auto& body = res.body();
       
        //parse result
        size_t nPosForm = body.find("<form");

            //get auth url
            {
                const std::string strKey = "action=\"";
                size_t nPosAction = body.find(strKey, nPosForm);
                size_t nPosEnd = body.find("\"", nPosAction + strKey.length());
                strLoginSubmitPath = body.substr(nPosAction + strKey.length(), nPosEnd - nPosAction - strKey.length());
            }

            //get lt
            {
                const std::string strKey = "name=\"lt\"";
                size_t nPosLtName = body.find(strKey, nPosForm);
                size_t nltBegin = body.find("\"", nPosLtName + strKey.length());
                size_t nLtEnd = body.find("\"", nltBegin + 1);
                strLoginParaLt = body.substr(nltBegin + 1, nLtEnd - nltBegin - 1);
            }

            //get execute
            {
                const std::string strKey = "name=\"execution\"";
                size_t nPosExecName = body.find(strKey, nPosForm);
                size_t nExecBegin = body.find("\"", nPosExecName + strKey.length());
                size_t nExecEnd = body.find("\"", nExecBegin + 1);
                strLoginParaExecution = body.substr(nExecBegin + 1, nExecEnd - nExecBegin - 1);
            }


            return true;

	}
    
    return false;
}

std::string CCLoginHelper::LoginToCAS(NXLHttpClient&httpClient, const std::string&  strLoginSubmitPath, const std::string&  strLoginParaLt,
	const std::string& strLoginParaExecution, const std::string& strUser, const std::string&  strPwd)
{
    //construct body
	std::string strExection =  CommonFun::UrlEncode(strLoginParaExecution);
	std::string strLt = strLoginParaLt; 
	std::string strEncodePwd = CommonFun::UrlEncode(strPwd);

    const size_t bodyLen = strLoginParaLt.length() + strLoginParaExecution.length() + strUser.length() + strPwd.length() + 1024;

    std::unique_ptr<char[]> strBody(new char[bodyLen]); 
    sprintf_s(strBody.get(), bodyLen, "username=%s&password=%s&lt=%s&execution=%s&_eventId=submit&submit=Login",
                strUser.c_str(), strEncodePwd.c_str(), strLt.c_str(), strExection.c_str() );


     http::request<http::string_body> req{ http::verb::post, strLoginSubmitPath, 11, strBody.get() };

	//set headers
	req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
	req.set(http::field::connection, "keep-alive");
    req.set(http::field::content_length, strlen(strBody.get()));
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.set(http::field::accept, "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
 
    //send request	
	boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
	http::response<http::string_body> res;
    try
    {
        httpClient.Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::LoginToCAS:%s", e.what() );
        return "";
	} 
	catch (...)
	{
        Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::LoginToCAS" );
        return "";
	}

    //get response header
    if (res.result() == http::status::found)
	{
         auto& headers = res.base();
		auto LocationHdr = headers.find(http::field::location);
        if(LocationHdr != headers.end())
        {
            std::string strLocationUrl = LocationHdr->value().to_string();
			Log::WriteLog(log_info, "CCLoginHelper::LoginToCAS:%s", strLocationUrl.c_str());
            return strLocationUrl;
        }
    }

    return  "";

}

std::string CCLoginHelper::CASSecurityCheck(NXLHttpClient& httpClient, const std::string& strCASSecurityCheckUrl)
{
	http::request<http::string_body> req{ http::verb::get, strCASSecurityCheckUrl, 11};

	//set headers
	req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
	req.set(http::field::connection, "keep-alive");

	//send request	
	boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
	http::response<http::string_body> res;
	try
	{
	    httpClient.Request(req, buffer, res);
	}
	catch (boost::system::system_error const& e) {
		//disconnected
		Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::CASSecurityCheck:%s", e.what());
		return "";
	}
	catch (...)
	{
		Log::WriteLog(log_fatal, "excpetion on CCLoginHelper::CASSecurityCheck");
		return "";
	}

	//get response header
	if (res.result() == http::status::found)
	{
		auto& headers = res.base();
		auto setCookieHdr = headers.find(http::field::set_cookie);
		if (setCookieHdr != headers.end())
		{
			std::string setCookie = setCookieHdr->value().to_string();
			Log::WriteLog(log_info, "CCLoginHelper::LoginToCAS set cookie:%s", setCookie.c_str());
			return setCookie;
		}
	}

    return "";

}