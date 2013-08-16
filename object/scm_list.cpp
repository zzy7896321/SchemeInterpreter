#include "scm_list.h"

#include <memory>
#include <set>

namespace ObjectDef{

bool isList(shared_ptr<Object> obj){
    std::set<Object*> occurence;
    Object* cur = obj.get();

    while (cur->getType() == ObjectType::PAIR){
        if (occurence.find(cur) != occurence.end()){
            return false;
        }

        occurence.insert(cur);
        cur = static_cast<Pair*>(cur)->getCdr().get();
    }

    return cur->getType() == ObjectType::EMPTYLIST;
}

}
