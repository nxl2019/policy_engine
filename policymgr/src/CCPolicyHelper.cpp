#include "CCPolicyHelper.h"
#include "log.h"
#include "exceptionhandle.h"
#include "tool.h"

bool CCPolicyHelper::SyncPolicy(const std::string &strCCService, const std::string &strCCPort,
                                const std::string &strSessionCookie, const std::string &strPolicyTag,
                                std::list<std::string> &lstPolicy)
{
    //create Connection to CCService
    NXLHttpClient httpClient(strCCService.c_str(), strCCPort.c_str());

    //get policy ids
    std::list<std::string> lstPolicyID;
    SearchPolicy(httpClient, strSessionCookie, strPolicyTag, lstPolicyID);

    //get policy details
    std::list<std::string>::iterator itPolicyID = lstPolicyID.begin();
    while (itPolicyID != lstPolicyID.end())
    {
        pt::ptree ptreePolicy;
        QueryPolicyDetails(httpClient, strSessionCookie, *itPolicyID, ptreePolicy);

        //searalize policy
        std::stringstream jsonstream;
        pt::write_json(jsonstream, ptreePolicy, false);
        std::string strPolicy = jsonstream.str();
        lstPolicy.push_back(strPolicy);

        Log::WriteLog(log_info, "Policy:%d\n%s", lstPolicy.size(), strPolicy.c_str() );

        //next
        itPolicyID++;
    }

    return true;
}

void CCPolicyHelper::SearchPolicy(NXLHttpClient &httpClient, const std::string &strSessionCookie,
                                  const std::string &strPolicyTag, std::list<std::string> &lstPolicyID)
{
    const std::string strCriteriaTemplate = "{\"criteria\":{\"fields\":[{\"field\":\"status\",\"type\":\"MULTI\",\"value\":{\"type\":\"String\",\"value\":[\"APPROVED\"]}},{\"field\":\"tags\",\"nestedField\":\"tags.key\",\"type\":\"NESTED_MULTI\",\"value\":{\"type\":\"String\",\"value\":[\"sql\"]}}],\"sortFields\":[{\"field\":\"lastUpdatedDate\",\"order\":\"DESC\"}],\"pageNo\":0,\"pageSize\":20}}";

    //set policy tag;
    std::string strCriteria;
    bool bSetTag = SetPolicyTag(strCriteriaTemplate, strPolicyTag, strCriteria);
    if (!bSetTag)
    {
        Log::WriteLog(log_fatal, "CCPolicyHelper::SearchPolicy set policy tag failed.");
        return;
    }

    //request policy one page a time
    int nPageNo = 0;
    while (true)
    {
        //construct request
        http::request<http::string_body> req{http::verb::post, "/console/api/v1/policy/search", 11, strCriteria};

        //set headers
        req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
        req.set(http::field::connection, "keep-alive");
        req.set(http::field::content_length, strCriteria.length());
        req.set(http::field::content_type, "application/json; charset=utf-8");
        req.set(http::field::accept, "application/json, text/plain, */*");
        req.set(http::field::cookie, strSessionCookie);

        //send request
        boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
        http::response<http::string_body> res;
        try
        {
            httpClient.Request(req, buffer, res);
        }
        catch (...)
        {
            handle_exception(" CCLoginHelper::SearchPolicy ");
            return;
        }

        //parse response
        if (res.result() == http::status::ok)
        {
            std::stringstream ss(res.body());
            boost::property_tree::ptree pt;
            try
            {
                boost::property_tree::read_json<boost::property_tree::ptree>(ss, pt);
                std::string strStatusCode = pt.get<std::string>("statusCode");
                if (strStatusCode == "1004")
                {
                    pt::ptree &ptDataArray = pt.get_child("data");
                    for (auto itData = ptDataArray.begin(); itData != ptDataArray.end(); itData++)
                    {
                        std::string strID = itData->second.get<std::string>("id");
                        lstPolicyID.push_back(strID);
                    }

                    //next page
                    int nTotalPage = pt.get<int>("totalPages");
                    if (nTotalPage - 1 > nPageNo)
                    {
                        nPageNo++;
                        SetPageNoInCriteria(strCriteria, nPageNo, strCriteria);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    Log::WriteLog(log_error, "CCPolicyHelper::SearchPolicy data status is not success, status=%s, body=%s",
                                  strStatusCode, res.body().c_str());
                    break;
                }
            }
            catch (...)
            {
                handle_exception(" CCLoginHelper::SearchPolicy ");
                return;
            }
        }
        else
        {
            Log::WriteLog(log_error, "CCPolicyHelper::SearchPolicy http status is not ok, status=%d, body=%s",
                          res.result(), res.body().c_str());
            break;
        }
    }
}

bool CCPolicyHelper::SetPolicyTag(const std::string &strOldCriteria, const std::string &strTag,
                                  std::string &strNewCriteria)
{
    pt::ptree ptCriteriaObj;
    try
    {
        std::stringstream ss(strOldCriteria);
        pt::read_json<pt::ptree>(ss, ptCriteriaObj);
        pt::ptree &ptCriteria = ptCriteriaObj.get_child("criteria");
        pt::ptree &ptFieldsArray = ptCriteria.get_child("fields");
        for (auto ptField = ptFieldsArray.begin(); ptField != ptFieldsArray.end(); ptField++)
        {
            std::string strField = ptField->second.get<std::string>("field");
            if (CommonFun::StrCaseCmp(strField.c_str(), "tags") == 0)
            {
                pt::ptree &ptValue = ptField->second.get_child("value");
                //remove old value
                ptValue.erase("value");

                //added new value
                pt::ptree ptreeTagValue;
                ptreeTagValue.put("", strTag);

                pt::ptree arrayValue;
                arrayValue.push_back(std::make_pair("", ptreeTagValue));

                ptValue.push_back(std::make_pair("value", arrayValue));

                //write to json
                std::stringstream jsonstream;
                pt::write_json(jsonstream, ptCriteriaObj, false);
                strNewCriteria = jsonstream.str();

                break;
            }
        }
    }
    catch (...)
    {
        handle_exception("set policy tag to criteria");
        return false;
    }

    return true;
}

bool CCPolicyHelper::SetPageNoInCriteria(const std::string &strOldCriteria, int nPageNo, std::string &strNewCriteria)
{
    pt::ptree ptCriteriaObj;
    try
    {
        std::stringstream ss(strOldCriteria);
        pt::read_json<pt::ptree>(ss, ptCriteriaObj);
        pt::ptree &ptCriteria = ptCriteriaObj.get_child("criteria");

        //remove pageNo
        ptCriteria.erase("pageNo");

        //add new value
        ptCriteria.put<int>("pageNo", nPageNo);

        //write to json
        std::stringstream jsonstream;
        pt::write_json(jsonstream, ptCriteriaObj, false);
        strNewCriteria = jsonstream.str();
    }
    catch (...)
    {
        handle_exception("set policy tag to criteria");
        return false;
    }

    return true;
}

void CCPolicyHelper::QueryPolicyDetails(NXLHttpClient &httpClient, const std::string &strSessionCookie,
                                      const std::string &strPolicyID, pt::ptree &ptreePolicy)
{
    //query policy
    const static std::string strPolicySearchBaseUrl = "/console/api/v1/policy/mgmt/active/";
    const std::string strPolicySearchUrl = strPolicySearchBaseUrl + strPolicyID;

    //construct request
    http::request<http::string_body> req{http::verb::get, strPolicySearchUrl, 11};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, strSessionCookie);

    //send request
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try
    {
        httpClient.Request(req, buffer, res);
    }
    catch (...)
    {
        handle_exception(" CCLoginHelper::GetPolicyDetails ");
        return;
    }

    //parse response
    if (res.result() == http::status::ok)
    {
        std::stringstream ss(res.body());
        boost::property_tree::ptree ptBody;
        try
        {
            boost::property_tree::read_json<boost::property_tree::ptree>(ss, ptBody);
            std::string strStatusCode = ptBody.get<std::string>("statusCode");
            if (strStatusCode == HTTP_GET_SUCCESS)
            {
                const pt::ptree &ptData = ptBody.get_child("data");

                //id
                int nID = ptData.get<int>("id");
                ptreePolicy.put<int>("id", nID);

                //name
                std::string strName = ptData.get<std::string>("fullName");
                ptreePolicy.put<std::string>("fullname", strName);

                //desc
                std::string strDesc = ptData.get<std::string>("description");
                ptreePolicy.put<std::string>("description", strDesc);

                //effectType
                std::string strEffect = ptData.get<std::string>("effectType");
                ptreePolicy.put<std::string>("effectType", strEffect);

                //actions
                const pt::ptree &ptActions = ptData.get_child(POLICY_JSON_ACTIONCOMP);
                pt::ptree ptPolicyAction;
                ExtraceComponentFromPolicy(ptActions, ptPolicyAction);
                ptreePolicy.push_back(std::make_pair(POLICY_JSON_ACTIONCOMP, ptPolicyAction));

                //subject
                const pt::ptree &ptSubjects = ptData.get_child(POLICY_JSON_SUBJECTCOMP);
                pt::ptree ptPolicySubjects;
                ExtraceComponentFromPolicy(ptSubjects, ptPolicySubjects);
                ptreePolicy.push_back(std::make_pair(POLICY_JSON_SUBJECTCOMP, ptPolicySubjects));

                //resource
                const pt::ptree &ptResources = ptData.get_child(POLICY_JSON_FROM_RESOURCECOMP);
                pt::ptree ptPolicyResource;     
                ExtraceComponentFromPolicy(ptResources, ptPolicyResource);
                ptreePolicy.push_back(std::make_pair(POLICY_JSON_FROM_RESOURCECOMP, ptPolicyResource));

                //deny obligations
                const pt::ptree &ptDenyOb = ptData.get_child("denyObligations");
                pt::ptree ptPolicyDenyOb;
                ExtraceObligationsFromPolicy(ptDenyOb, ptPolicyDenyOb);
                ptreePolicy.push_back(std::make_pair("denyObligations", ptPolicyDenyOb));

                //allow obligations
                const pt::ptree &ptAllowOb = ptData.get_child("allowObligations");
                 pt::ptree ptPolicyAllowOb;
                ExtraceObligationsFromPolicy(ptAllowOb, ptPolicyAllowOb);
                ptreePolicy.push_back(std::make_pair("allowObligations", ptPolicyAllowOb));

                //advance conditions
                std::string strExpress = ptData.get<std::string>("expression");
                ptreePolicy.put<std::string>("expression", strExpress);
            }
        }
        catch (...)
        {
            handle_exception(" CCPolicyHelper::GetPolicyDetails parse result ");
            return;
        }
    }

    //query action component.
    try
    {
        pt::ptree &ptPolicyActions = ptreePolicy.get_child("actionComponents");
        for (auto itActions = ptPolicyActions.begin(); itActions != ptPolicyActions.end(); itActions++)
        {
            pt::ptree &ptComponents = itActions->second.get_child("components");
            for (auto itComponent = ptComponents.begin(); itComponent != ptComponents.end(); itComponent++)
            {
                pt::ptree &ptComponent = itComponent->second;
                std::string strID = ptComponent.get<std::string>("id");
                QueryActionComponent(httpClient, strSessionCookie, ptComponent, strID);
            }
        }
    }
    catch(...)
    {
        handle_exception(" CCPolicyHelper::GetPolicyDetails query action component ");
        return;
    }

    //query subject component
    try
    {
        pt::ptree &ptPolicySubject = ptreePolicy.get_child("subjectComponents");
        for (auto itSubject = ptPolicySubject.begin(); itSubject != ptPolicySubject.end(); itSubject++)
        {
            pt::ptree &ptComponents = itSubject->second.get_child("components");
            for (auto itComponent = ptComponents.begin(); itComponent != ptComponents.end(); itComponent++)
            {
                pt::ptree &ptComponent = itComponent->second;
                std::string strID = ptComponent.get<std::string>("id");
                QuerySubjectComponent(httpClient, strSessionCookie, ptComponent, strID);
            }
        }
    }
    catch(...)
    {
        handle_exception(" CCPolicyHelper::GetPolicyDetails query subject component ");
        return;
    }


    //query resource component.
    try
    {
        pt::ptree &ptPolicyFromResource = ptreePolicy.get_child("fromResourceComponents");
        for (auto itFromResource = ptPolicyFromResource.begin(); itFromResource != ptPolicyFromResource.end(); itFromResource++)
        {
            pt::ptree &ptComponents = itFromResource->second.get_child("components");
            for (auto itComponent = ptComponents.begin(); itComponent != ptComponents.end(); itComponent++)
            {
                pt::ptree &ptComponent = itComponent->second;
                std::string strID = ptComponent.get<std::string>("id");
                QueryResourceComponent(httpClient, strSessionCookie, ptComponent, strID);
            }
        }
    }
    catch(...)
    {
        handle_exception(" CCPolicyHelper::GetPolicyDetails query resource component ");
        return;
    }

}

bool CCPolicyHelper::ExtraceObligationsFromPolicy(const pt::ptree &ptDenyOb, pt::ptree &ptObs)
{
    try
    {
        for (auto itDenyOb = ptDenyOb.begin(); itDenyOb != ptDenyOb.end(); itDenyOb++)
        {
            pt::ptree ptOb;
            ptOb.put<std::string>("name", itDenyOb->second.get<std::string>("name"));
            ptOb.push_back(std::make_pair("params", itDenyOb->second.get_child("params")));

            ptObs.push_back(std::make_pair("", ptOb));
        }
    }
    catch (...)
    {
        handle_exception("  CCPolicyHelper::ExtraceObligations ");
        return false;
    }

    return true;
}

bool CCPolicyHelper::ExtraceComponentFromPolicy(const pt::ptree &ptComponents, pt::ptree &ptPolicyComponents)
{
    try
    {
        for (auto itCompents = ptComponents.begin(); itCompents != ptComponents.end(); itCompents++)
        {
            pt::ptree ptPolicyComps;
            ptPolicyComps.put<std::string>("operator", itCompents->second.get<std::string>("operator"));

            pt::ptree ptPolicyCompLine;
            pt::ptree ptCompsLine= itCompents->second.get_child("components");
            for (auto itCompoentItem = ptCompsLine.begin(); itCompoentItem != ptCompsLine.end(); itCompoentItem++)
            {
                pt::ptree ptPolicyComp;
                ptPolicyComp.put<int>("id", itCompoentItem->second.get<int>("id"));
                ptPolicyCompLine.push_back(std::make_pair("", ptPolicyComp));
            }

            ptPolicyComps.push_back(std::make_pair("components", ptPolicyCompLine));
            ptPolicyComponents.push_back(std::make_pair("", ptPolicyComps));
        }
    }
    catch (...)
    {
        handle_exception("  CCPolicyHelper::ExtraceComponentFromPolicy ");
        return false;
    }

    return true;
}

bool  CCPolicyHelper::QueryActionComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                           pt::ptree& ptComponent, const std::string& strID)
{
    pt::ptree ptComponentData;
    if(QueryComponent(httpClient, strSessionCookie, ptComponentData, strID))
    {
        try
        {
            const pt::ptree& ptData = ptComponentData.get_child("data");

            //name
            ptComponent.put<std::string>("name", ptData.get<std::string>("name"));

            //actions
            ptComponent.push_back( std::make_pair("actions", ptData.get_child("actions")) );

        }
        catch(...)
        {
            handle_exception(" CCPolicyHelper::QueryActionComponent parse result ");
            return false;
        }
    }

    return true;
}

bool  CCPolicyHelper::QuerySubjectComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                      pt::ptree& ptComponent, const std::string& strID)
{
    pt::ptree ptComponentData;
    if(QueryComponent(httpClient, strSessionCookie, ptComponentData, strID))
    {
        try
        {
            const pt::ptree& ptData = ptComponentData.get_child("data");

            //name
            ptComponent.put<std::string>("name", ptData.get<std::string>("name"));

            //conditions
            ptComponent.push_back( std::make_pair("conditions", ptData.get_child("conditions")) );

        }
        catch(...)
        {
            handle_exception(" CCPolicyHelper::QuerySubjectComponent parse result ");
            return false;
        }
    }

    return true;

}

bool  CCPolicyHelper::QueryResourceComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                      pt::ptree& ptComponent, const std::string& strID)
{
     pt::ptree ptComponentData;
    if(QueryComponent(httpClient, strSessionCookie, ptComponentData, strID))
    {
        try
        {
            const pt::ptree& ptData = ptComponentData.get_child("data");

            //name
            ptComponent.put<std::string>("name", ptData.get<std::string>("name"));

            //conditions
            ptComponent.push_back( std::make_pair("conditions", ptData.get_child("conditions")) );

        }
        catch(...)
        {
            handle_exception(" CCPolicyHelper::QueryResourceComponent parse result ");
            return false;
        }
    }

    return true;

}


bool CCPolicyHelper::QueryComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                    pt::ptree& ptComponentData, const std::string& strID)
{
     //query policy
    const static std::string strComponentSearchBaseUrl = "/console/api/v1/component/mgmt/active/";
    const std::string strComponentSearchUrl = strComponentSearchBaseUrl + strID;

    //construct request
    http::request<http::string_body> req{http::verb::get, strComponentSearchUrl, 11};

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::accept, "application/json, text/plain, */*");
    req.set(http::field::cookie, strSessionCookie);

    //send request
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try
    {
        httpClient.Request(req, buffer, res);
    }
    catch (...)
    {
        handle_exception(" CCLoginHelper::QueryComponent ");
        return false;
    }

    //parse response
    if (res.result() == http::status::ok)
    {
        try
        {
            std::stringstream ss(res.body());
            boost::property_tree::read_json<boost::property_tree::ptree>(ss, ptComponentData);
            return true;
        }
        catch(...)
        {
             handle_exception(" CCLoginHelper::QueryComponent ");
             return false;
        }
    }
    else
    {
        Log::WriteLog(log_error,  "CCPolicyHelper::QueryComponent http status is not OK, status:%d, res:%s",
                      res.result(), res.body().c_str() );
    }

    return false;
}