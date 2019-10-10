#ifndef CC_POLICY_HELPER_H
#define CC_POLICY_HELPER_H
#include <string>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "NXLHttpClient.h"

namespace pt = boost::property_tree;

#define HTTP_POST_SUCCESS "1004"
#define HTTP_GET_SUCCESS  "1003"

#define POLICY_JSON_ACTIONCOMP         "actionComponents"
#define POLICY_JSON_SUBJECTCOMP        "subjectComponents"
#define POLICY_JSON_FROM_RESOURCECOMP  "fromResourceComponents"
#define POLICY_JSON_EFFECTIVE_TYPE	   "effectType"
#define POLICY_JSON_EFFECTIVE_TYPE_ALLOW	"allow"
#define POLICY_JSON_EFFECTIVE_TYPE_DENY		"deny"

#define COMPONENT_OPERATOR_IN          "IN"
#define COMPONENT_OPERATOR_NOTIN       "NOT"

class CCPolicyHelper
{
    public:
    static bool SyncPolicy(const std::string& m_strCCService,const std::string& m_strPort, 
                    const std::string&strSessionCookie, const std::string& strPolicyTag, 
                    std::list<std::string>& lstPolicy);

    private:
    static void  SearchPolicy(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                             const std::string& strPolicyTag, std::list<std::string>& lstPolicyID);

    static void QueryPolicyDetails(NXLHttpClient& httpClient,const std::string& strSessionCookie,
                                 const std::string& strPolicyID, pt::ptree& ptreePolicy);

    static bool  QueryActionComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                      pt::ptree& ptComponent, const std::string& strID);
    static bool  QuerySubjectComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                      pt::ptree& ptComponent, const std::string& strID);
    static bool  QueryResourceComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                      pt::ptree& ptComponent, const std::string& strID);
    static bool  QueryComponent(NXLHttpClient& httpClient, const std::string& strSessionCookie,
                                pt::ptree& ptComponent, const std::string& strID);

    private:
    static bool SetPolicyTag(const std::string& strOldCriteria, const std::string& strTag, std::string& strNewCriteria);
    static bool SetPageNoInCriteria(const std::string& strOldCriteria,int nPageNo, std::string& strNewCriteria);
    static bool ExtraceObligationsFromPolicy(const pt::ptree& ptDenyOb, pt::ptree& ptreePolicy);
    static bool ExtraceComponentFromPolicy(const pt::ptree& ptComponents, pt::ptree& ptPolicyComponents);


};

/*
policy format example:

{
	"id": "93",
	"fullname": "ROOT_93\/sql-test",
	"description": "for sql test",
	"effectType": "allow",
	"actionComponents": [{
		"operator": "IN",
		"components": [{
			"id": "90",
			"name": "DELETE",
			"actions": ["DELETE"]
		}, {
			"id": "91",
			"name": "EDIT",
			"actions": ["EDIT"]
		}]
	}],
	"subjectComponents": [{
		"operator": "IN",
		"components": [{
			"id": "76",
			"name": "qapf1-jimmy.carter",
			"conditions": [{
				"attribute": "emailaddress",
				"operator": "=",
				"value": "jimmy.carter@qapf1.qalab01.nextlabs.com",
				"rhsType": "CONSTANT",
				"rhsvalue": "jimmy.carter@qapf1.qalab01.nextlabs.com"
			}]
		}, {
			"id": "67",
			"name": "department=qa",
			"conditions": [{
				"attribute": "department",
				"operator": "=",
				"value": "qa",
				"rhsType": "CONSTANT",
				"rhsvalue": "qa"
			}]
		}]
	}, {
		"operator": "NOT",
		"components": [{
			"id": "77",
			"name": "adfs-jimmy.carter",
			"conditions": [{
				"attribute": "emailaddress",
				"operator": "=",
				"value": "jimmy.carter@adfs.com",
				"rhsType": "CONSTANT",
				"rhsvalue": "jimmy.carter@adfs.com"
			}]
		}, {
			"id": "84",
			"name": "email address contains Administrator",
			"conditions": [{
				"attribute": "mail",
				"operator": "=",
				"value": "**Administrator**",
				"rhsType": "CONSTANT",
				"rhsvalue": "**Administrator**"
			}]
		}]
	}],
	"fromResourceComponents": [{
		"operator": "IN",
		"components": [{
			"id": "94",
			"name": "sql-tableA",
			"conditions": [{
				"attribute": "attr1",
				"operator": "!=",
				"value": "1",
				"rhsType": "CONSTANT",
				"rhsvalue": "1"
			}, {
				"attribute": "attr2",
				"operator": "!=",
				"value": "2",
				"rhsType": "CONSTANT",
				"rhsvalue": "2"
			}]
		}, {
			"id": "95",
			"name": "sql-tableB",
			"conditions": [{
				"attribute": "attr1",
				"operator": "!=",
				"value": "2",
				"rhsType": "CONSTANT",
				"rhsvalue": "2"
			}, {
				"attribute": "attr2",
				"operator": "!=",
				"value": "3",
				"rhsType": "CONSTANT",
				"rhsvalue": "3"
			}]
		}]
	}, {
		"operator": "NOT",
		"components": [{
			"id": "94",
			"name": "sql-tableA",
			"conditions": [{
				"attribute": "attr1",
				"operator": "!=",
				"value": "1",
				"rhsType": "CONSTANT",
				"rhsvalue": "1"
			}, {
				"attribute": "attr2",
				"operator": "!=",
				"value": "2",
				"rhsType": "CONSTANT",
				"rhsvalue": "2"
			}]
		}, {
			"id": "95",
			"name": "sql-tableB",
			"conditions": [{
				"attribute": "attr1",
				"operator": "!=",
				"value": "2",
				"rhsType": "CONSTANT",
				"rhsvalue": "2"
			}, {
				"attribute": "attr2",
				"operator": "!=",
				"value": "3",
				"rhsType": "CONSTANT",
				"rhsvalue": "3"
			}]
		}]
	}],
	"denyObligations": [{
		"name": "log",
		"params": ""
	}, {
		"name": "ob1",
		"params": {
			"name1": "value1",
			"name2": "value2"
		}
	}, {
		"name": "ob2",
		"params": {
			"name1": "value1",
			"name2": "value2"
		}
	}],
	"allowObligations": [{
		"name": "ob1",
		"params": {
			"name1": "value1",
			"name2": "value2"
		}
	}, {
		"name": "ob2",
		"params": {
			"name1": "value1",
			"name2": "value2"
		}
	}],
	"expression": "user.level = \"1\""
}

*/
#endif
