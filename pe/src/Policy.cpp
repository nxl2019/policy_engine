#include "tool.h"
#include "Handle.h"
#include "policy_expression.h"
#include "json/json.h"
#include "eval_expression.h"
#include "policy_regex.h"
#include "parse_expression.h"
#include "Lex.h"
#include "Policy.h"
#include <assert.h>
#include <vector>
#include "PolicyModelList.h"
AstColumnRef::COL_TYPE  transform_type(PolicyModel::PM_TYPE pm_type) {
    switch (pm_type) {
        case PolicyModel::PM_RES:
            return AstColumnRef::RES;
        case PolicyModel::PM_SUB_USER:
            return AstColumnRef::SUB;
        case PolicyModel::PM_SUB_APP:
            return AstColumnRef::APP;
        case PolicyModel::PM_SUB_HOST:
            return AstColumnRef::HOST;
        default:
            return AstColumnRef::OTHER;
    }
}
AstColumnRef::VAL_TYPE  transform_type(AttributeInfo::ATTR_TYPE attr_type) {
    switch (attr_type) {
        case AttributeInfo::A_NUMBER:
            return AstColumnRef::CC_NUMBER;
        case AttributeInfo::A_STRING:
            return AstColumnRef::CC_STRING;
        case AttributeInfo::A_MULTI:
            return AstColumnRef::CC_MULTI;
        default:
            return AstColumnRef::CC_OTHER;
    }
}

#define COLUMN_REF_ACTION "ACTION"

const std::map<std::string,const AstExpr::EXPR_TYPE, CommonFun::IgnoreCaseCmp> OPERATOR_TABLE{
        {"=",   AstExpr::COMP_EQ},
        {"!=",  AstExpr::COMP_NEQ},
        {">",   AstExpr::COMP_GT},
        {">=",  AstExpr::COMP_GE},
        {"<",   AstExpr::COMP_LT},
        {"<=",  AstExpr::COMP_LE},
        {"INCLUDES",           AstExpr::INCLUDES},
        {"EQUALS_UNORDERED",   AstExpr::EQUALS_UNORDERED}
};
AstExpr::EXPR_TYPE get_asttype_from_oprtable(const std::string & sopr) {
    auto it = OPERATOR_TABLE.find(sopr);
    if (it == OPERATOR_TABLE.end()) return AstExpr::EXPER_NOT_SUPPORT;
    return it->second;
}

AstExpr * parse_from_condition(const Json::Value & json, PolicyModelList * ppmlst, const uint64_t pm_id);
AstExpr * parse_from_conditions(const Json::Value & conditions, PolicyModelList * ppmlst, const uint64_t pm_id);
AstExpr * parse_from_components(const Json::Value & components, PolicyModelList * ppmlst, bool is_not = false);
AstExpr * parse_from_com_components(const Json::Value & com_components, PolicyModelList * ppmlst);

AstExpr * parse_from_actions(const Json::Value & actions );
AstExpr * parse_from_components_for_action(const Json::Value & components,  bool is_not = false);
AstExpr * parse_from_action_components(const Json::Value & action_components);

AstExpr * parse_from_expression(const Json::Value & action_components, PolicyModelList * ppmlst);

void analyze_reference( AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes, std::set<std::string> & resourceattrs, std::set<std::string> & rhost, std::set<std::string> & rapp); /* actions and attibutes recursive query function */

bool parse_column_ref( std::string & pm_name, std::string & attr_name, const std::string & s, const AstColumnRef::COL_TYPE  type); /* parser r-value to column-ref */

Policy::~Policy() {
    if(_expr) delete (_expr);
    _expr = nullptr;
    if(_pres_expr) delete (_pres_expr);
    _pres_expr = nullptr;
    for (auto it: _obg_cols) {
        delete(it);
    }
    _obg_cols.clear();
}

bool parse_column_ref( std::string & pm_name, std::string & attr_name, const std::string & s, const AstColumnRef::COL_TYPE  type) {
    if(s.length() < 4) return false;
    if(s[0] != '$') return false;
    if(s[1] != '{') return false;
    // ${abc.def.efg...}
    int len = s.length();
    if(s[len-1] != '}') return false;
    int i = 2;
    int start = i;
    //filter first dot ,  if type = resource
    if(type == AstColumnRef::RES) {
        while(i <= len-1) {
            if(s[i] == '.'  ) {
                i++;
                break;
            }
            i++;
        }
    }
    //second dot is resource/subject model name
    start = i;
    while(i <= len-1) {
        if (s[i] == '.' ) {
            pm_name = s.substr(start, i - start);
            i++;
            break;
        }
        i++;
    }
    //final is attribute name
    start = i;
    while (i <= len-1) {
        if (s[i] == '}' && i == len-1) {
            attr_name = s.substr(start, i - start);
        }
        i++;
    }
    return true;
}

AstExpr * parse_from_condition(const Json::Value & json, PolicyModelList * ppmlst, const uint64_t pm_id){

    std::string column_ref = json["attribute"].asString();
    AstId * pastid = new AstId(column_ref);
    AstIds ids;
    ids.push_back(pastid);
    AstExpr * pexpr_left = NULL;
    if (ppmlst != NULL) {
        AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByID(pm_id));
        AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmidAttrName(pm_id, column_ref));
        pexpr_left = new AstColumnRef(col_type, val_type, ids);
    } else {
        pexpr_left = new AstColumnRef(AstColumnRef::OTHER, AstColumnRef::CC_OTHER, ids);
    }


    AstExpr * pexpr_right = NULL;
    std::string constant_value = json["value"].asString();
    std::string rhs_type = json["rhsType"].asString();
    if (rhs_type.compare("CONSTANT") == 0) {
        if ( is_policy_regex(constant_value) ) {
            AstConstantValue * pexpr_temp = new AstConstantValue(AstExpr::C_PATTERN);
            pexpr_temp->SetValue(constant_value);
            pexpr_right = pexpr_temp;
        } else {
            AstConstantValue * pexpr_temp = new AstConstantValue(AstExpr::C_STRING);
            pexpr_temp->SetValue(constant_value);
            pexpr_right = pexpr_temp;
        }

    } else if (rhs_type.compare("SUBJECT") == 0){
        AstIds ids;
        std::string pm_name, attr_name;
        parse_column_ref(pm_name, attr_name, constant_value, AstColumnRef::SUB);
        ids.push_back(new AstId(pm_name));
        ids.push_back(new AstId(attr_name));
        if (ppmlst != NULL) {
            AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByPmname(pm_name));
            AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmnameAttrName(pm_name, attr_name));
            pexpr_right = new AstColumnRef(col_type, val_type, ids);
        } else {
            pexpr_right = new AstColumnRef(AstColumnRef::OTHER, AstColumnRef::CC_OTHER, ids);
        }

    } else if (rhs_type.compare("RESOURCE") == 0) {
        AstIds ids;
        std::string pm_name, attr_name;
        parse_column_ref(pm_name, attr_name, constant_value,AstColumnRef::RES);
        ids.push_back(new AstId(pm_name));
        ids.push_back(new AstId(attr_name));
        if (ppmlst != NULL) {
            AstColumnRef::COL_TYPE col_type = transform_type(ppmlst->GetPMTypeByPmname(pm_name));
            AstColumnRef::VAL_TYPE val_type = transform_type(ppmlst->GetAttrTypeByPmnameAttrName(pm_name, attr_name));
            pexpr_right = new AstColumnRef(col_type, val_type, ids);
        } else {
            pexpr_right = new AstColumnRef(AstColumnRef::OTHER, AstColumnRef::CC_OTHER, ids);
        }

    } else {
        return new AstConstantValue(AstExpr::C_UNKNOWN);
    }



    std::string op_cond = json["operator"].asString();
    AstExpr::EXPR_TYPE ast_type = get_asttype_from_oprtable(op_cond);

    if (ast_type == AstExpr::EXPER_NOT_SUPPORT) {
        return new AstExpr(AstExpr::EXPER_NOT_SUPPORT);
    }

    if (pexpr_right->GetExprType() == AstExpr::C_PATTERN) {
        if (ast_type == AstExpr::COMP_EQ) {
            ast_type = AstExpr::LIKE;
        } else if (ast_type == AstExpr::COMP_NEQ) {
            ast_type = AstExpr::NOT_LIKE;
        }
    }
    return new AstBinaryOpExpr(ast_type, pexpr_left, pexpr_right);

}

AstExpr * parse_from_conditions(const Json::Value & conditions, PolicyModelList * ppmlst, const uint64_t pm_id) {
    Json::Value js_cond = conditions[0];
    ///BinaryExpr
    AstExpr * pexp = parse_from_condition(js_cond, ppmlst, pm_id);

    int i_cond_size = conditions.size();
    for (int i = 1; i < i_cond_size; ++i) {//AND
        Json::Value js_cond = conditions[i];
        ///BinaryExpr
        AstExpr * pexpr_condition = parse_from_condition(js_cond, ppmlst, pm_id);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::AND, pexpr_temp, pexpr_condition);
    }

    return  pexp;
}

AstExpr * parse_from_components(const Json::Value & components, PolicyModelList * ppmlst, bool is_not) {
    Json::Value js_comp = components[0];
    uint64_t pm_id = js_comp["policy_model_id"].asInt();

    //std::string comp_name = js_comp["name"].asString();
    Json::Value js_conditons = js_comp["conditions"];
    AstExpr * pexp =  parse_from_conditions(js_conditons, ppmlst, pm_id);

    int i_comp_size = components.size();
    for (int i = 1; i < i_comp_size; ++i) {

        Json::Value js_comp = components[i];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        Json::Value js_conditons = js_comp["conditions"];
        AstExpr * pexp_sub = parse_from_conditions(js_conditons, ppmlst, pm_id);
        ///
        AstExpr * pexpr_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexpr_temp, pexp_sub);

    }
    // NOT
    if (is_not && pexp) {
        AstExpr * pexpr_temp = pexp;
        pexp = new AstUnaryOpExpr(AstExpr::NOT, pexpr_temp);
    }

    return  pexp;
}

AstExpr * parse_from_com_components(const Json::Value & com_components, PolicyModelList * ppmlst) {
    if (com_components.size() == 0) {
        return  new AstConstantValue(AstExpr::C_TRUE);
    }
    Json::Value js_opcomp = com_components[0];
    std::string op = js_opcomp["operator"].asString();
    bool is_not = op.compare("IN") != 0;
    Json::Value js_components = js_opcomp["components"];
    AstExpr * pexp = parse_from_components(js_components, ppmlst, is_not);

    for (unsigned int i = 1; i < com_components.size(); ++i) {
        Json::Value js_opcomp = com_components[i];
        std::string op = js_opcomp["operator"].asString();
        bool is_not = op.compare("IN") != 0;
        Json::Value js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components(js_components, ppmlst, is_not);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return pexp;
}

AstExpr *  parse_from_actions(const Json::Value & actions ){
    AstExpr * pexp = NULL;
    {
        std::string str_action = actions[0].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(AstColumnRef::ACTION, AstColumnRef::CC_STRING,  ids);

        pexp = new AstBinaryOpExpr(AstExpr::COMP_EQ, pexp_left, pexp_right);
    }

    for (unsigned int i = 1; i < actions.size(); ++i) {
        std::string str_action = actions[i].asString();
        AstConstantValue * pexp_right = new AstConstantValue(AstExpr::C_STRING);
        pexp_right->SetValue(str_action);

        AstId * pid = new AstId(COLUMN_REF_ACTION);
        AstIds ids ;
        ids.push_back(pid);
        AstExpr * pexp_left = new AstColumnRef(AstColumnRef::ACTION, AstColumnRef::CC_STRING,  ids);

        AstExpr * pexp_sub = new AstBinaryOpExpr(AstExpr::COMP_EQ, pexp_left, pexp_right);

        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_components_for_action(const Json::Value & components,  bool is_not) {
    Json::Value js_comp = components[0];
    std::string comp_id = js_comp["id"].asString();
    std::string comp_name = js_comp["name"].asString();
    Json::Value js_actions = js_comp["actions"];
    AstExpr * pexp =  parse_from_actions(js_actions);

    for (unsigned int i = 1; i < components.size(); ++i) {
        Json::Value js_comp = components[i];
        std::string comp_id = js_comp["id"].asString();
        std::string comp_name = js_comp["name"].asString();
        Json::Value js_actions = js_comp["actions"];
        AstExpr * pexp_sub =  parse_from_actions(js_actions);
        ///
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    // NOT
    if (is_not && pexp) {
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
    bool is_not = op.compare("IN") != 0;
    Json::Value js_components = js_opcomp["components"];
    AstExpr * pexp = parse_from_components_for_action(js_components,  is_not);

    for ( unsigned int i = 1; i < action_components.size(); ++i) {
        Json::Value js_opcomp = action_components[i];
        std::string op = js_opcomp["operator"].asString();
        bool is_not = op.compare("IN") != 0;
        Json::Value js_components = js_opcomp["components"];
        AstExpr * pexp_sub = parse_from_components_for_action(js_components,  is_not);
        AstExpr * pexp_temp = pexp;
        pexp = new AstBinaryOpExpr(AstExpr::OR, pexp_temp, pexp_sub);
    }

    return  pexp;
}

AstExpr * parse_from_expression(const Json::Value & action_components, PolicyModelList * ppmlst) {
    std::string str_expression = action_components.asString();
    if (str_expression.compare("null") == 0) {
        return new AstConstantValue(AstExpr::C_TRUE);
    }
    Lex lex(str_expression);
    lex.Next();
    ParseException e;
    e._syms = ppmlst;
    AstExpr *expr = parse_boolean_expr(&lex, &e);

    if (e._code != ParseException::SUCCESS || lex.GetCurrent()->GetType() != Token::TK_END_P) {
        return  new AstConstantValue(AstExpr::C_UNKNOWN);
    }
    return  expr;
}

bool parse_id_from_obligation_json(const Json::Value & json_obgs, std::vector<AstColumnRef*> &obg_cols) {
    for (unsigned int i = 0; i < json_obgs.size(); ++i) {
        Json::Value json_obg = json_obgs[i];
        //std::string strname = json_obg["name"].asString();
        Json::Value json_params = json_obg["params"];
        if (json_params.isString() || json_params.isNull()) {
            continue;
        }
        Json::Value::Members members = json_params.getMemberNames();
        for (auto it = members.begin(); it != members.end(); ++it) {
            std::string strkey(*it);
            std::string strvalue = json_params[strkey].asString();
            LexOb lex(strvalue);
            lex.Next();
            std::vector<AstColumnRef*> cols = parse_oblication(&lex);
            obg_cols.insert(obg_cols.end(), cols.begin(), cols.end());
        }
    }
    return true;
}

PolicyEngineReturn Policy::ParseFromJson(const std::string& json_str, PolicyModelList * ppmlst) {
    Json::CharReaderBuilder builder;
    Json::CharReader *pread = builder.newCharReader();
    Json::Value root;
    if (!pread->parse(json_str.c_str(), json_str.c_str() + json_str.length(), &root, nullptr)) {
        delete (pread);
        printf("json string is incorrect");
        return POLICY_ENGINE_FAIL;
    }
    delete (pread);
    pread = nullptr;

    return  ParseFromJson(root, ppmlst);
}

PolicyEngineReturn Policy::ParseFromJson(const Json::Value & root, PolicyModelList * ppmlst ) {
    Json::StreamWriterBuilder builder;
    const std::string json_file = Json::writeString(builder, root);
    printf("polic: %s\n", json_file.c_str());
    //action
    Json::Value actions_components = root["actionComponents"];
    AstExpr * pexp_action_comps = parse_from_action_components(actions_components);
    //subject
    Json::Value subject_components = root["subjectComponents"];
    AstExpr * pexp_subject_comps = parse_from_com_components(subject_components, ppmlst);
    //resource
    Json::Value resource_components = root["fromResourceComponents"];
    AstExpr * pexp_resource_comps = parse_from_com_components(resource_components, ppmlst);
    //advance
    Json::Value expression = root["expression"];
    AstExpr * pexp_expression = parse_from_expression(expression, ppmlst);
    //printf("aaa=%s\n", expression.asCString());
    Json::Value denyobgs = root["denyObligations"];
    parse_id_from_obligation_json(denyobgs, _obg_cols);
    Json::Value allowobgs = root["allowObligations"];
    parse_id_from_obligation_json(allowobgs, _obg_cols);

    AstExpr * pexpr_resource_result = NULL; /// resource tree  can't be calculated
    if (AstExpr::C_TRUE != pexp_resource_comps->GetExprType()) {
        pexpr_resource_result = new AstConstantValue(AstExpr::C_UNKNOWN);
    } else {
        pexpr_resource_result = new AstConstantValue(AstExpr::C_TRUE);
    }

    AstExpr * pexp_sa = new AstBinaryOpExpr(AstExpr::AND, pexp_action_comps, pexp_subject_comps );
    AstExpr * pexp_sar = new AstBinaryOpExpr(AstExpr::AND, pexp_sa, pexpr_resource_result );

    _expr = new AstBinaryOpExpr(AstExpr::AND, pexp_sar, pexp_expression);
    _pres_expr = pexp_resource_comps;

    AnalyzeReference();

    return POLICY_ENGINE_SUCCESS;
}
void analyze_reference( AstExpr * pexpr, std::set<std::string> & actions, std::set<std::string> & attributes, std::set<std::string> & resourceattrs, std::set<std::string> & rhost, std::set<std::string> & rapp) {
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
            analyze_reference(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetLeft(), actions, attributes, resourceattrs, rhost, rapp);
            analyze_reference(dynamic_cast< AstBinaryOpExpr*>(pexpr)->GetRight(), actions, attributes, resourceattrs, rhost, rapp);
        } break;
        case AstExpr::NOT: {
            analyze_reference(dynamic_cast< AstUnaryOpExpr*>(pexpr)->GetExpr(), actions, attributes, resourceattrs, rhost, rapp);
        } break;
        case AstExpr::EXPR_COLUMN_REF: {
            AstColumnRef * pexpr_col = dynamic_cast<AstColumnRef*>(pexpr);
            if(AstColumnRef::ACTION == pexpr_col->GetColType()) {

                AstExpr * pparent = pexpr_col->GetParent();
                AstExpr * pbrother = dynamic_cast<AstBinaryOpExpr*>(pparent)->GetRight();
                assert(pbrother->GetExprType() == AstExpr::C_STRING);
                actions.insert(dynamic_cast<AstConstantValue*>(pbrother)->GetValueAsStr());
            } else if (AstColumnRef::SUB == pexpr_col->GetColType()) {
                attributes.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::RES == pexpr_col->GetColType()) {
                resourceattrs.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::HOST == pexpr_col->GetColType()) {
                rhost.insert(pexpr_col->GetColumn().back()->GetId());
            } else  if (AstColumnRef::APP == pexpr_col->GetColType()) {
                rapp.insert(pexpr_col->GetColumn().back()->GetId());
            }
        } break;
        default: break;
    }
}

bool Policy::AnalyzeReference() {
    analyze_reference(_expr, _actions, _subjectattrs, _resourceattrs, _host, _app);
    analyze_reference(_pres_expr, _actions, _subjectattrs, _resourceattrs, _host, _app);
    //if(_actions.size() == 0 && _subjectattrs.size() == 0 && _resourceattrs.size() == 0 && _host.size() == 0 && _app.size() == 0) return  false;
    for (auto it: _obg_cols) {
        analyze_reference(it, _actions, _subjectattrs, _resourceattrs, _host, _app);
    }
    return  true;
}

void Policy::GetAction(std::set<std::string>& ractions) {
    ractions.insert(_actions.begin(), _actions.end());
}

void Policy::GetSubjectAttributes(std::set<std::string>& subjectattrs) {
    subjectattrs.insert(_subjectattrs.begin(), _subjectattrs.end());
}

void Policy::GetResourceAttributes(std::set<std::string>& resourceattrs) {
    resourceattrs.insert(_resourceattrs.begin(), _resourceattrs.end());
}

void Policy::GetHost(std::set<std::string>& host) {
    host.insert(_host.begin(), _host.end());
}
void Policy::GetApp(std::set<std::string>& app) {
    app.insert(_app.begin(), _app.end());
}


PolicyEngineReturn Policy::TryMatch(const Subject *subject, const std::string& action, const Resource *res, const Host *host, const App *app , Value::BOOLEAN& rboolean) {
    if (NULL == subject)  return  POLICY_ENGINE_FAIL;
//      rboolean = eval_expression(_expr, const_cast<Subject*>(subject), action.c_str());
        rboolean = eval_expression(_expr, const_cast<Subject*>(subject), action, const_cast<Resource*>(res), const_cast<Host*>(host), const_cast<App*>(app));
    return POLICY_ENGINE_SUCCESS;
}

std::string print_ids(const AstIds & ids) {
    std::string  ref;
    ref +="\"";
    for (unsigned int i = 0; i < ids.size(); ++i) {

        ref += ids[i]->GetId();
        if (i != ids.size()-1) {
            ref += "\".";
        }
        ref +="\"";

    }
    return  ref;
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
            AstColumnRef * ptemp = dynamic_cast<AstColumnRef *>(pexpr);
            std::string sid = print_ids(ptemp->GetColumn() );
            if (ptemp->GetColType()==AstColumnRef::ACTION) {
                printf("|-COLUMN_REF-%s-\"%s\"\n", "ACTION", sid.c_str());
            } else if (ptemp->GetColType()==AstColumnRef::SUB) {
                printf("|-COLUMN_REF-%s-\"%s\"\n", "SUBJECT", sid.c_str());
            } else {
                printf("|-COLUMN_REF-%s-\"%s\"\n", "RESOURCE", sid.c_str());
            }
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
            printf("|-C_STRING-'%s'\n", dynamic_cast<AstConstantValue *>(pexpr)->GetValueAsStr());
        }break;
        case AstExpr::C_PATTERN: {
            printf("|-C_PATTERN-'%s'\n", dynamic_cast<AstConstantValue *>(pexpr)->GetValueAsStr());
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
    printf("--------------ACTION/SUBJECT/ADVANCE----------------\n");
    print(_expr, 1);
    printf("--------------------RESOURCE------------------------\n");
    print(_pres_expr, 1);
}

