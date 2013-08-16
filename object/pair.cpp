#include "pair.h"
#include <map>
#include <stack>
#include <sstream>

namespace ObjectDef{

std::string Pair::getExternalRep() const{
	std::ostringstream os;
	std::map<const Object*, int> dfsTree;
	std::stack<const Object*> father;
	int tot = 0;

	const Object* cur = this;
	os << '(';
	while (cur || !father.empty()){
        if (cur){
          //std::cout << "push " << (void*)(cur) << " car = " << (void*)(static_cast<const Pair*>(cur)->car.get()) << " cdr = " << (void*)(static_cast<const Pair*>(cur)->cdr.get()) << std::endl;
            father.push(cur);
            dfsTree.insert({cur, tot++});
            if (static_cast<const Pair*>(cur)->car->getType() == ObjectDef::ObjectType::PAIR){
                auto iter = dfsTree.find(static_cast<const Pair*>(cur)->car.get());
                if (iter != dfsTree.end()){
                    os << "<circular>#" << iter->second;
                    cur = nullptr;
                }
                else {
                    os << '(';
                    cur = static_cast<const Pair*>(cur)->car.get();
                }
            }
            else {
                os << static_cast<const Pair*>(cur)->car->getExternalRep();
                cur = nullptr;
            }
        }

        else {
            cur = father.top();
            //std::cout << "pop " << (void*)(cur) << " car = " << (void*)(static_cast<const Pair*>(cur)->car.get()) << " cdr = " << (void*)(static_cast<const Pair*>(cur)->cdr.get()) << std::endl;

            if (static_cast<const Pair*>(cur)->cdr->getType() == ObjectDef::ObjectType::PAIR){
                auto iter = dfsTree.find(static_cast<const Pair*>(cur)->cdr.get());
                if (iter != dfsTree.end()){
                    os << " . <circular>#" << iter->second << ')';
                    cur = nullptr;
                }
                else {
                    os << ' ';
                    cur = static_cast<const Pair*>(cur)->cdr.get();
                }
            }
            else if (static_cast<const Pair*>(cur)->cdr->getType() == ObjectDef::ObjectType::EMPTYLIST){
                os << ')';
                cur = nullptr;
            }
            else {
                os << " . " << static_cast<const Pair*>(cur)->cdr->getExternalRep() << ')';
                cur = nullptr;
            }
            father.pop();
        }
	}

	return os.str();
}

bool pairEqual(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return obj1->getExternalRep() == obj2->getExternalRep();
}

}
