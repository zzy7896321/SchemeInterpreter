#ifndef SCM_LIST_H_INCLUDED
#define SCM_LIST_H_INCLUDED

#include "object.h"
#include "pair.h"

#include <memory>

namespace ObjectDef{

using std::shared_ptr;

bool isList(shared_ptr<Object> obj);

inline shared_ptr<Object> getDummyList(){
	static shared_ptr<Object> dummyList(new Object(ObjectType::LIST));
	return dummyList;
}


}

#endif	//SCM_LIST_H_INCLUDED
