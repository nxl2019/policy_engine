#ifndef TALK_WITH_CC
#define TALK_WITH_CC

#include "tool.h"
#include <string>
#include "NXLHttpClient.h"
#define HEAD_X_CSRF_TOKEN    "X-CSRF-TOKEN"

class TalkWithCC {
public:
    static TalkWithCC *MakeTalk(const std::string& service, const std::string& port, const std::string& user, const std::string& pwd);
    TalkWithCC(NXLHttpClient *http_client, const std::string& user, const std::string& pwd);
    virtual ~TalkWithCC() { delete (_http_client); _http_client = nullptr; }
    /* todo */
    virtual bool SearchPolicyIDsByTag(const std::string& tag, std::vector<std::string>& out) = 0;
    virtual bool SearchPolicyByID(const std::string& id, std::string& out) = 0;
    virtual bool SearchComponentByID(const std::string& id, std::string& out) = 0;
    virtual bool SearchPolicyModelByID(const std::string& id, std::string& out) = 0;
    virtual bool SearchPolicyModellist( std::map<std::string, uint64_t,CommonFun::IgnoreCaseCmp> & outmap) = 0;
    virtual bool SearchPolicyModelPreAttrByName(const std::string& name, std::string& out) = 0;

public:
    virtual bool LoginToCAS() = 0;
    virtual bool CasSecurityCheck(http::response<http::string_body>& res) = 0;

public:
    static bool GetCASLoginParameters(const std::string& service, const std::string& port, const std::string& user, const std::string& pwd, TalkWithCC*& out);
    static bool GetCASLoginUrl(NXLHttpClient& http_client, const std::string& service, const std::string& port,
                               std::string& location);

protected:
    virtual bool GetRequestResult(std::string& out, http::request<http::string_body> & req) ;
protected:
    NXLHttpClient  *_http_client;
    std::string     _user;
    std::string     _pwd;
};

class TalkWithCCLower : public TalkWithCC {
public:
    TalkWithCCLower(NXLHttpClient *http_client, const std::string& user, const std::string& pwd,
            const std::string& cas_login_url, const std::string& login_submit_path,
            const std::string& login_param_lt, const std::string& login_param_execution);

    virtual bool SearchPolicyIDsByTag(const std::string& tag, std::vector<std::string>& out) override ;
    virtual bool SearchPolicyByID(const std::string& id, std::string& out) override;
    virtual bool SearchComponentByID(const std::string& id, std::string& out) override;
    virtual bool SearchPolicyModelByID(const std::string& id, std::string& out) override;
    virtual bool SearchPolicyModellist(std::map<std::string, uint64_t,CommonFun::IgnoreCaseCmp> & outmap) override ;
    virtual bool SearchPolicyModelPreAttrByName(const std::string& name, std::string& out) override;

public:
    virtual bool LoginToCAS() override;
    virtual bool CasSecurityCheck(http::response<http::string_body>& res) override;

protected:
    virtual std::string CtorLoginBody();
    virtual http::request<http::string_body> ConstructLoginRequest();
    virtual http::request<http::string_body> ConstructPolicySearchRequestByTag(const std::string& tag, const int page) ;
    virtual http::request<http::string_body> ConstructPolicyRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructComponentRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructPolicyModelRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructPolicyModelListRequest( const int page);
    virtual http::request<http::string_body> ConstructPolicyModelPreAttrRequestByName(const std::string& name);
protected:
    std::string     _cas_login_url;
    std::string     _login_submit_path;
    std::string     _login_param_lt;
    std::string     _login_param_execution;

    std::string     _cas_security_check_url;

    std::string     _set_cookie;
};


class TalkWithCC91 : public TalkWithCCLower {
public:
    TalkWithCC91(NXLHttpClient *http_client, const std::string& user, const std::string& pwd,
                 const std::string& cas_login_url, const std::string& login_submit_path,
                 const std::string& login_param_lt, const std::string& login_param_execution,
                 const std::string& login_param_csrf_token, const std::string& login_cookie);

public:
    virtual bool CasSecurityCheck(http::response<http::string_body>& res) override;
protected:
    virtual std::string CtorLoginBody() override;
    virtual http::request<http::string_body> ConstructLoginRequest() override;
    virtual http::request<http::string_body> ConstructPolicySearchRequestByTag(const std::string& tag, const int page) ;
    virtual http::request<http::string_body> ConstructPolicyRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructComponentRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructPolicyModelRequestByID(const std::string& cid);
    virtual http::request<http::string_body> ConstructPolicyModelListRequest( const int page);
    virtual http::request<http::string_body> ConstructPolicyModelPreAttrRequestByName(const std::string& name);
protected:
    std::string     _login_param_csrf_token;
    std::string     _login_cookie;
    std::string     _csfr_token;
};

#endif