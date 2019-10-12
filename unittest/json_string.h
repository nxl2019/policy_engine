//
// Created by jeff on 2019/10/11.
//

#ifndef POLICY_ENGINE_JSON_STRING_H
#define POLICY_ENGINE_JSON_STRING_H


const std::string JSON_TEST = "{\n"
                              "\t\"id\": \"121\",\n"
                              "\t\"fullname\": \"ROOT_121\\/SPE-BasicEnforce\",\n"
                              "\t\"description\": \"null\",\n"
                              "\t\"effectType\": \"deny\",\n"
                              "\t\"actionComponents\": [{\n"
                              "\t\t\"operator\": \"IN\",\n"
                              "\t\t\"components\": [{\n"
                              "\t\t\t\"id\": \"61\",\n"
                              "\t\t\t\"name\": \"edit\",\n"
                              "\t\t\t\"actions\": [\"EDIT\"]\n"
                              "\t\t}, {\n"
                              "\t\t\t\"id\": \"60\",\n"
                              "\t\t\t\"name\": \"delete\",\n"
                              "\t\t\t\"actions\": [\"DELETE\"]\n"
                              "\t\t}]\n"
                              "\t}],\n"
                              "\t\"subjectComponents\": [{\n"
                              "\t\t\"operator\": \"IN\",\n"
                              "\t\t\"components\": [{\n"
                              "\t\t\t\"id\": \"20702\",\n"
                              "\t\t\t\"name\": \"groupsid-james.polk\",\n"
                              "\t\t\t\"conditions\": [{\n"
                              "\t\t\t\t\"attribute\": \"groupid\",\n"
                              "\t\t\t\t\"operator\": \"=\",\n"
                              "\t\t\t\t\"value\": \"S-1-5-21-2018228179-1005617703-974104760-188953\",\n"
                              "\t\t\t\t\"rhsType\": \"CONSTANT\",\n"
                              "\t\t\t\t\"rhsvalue\": \"S-1-5-21-2018228179-1005617703-974104760-188953\"\n"
                              "\t\t\t}]\n"
                              "\t\t}]\n"
                              "\t}],\n"
                              "\t\"fromResourceComponents\": [{\n"
                              "\t\t\"operator\": \"IN\",\n"
                              "\t\t\"components\": [{\n"
                              "\t\t\t\t\"id\": \"90\",\n"
                              "\t\t\t\t\"name\": \"spe-list item\",\n"
                              "\t\t\t\t\"conditions\": [{\n"
                              "\t\t\t\t\t\t\"attribute\": \"sub_type\",\n"
                              "\t\t\t\t\t\t\"operator\": \"=\",\n"
                              "\t\t\t\t\t\t\"value\": \"list item\",\n"
                              "\t\t\t\t\t\t\"rhsType\": \"CONSTANT\",\n"
                              "\t\t\t\t\t\t\"rhsvalue\": \"list item\"\n"
                              "\t\t\t\t\t},\n"
                              "\t\t\t\t\t{\n"
                              "\t\t\t\t\t\t\"attribute\": \"name\",\n"
                              "\t\t\t\t\t\t\"operator\": \"=\",\n"
                              "\t\t\t\t\t\t\"value\": \"${user.name}\",\n"
                              "\t\t\t\t\t\t\"rhsType\": \"SUBJECT\",\n"
                              "\t\t\t\t\t\t\"rhsvalue\": \"user.name\"\n"
                              "\t\t\t\t\t}\n"
                              "\t\t\t\t]\n"
                              "\t\t\t}\n"
                              "\t\t]\n"
                              "\t}],\n"
                              "\t\"denyObligations\": [{\n"
                              "\t\t\"name\": \"log\",\n"
                              "\t\t\"params\": \"\"\n"
                              "\t}],\n"
                              "\t\"allowObligations\": \"\",\n"
                              "\t\"expression\": \"resource.spe.url = \\\"sharepoint:\\/\\/**basicsite\\/**\\\"\"\n"
                              "}";

#endif //POLICY_ENGINE_JSON_STRING_H
/*
 * {
	"id": "121",
	"fullname": "ROOT_121\/SPE-BasicEnforce",
	"description": "null",
	"effectType": "deny",
	"actionComponents": [{
		"operator": "IN",
		"components": [{
			"id": "61",
			"name": "edit",
			"actions": ["EDIT"]
		}, {
			"id": "60",
			"name": "delete",
			"actions": ["DELETE"]
		}]
	}],
	"subjectComponents": [{
		"operator": "IN",
		"components": [{
			"id": "20702",
			"name": "groupsid-james.polk",
			"conditions": [{
				"attribute": "groupid",
				"operator": "=",
				"value": "S-1-5-21-2018228179-1005617703-974104760-188953",
				"rhsType": "CONSTANT",
				"rhsvalue": "S-1-5-21-2018228179-1005617703-974104760-188953"
			}]
		}]
	}],
	"fromResourceComponents": [{
		"operator": "IN",
		"components": [{
				"id": "90",
				"name": "spe-list item",
				"conditions": [{
						"attribute": "sub_type",
						"operator": "=",
						"value": "list item",
						"rhsType": "CONSTANT",
						"rhsvalue": "list item"
					},
					{
						"attribute": "name",
						"operator": "=",
						"value": "${user.name}",
						"rhsType": "SUBJECT",
						"rhsvalue": "item"
					}
				]
			}
		]
	}],
	"denyObligations": [{
		"name": "log",
		"params": ""
	}],
	"allowObligations": "",
	"expression": "resource.spe.url = \"sharepoint:\""
}
 *
 * */

/* expectation
 *                                                              /-----------------AND-------------------\
 *                                           /-----------------AND-------------------\                C_TURE
 *                           /------------- AND ---------------------\              C_UNKNOWN
 *                 /-------  OR-----------\                   /-----COMP_EQ---------------\
 *         /----COMP_EQ---\          /---COMP_EQ-----\      groupid     'S-1-5-21-2018228179-1005617703-974104760-188953'
 *      ACTION        'EDIT'    ACTION          'DELETE'
 *
 * */
