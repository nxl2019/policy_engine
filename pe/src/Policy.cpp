#include "Policy.h"
#include "Handle.h"
#include "policy_expression.h"
#include "json/json.h"

#define COLUMN_REF_ACTION "ACTION"

AstExpr * parse_from_condition(const Json::Value & json, AstColumnRef::COL_TYPE type);
AstExpr * parse_from_conditions(const Json::Value & conditions, AstColumnRef::COL_TYPE type);
AstExpr * parse_from_components(const Json::Value & components, AstColumnRef::COL_TYPE type, bool bIsNot = false);
AstExpr * parse_from_subject_components(const Json::Value & subject_components);

AstExpr * parse_from_actions(const Json::Value & actions, AstColumnRef::COL_TYPE type );
AstExpr * parse_from_components_for_action(const Json::Value & components, AstColumnRef::COL_TYPE type, bool bIsNot = false);
AstExpr * parse_from_action_components(const Json::Value & action_components);

AstExpr * parse_from_expression(const Json::Value & action_components);

void query_action_and_attribute(const AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes); /* actions and attibutes recursive query function */

Policy::~Policy() {
    delete (_expr);
    _expr = nullptr;
}

AstExpr * parse_from_condition(const Json::Value & json, AstColumnRef::COL_TYPE type){

    std::string rhs_type = json["rhsType"].asString();
    if (rhs_type.compare("CONSTANT") != 0) {
        return new AstExpr(AstExpr::EXPER_NOT_SUPPORT);
    }

    AstExpr * pexpr = NULL;
    std::string column_ref = json["attribute"].asString();
    AstId * pastid = new AstId(column_ref);
    AstIds ids;
    ids.push_back(pastid);
    AstExpr * pexpr_left = new AstColumnRef(type, ids);

    std::string constant_value = json["value"].asString();
    AstConstantValue * pexpr_right = new AstConstantValue(AstExpr::C_STRING);
    pexpr_right->SetValue(constant_value);

    std::string op_cond = json["operator"].asString();
    if (op_cond.compare("=") == 0) {
        if (constant_value.find("**") != std::string::npos ) {
            pexpr = new AstBinaryOpExpr(AstExpr::LIKE, pexpr_left, pexpr_right);
        } else {
            pexpr = new AstBinaryOpExpr(AstExpr::COMP_EQ, pexpr_left, pexpr_right);
        }
    }
    else if (op_cond.compare("!=") == 0) {
        if (constant_value.find("**")  != std::string::npos)  {
            pexpr = new AstBinaryOpExpr(AstExpr::NOT_LIKE, pexpr_left, pexpr_right);
        } else {
            pexpr = new AstBinaryOpExpr(AstExpr::COMP_NEQ, pexpr_left, pexpr_right);
        }
    }
    else if (op_cond.compare(">") == 0) {
        pexpr = new AstBinaryOpExpr(AstExpr::COMP_GT, pexpr_left, pexpr_right);
    }
    else if (op_cond.compare(">=") == 0) {
        pexpr = new AstBinaryOpExpr(AstExpr::COMP_GE, pexpr_left, pexpr_right);
    }
    else if (op_cond.compare("<") == 0) {
        pexpr = new AstBinaryOpExpr(AstExpr::COMP_LT, pexpr_left, pexpr_right);
    }
    else if (op_cond.compare("<=") == 0) {
        pexpr = new AstBinaryOpExpr(AstExpr::COMP_LE, pexpr_left, pexpr_right);
    }
    else {
        pexpr = new AstExpr(AstExpr::EXPER_NOT_SUPPORT);
    }

    return pexpr;
}

AstExpr * parse_from_conditions(const Json::Value & conditions, AstColumnRef::COL_TYPE type) {
    Json::Value js_cond = conditions[0];
    ///BinaryExpr
    AstExpr * pexp = parse_from_condition(js_cond, AstColumnRef::SUB);

    int i_cond_size = conditions.size();
    for (int i = 1; i < i_cond_size; ++i) {//AND
        Json::Value js_cond = conditions[i];
        ///BinaryExpr
        AstExpr * pexpr_condition = parse_from_condition(js_cond, AstColumnRef::SUB);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::AND, pexpr_temp, pexpr_condition);
    }

    return  pexp;
}

AstExpr * parse_from_components(const Json::Value & components, AstColumnRef::COL_TYPE type, bool bIsNot) {
    Json::Value js_comp = components[0];
    std::string comp_id = js_comp["id"].asString();
    std::string comp_name = js_comp["name"].asString();
    Json::Value js_conditons = js_comp["conditions"];
    AstExpr * pexp =  parse_from_conditions(js_conditons, type);

    int i_comp_size = components.size();
    for (int i = 1; i < i_comp_size; ++i) {

        Json::Value js_comp = components[i];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        Json::Value js_conditons = js_comp["conditions"];
        AstExpr * pexp_sub = parse_from_conditions(js_conditons, type);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexpr_temp, pexp_sub);

    }
    // NOT
    if (bIsNot && pexp) {
        AstExpr * pexpr_temp = pexp;
        pexp = new AstUnaryOpExpr(AstExpr::NOT, pexpr_temp);
    }

    return  pexp;
}

AstExpr * parse_from_subject_components(const Json::Value & subject_components) {
    if (subject_components.size() == 0) {
        return  new AstConstantValue(AstExpr::C_TRUE);
    }
    Json::Value js_opcomp = subject_components[0];
    std::string op = js_opcomp["operator"].asString();
    bool bIsNot = op.compare("IN") != 0;
    Json::Value js_components = js_opcomp["components"];
    AstExpr * pexp = parse_from_components(js_components, AstColumnRef::SUB, bIsNot);

    for (unsigned int i = 1; i < subject_components.size(); ++i) {
        Json::Value js_opcomp = subject_components[0];
        std::string op = js_opcomp["operator"].asString();
        bool bIsNot = op.compare("IN") != 0;
        Json::Value js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components(js_components, AstColumnRef::SUB, bIsNot);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return pexp;
}

AstExpr * parse_from_resource_components(const Json::Value & resource_components){
    if (resource_components.size() == 0) {
        return  new AstConstantValue(AstExpr::C_TRUE);
    }
    return  new AstConstantValue(AstExpr::C_UNKNOWN);
}
AstExpr *  parse_from_actions(const Json::Value & actions, AstColumnRef::COL_TYPE type ){
    AstExpr * pexp = NULL;
    {
        std::string str_action = actions[0].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(type, ids);

        pexp = new AstBinaryOpExpr(AstExpr::COMP_EQ, pexp_left, pexp_right);
    }

    for (unsigned int i = 1; i < actions.size(); ++i) {
        std::string str_action = actions[i].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(type, ids);

        AstExpr * pexp_sub = new AstBinaryOpExpr(AstExpr::COMP_EQ, pexp_left, pexp_right);

        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_components_for_action(const Json::Value & components, AstColumnRef::COL_TYPE type, bool bIsNot) {
    Json::Value js_comp = components[0];
    std::string comp_id = js_comp["id"].asString();
    std::string comp_name = js_comp["name"].asString();
    Json::Value js_actions = js_comp["actions"];
    AstExpr * pexp =  parse_from_actions(js_actions, type);

    for (unsigned int i = 1; i < components.size(); ++i) {
        Json::Value js_comp = components[i];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        Json::Value js_actions = js_comp["actions"];
        AstExpr * pexp_sub =  parse_from_actions(js_actions, type);
        ///
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    // NOT
    if (bIsNot && pexp) {
        AstExpr * pexpr_temp = pexp;
        pexp = new AstUnaryOpExpr(AstExpr::NOT, pexpr_temp);
    }

    return  pexp;
}

AstExpr * parse_from_action_components(const Json::Value & action_components){
    if (action_components.size() == 0) {
        return  new AstConstantValue(AstExpr::C_TRUE);
    }
    Json::Value js_opcomp = action_components[0];
    std::string op = js_opcomp["operator"].asString();
    bool bIsNot = op.compare("IN") != 0;
    Json::Value js_components = js_opcomp["components"];
    AstExpr * pexp = parse_from_components_for_action(js_components, AstColumnRef::ACTION, bIsNot);

    for ( unsigned int i = 1; i < action_components.size(); ++i) {
        Json::Value js_opcomp = action_components[0];
        std::string op = js_opcomp["operator"].asString();
        bool bIsNot = op.compare("IN") != 0;
        Json::Value js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components_for_action(js_components, AstColumnRef::ACTION, bIsNot);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_expression(const Json::Value & action_components) {
    std::string str_expression = action_components.asString();
    if (str_expression.compare("null")) {
        return new AstConstantValue(AstExpr::C_TRUE);
    }
    return new AstConstantValue(AstExpr::C_UNKNOWN);
}


PolicyEngineReturn Policy::ParseFromJson(const std::string& json_string) {
    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(json_string.c_str(), json_string.c_str() + json_string.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return POLICY_ENGINE_FAIL;
    }
    delete (pread);
    pread = nullptr;
    //subject
    Json::Value subject_components = root["subjectComponents"];
    AstExpr * pexp_subject_comps = parse_from_subject_components(subject_components);
    //action
    Json::Value actions_components = root["actionComponents"];
    AstExpr * pexp_action_comps = parse_from_action_components(actions_components);
    //resource
    Json::Value resource_components = root["fromResourceComponents"];
    AstExpr * pexp_resource_comps = parse_from_resource_components(resource_components);
    //advance
    Json::Value expression = root["expression"];
    AstExpr * pexp_expression = parse_from_expression(expression);

    AstExpr * pexp_sa = new AstBinaryOpExpr(AstExpr::AND, pexp_subject_comps, pexp_action_comps);
    AstExpr * pexp_sar = new AstBinaryOpExpr(AstExpr::AND, pexp_sa, pexp_resource_comps);
    _expr = new AstBinaryOpExpr(AstExpr::AND, pexp_sar, pexp_expression);

    QueryActionsAndAttributes();

    return POLICY_ENGINE_SUCCESS;
}
void query_action_and_attribute( AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes) {
    switch(pexpr->GetExprType()) {
        case AstExpr::AND:
        case AstExpr::OR:
        case AstExpr::LIKE:
        case AstExpr::NOT_LIKE:
        case AstExpr::COMP_EQ:
        case AstExpr::COMP_NEQ:
        case AstExpr::COMP_GT:
        case AstExpr::COMP_GE:
        case AstExpr::COMP_LT:
        case AstExpr::COMP_LE: {
            query_action_and_attribute(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetLeft(), actions, attributes);
            query_action_and_attribute(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetRight(), actions, attributes);
        } break;
        case AstExpr::NOT: {
            query_action_and_attribute(dynamic_cast< AstUnaryOpExpr*>(pexpr)->GetExpr(), actions, attributes);
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            AstColumnRef * pexpr_col = dynamic_cast<AstColumnRef*>(pexpr);
            if(AstColumnRef::ACTION == pexpr_col->GetColType()) {
                actions.insert(pexpr_col->GetColumn().back()->GetId());
            } else if (AstColumnRef::SUB == pexpr_col->GetColType()) {
                attributes.insert(pexpr_col->GetColumn().back()->GetId());
            }
        } break;
        default: break;
    }
}
bool Policy::QueryActionsAndAttributes() {
    query_action_and_attribute(_expr, _actions, _attributes);
    if(_actions.size() == 0 && _attributes.size() == 0) return  false;
    return  true;
}


void Policy::GetAction(std::set<std::string>& ractions) {
    ractions = _actions;
}

void Policy::GetSubjectAttributes(std::set<std::string>& rattributes) {
    rattributes = _attributes;
}

PolicyEngineReturn Policy::TryMatch(const Subject *subject, const std::string& action, BOOLEAN& rboolean) {
    /* todo */
    return POLICY_ENGINE_TODO;
}

void print(AstExpr * pexpr, int lvl){
    for (int i = 0; i < lvl; ++i)
        printf("   ");
    switch (pexpr->GetExprType()) {
        case AstExpr::OR:
        case AstExpr::AND:
        case AstExpr::LIKE:
        case AstExpr::NOT_LIKE:
        case AstExpr::COMP_EQ:
        case AstExpr::COMP_NEQ:
        case AstExpr::COMP_GT:
        case AstExpr::COMP_GE:
        case AstExpr::COMP_LT:
        case AstExpr::COMP_LE:{
            switch (pexpr->GetExprType()) {
                case AstExpr::OR:printf("|-OR\n"); break;
                case AstExpr::AND:printf("|-AND\n"); break;
                case AstExpr::LIKE:printf("|-LIKE\n"); break;
                case AstExpr::NOT_LIKE:printf("|-NOT_LIKE\n"); break;
                case AstExpr::COMP_EQ:printf("|-COMP_EQ\n"); break;
                case AstExpr::COMP_NEQ:printf("|-COMP_NEQ\n"); break;
                case AstExpr::COMP_GT:printf("|-COMP_GT\n"); break;
                case AstExpr::COMP_GE:printf("|-COMP_GE\n"); break;
                case AstExpr::COMP_LT:printf("|-COMP_LT\n"); break;
                case AstExpr::COMP_LE:printf("|-COMP_LE\n"); break;
                default:
                    break;

            }

            print(dynamic_cast<AstBinaryOpExpr*>(pexpr)->GetLeft(), lvl+1);
            print(dynamic_cast<AstBinaryOpExpr*>(pexpr)->GetRight(), lvl+1);
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            const AstIds &col = dynamic_cast<AstColumnRef *>(pexpr)->GetColumn();
            AstId *id = col[0];
            printf("|-%s\n", id->GetId().c_str());
        }
            break;
            // C_TRUE, C_FALSE, C_UNKNOWN, C_NULL, C_NUMBER, C_STRING,
        case AstExpr::C_TRUE: {
            printf("|-C_TRUE\n" );
        }break;
        case AstExpr::C_FALSE: {
            printf("|-C_FALSE\n");
        }break;
        case AstExpr::C_UNKNOWN: {
            printf("|-C_UNKNOWN\n");
        }break;
        case AstExpr::C_NULL: {
            printf("|-C_NULL\n");
        }break;
        case AstExpr::C_STRING: {
            printf("|-%s\n", dynamic_cast<AstConstantValue *>(pexpr)->GetValueAsStr());
        }break;
        case AstExpr::NOT: {
            printf("|-NOT\n");
            print(dynamic_cast<AstUnaryOpExpr*>(pexpr)->GetExpr(), lvl+1);
        } break;
        default:
            break;
    }
}

void Policy::Dump() {
    print(_expr, 1);
}

