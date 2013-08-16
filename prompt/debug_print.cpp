#include "prompt_declaration.h"

    #include <iostream>
    #include <string>
    #include <memory>
    #include "../object/object.h"

    void printParseTree_actual(std::shared_ptr<ParserEnv::ParseTree> root, int cur_cnt, int& node_cnt, std::ostream& output){
        using std::endl;
        if (!root) return;
        int tcnt = (node_cnt += 2);
        output << "[node "<< cur_cnt << "] " << (void*)(&root) << endl;
        output << "\tobject ptr = " << ((void*)(root->obj.get())) << endl;
        if (root->obj){
            output << "\tobject type = "; printType(root->obj->getType()); output << endl;
            output << "\tobject extrep = " << root->obj->getExternalRep() << endl;
        }
        output << "\tchild = " << tcnt-1 << ", " << ((void*)(root->child.get())) << endl;
        output << "\tnext = " << tcnt << ", " << ((void*)(root->next.get())) << endl;
        printParseTree_actual(root->child, tcnt - 1, node_cnt, output);
        printParseTree_actual(root->next, tcnt, node_cnt, output);
    }

    void printParseTree(std::shared_ptr<ParserEnv::ParseTree> root, std::ostream& output){
        int node_cnt = 0;
        printParseTree_actual(root, node_cnt, node_cnt, output);
    }

    void printObject(std::shared_ptr<ObjectDef::Object> obj, std::ostream& output){
        using std::endl;
        output << "Object type = "; printType(obj->getType());
        output << " extRep = " << obj->getExternalRep() << endl;
    }

    void printType(ObjectDef::ObjectType type, std::ostream& output){
        using std::endl;
        using ObjectDef::ObjectType;
        switch (type){
        case ObjectType::OBJECT:
            output << "OBJECT";
            break;

        case ObjectType::BOOLEAN:
            output << "BOOLEAN";
            break;

        case ObjectType::INT:
            output << "INT";
            break;

        case ObjectType::BIGINT:
            output << "BIGINT";
            break;

        case ObjectType::RATIONAL:
            output << "RATIONAL";
            break;

        case ObjectType::REAL:
            output << "REAL";
            break;

        case ObjectType::COMPLEX:
            output << "COMPLEX";
            break;

        case ObjectType::PROC:
            output << "PROC";
            break;

        case ObjectType::BUILTIN:
            output << "BUILTIN";
            break;

        case ObjectType::PAIR:
            output << "PAIR";
            break;

        case ObjectType::LIST:
            output << "LIST";
            break;

        case ObjectType::SYMBOL:
            output << "SYMBOL";
            break;

        case ObjectType::CHAR:
            output << "CHAR";
            break;

        case ObjectType::STRING:
            output << "STRING";
            break;

        case ObjectType::VECTOR:
            output << "VECTOR";
            break;

        case ObjectType::EMPTYLIST:
            output << "EMPTYLIST";
            break;

        case ObjectType::PAIRDOT:
            output << "PAIRDOT";
            break;
        case ObjectType::ENVIRONMENT:
            output << "ENVIRONMENT";
            break;
        case ObjectType::VOID:
            output << "VOID";
            break;
        case ObjectType::PROMISE:
            output << "PROMISE";
            break;
        case ObjectType::EXPRESSIONPACK:
            output << "EXPRESSIONPACK";
            break;
        }
    }
