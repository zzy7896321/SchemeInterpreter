/** @file
 *  Section 6.4 Control features.
 */
#include "builtinproc.h"
#include <cassert>

namespace BuiltinProc{

callResultType procedure_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::PROC ||
                                param[0]->getType() == ObjectDef::ObjectType::BUILTIN);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType apply_func(parameterType param, sptrObject& obj){
    if (!ObjectDef::isList(param.back())){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "apply. Need last to be a list");
    }

    std::vector<sptrObject> param2(param.begin()+1, param.end()-1);
    sptrObject lst = param.back();
    while (lst->getType() == ObjectDef::ObjectType::PAIR){
        param2.push_back(ObjectDef::getCar(lst));
        lst = ObjectDef::getCdr(lst);
    }

    if (param[0]->getType() == ObjectDef::ObjectType::BUILTIN){
        return static_cast<ObjectDef::Builtin*>(param[0].get())->invoke(param2, obj);
    }
    else {  // PROC
        return static_cast<ObjectDef::Procedure*>(param[0].get())->invoke(param2, obj);
    }

    assert(false);
	return std::make_pair(Interp::EVAL_UNEXPECTED_SITUATION, "should never reach here");
}

callResultType map_func(parameterType param, sptrObject& obj){
    if (param[0] -> getType() == ObjectDef::ObjectType::BUILTIN){
        obj = sptrObject(new ObjectDef::Pair(ObjectDef::Void::getVoid(), ObjectDef::EmptyList::getEmptyList()));
        sptrObject lst_end = obj;
        bool not_empty = false;
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
        }
        while (not_empty){
            not_empty = false;
            std::vector<sptrObject> param2;
            for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
                if ((*iter)->getType() != ObjectDef::ObjectType::PAIR){
                    return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "map. length of lists do not equal.");
                }
                param2.push_back(ObjectDef::getCar(*iter));
                *iter = ObjectDef::getCdr(*iter);
                not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
            }

            sptrObject ret;
            auto callRes = static_cast<ObjectDef::Builtin*>(param[0].get())->invoke(param2, ret);
            if (callRes.first != Interp::EVAL_NO_ERROR) return std::move(callRes);
            ObjectDef::setCdr(lst_end, sptrObject(new ObjectDef::Pair(ret, ObjectDef::EmptyList::getEmptyList())));
            lst_end = ObjectDef::getCdr(lst_end);
        }
        obj = ObjectDef::getCdr(obj);
    }
    else{   //PROC
        obj = sptrObject(new ObjectDef::Pair(ObjectDef::Void::getVoid(), ObjectDef::EmptyList::getEmptyList()));
        sptrObject lst_end = obj;
        bool not_empty = false;
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
        }
        while (not_empty){
            not_empty = false;
            std::vector<sptrObject> param2;
            for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
                if ((*iter)->getType() != ObjectDef::ObjectType::PAIR){
                    return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "map. length of lists do not equal.");
                }
                param2.push_back(ObjectDef::getCar(*iter));
                *iter = ObjectDef::getCdr(*iter);
                not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
            }

            sptrObject ret;
            auto callRes = static_cast<ObjectDef::Procedure*>(param[0].get())->invoke(param2, ret);
            if (callRes.first != Interp::EVAL_NO_ERROR) return std::move(callRes);
            ObjectDef::setCdr(lst_end, sptrObject(new ObjectDef::Pair(ret, ObjectDef::EmptyList::getEmptyList())));
            lst_end = ObjectDef::getCdr(lst_end);
        }
        obj = ObjectDef::getCdr(obj);
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType for_each_func(parameterType param, sptrObject& obj){
    if (param[0] -> getType() == ObjectDef::ObjectType::BUILTIN){
        bool not_empty = false;
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
        }
        while (not_empty){
            not_empty = false;
            std::vector<sptrObject> param2;
            for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
                if ((*iter)->getType() != ObjectDef::ObjectType::PAIR){
                    return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "map. length of lists do not equal.");
                }
                param2.push_back(ObjectDef::getCar(*iter));
                *iter = ObjectDef::getCdr(*iter);
                not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
            }

            auto callRes = static_cast<ObjectDef::Builtin*>(param[0].get())->invoke(param2, obj);
            if (callRes.first != Interp::EVAL_NO_ERROR) return std::move(callRes);
        }
    }
    else{   //PROC
        bool not_empty = false;
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
        }
        while (not_empty){
            not_empty = false;
            std::vector<sptrObject> param2;
            for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
                if ((*iter)->getType() != ObjectDef::ObjectType::PAIR){
                    return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "map. length of lists do not equal.");
                }
                param2.push_back(ObjectDef::getCar(*iter));
                *iter = ObjectDef::getCdr(*iter);
                not_empty = not_empty || ((*iter)->getType() == ObjectDef::ObjectType::PAIR);
            }

            auto callRes = static_cast<ObjectDef::Procedure*>(param[0].get())->invoke(param2, obj);
            if (callRes.first != Interp::EVAL_NO_ERROR) return std::move(callRes);
        }
    }

    obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType force_func(parameterType param, sptrObject& obj){
	return static_cast<ObjectDef::Promise*>(param[0].get())->force(obj);
}

}

