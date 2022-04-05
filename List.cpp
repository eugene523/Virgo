#include <cassert>
#include "List.h"
#include "Type.h"
#include "Args.h"
#include "Int.h"
#include "Str.h"
#include "Invoker.h"
#include "Err.h"
#include "ErrorMessages.h"
#include "VM.h"

Ref List_OpAdd(Ref a, Ref b) {
    Obj * a_obj = GET_OBJ(a);
    assert(a_obj->Is(List::t));

    Obj * b_obj = GET_OBJ(b);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    if (!b_obj->Is(List::t))
        return NEW_REF(new Err(ERR_OP_WRONG_TYPE));

    auto a_elems = ((List*)a_obj)->elements;
    auto b_elems = ((List*)b_obj)->elements;

    List * result = new List();

    for (auto a_elem : a_elems)
        result->elements.push_back(a_elem);

    for (auto b_elem : b_elems)
        result->elements.push_back(b_elem);

    return NEW_REF(result);
}

Ref List_Get(Ref a, Ref argsRef) {
    Obj * a_obj = GET_OBJ(a);
    assert(a_obj->Is(List::t));
    List * list = (List*)a_obj;

    Obj * b_obj = GET_OBJ(argsRef);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    assert(b_obj->Is(Args::t));
    auto * argsObj = (Args*)b_obj;
    Ref indexRef = argsObj->Get(0);
    Obj * indexObj = GET_OBJ(indexRef);
    if (indexObj->Is(Int::t)) {
        Int * index = (Int*)indexObj;
        if (index->val < 0 || index->val >= list->elements.size())
            return NEW_REF(new Err("Index is out of range."));
        return list->elements[index->val];
    }
    return NEW_REF(new Err("Index must be a positive integer."));
}

Ref List_Set(Ref a, Ref argsRef, Ref val) {
    Obj * a_obj = GET_OBJ(a);
    assert(a_obj->Is(List::t));
    List * list = (List*)a_obj;

    Obj * b_obj = GET_OBJ(argsRef);
    if (b_obj == nullptr)
        return NEW_REF(new Err(ERR_SECOND_ARG_IS_NULL));

    assert(b_obj->Is(Args::t));
    auto * argsObj = (Args*)b_obj;
    Ref indexRef = argsObj->Get(0);
    Obj * indexObj = GET_OBJ(indexRef);
    if (indexObj->Is(Int::t)) {
        Int * index = (Int*)indexObj;
        if (index->val < 0 || index->val >= list->elements.size())
            return NEW_REF(new Err("Index is out of range."));
        list->elements[index->val] = val;
        return Ref::none;
    }
    return NEW_REF(new Err("Index must be a positive integer."));
}

Ref List_GetField(Ref self, Ref fieldName) {
    Obj * selfObj = GET_OBJ(self);
    assert(selfObj->Is(List::t));
    List * list = (List*)selfObj;
    if (list->fields.count(fieldName) != 0)
        return list->fields.at(fieldName);

    std::stringstream s;
    Str * name = (Str*)(GET_OBJ(fieldName));
    s << "Object of type '" << selfObj->type->name
      << "does not contain '" << name->val << "' field or method.";
    return NEW_REF(new Err(s.str()));
}

Ref List_Method_Add(Obj * self, Ref args) {
    auto * listObj = (List*)self;
    auto * argsObj = (Args*)GET_OBJ(args);
    if (argsObj->NumOfArguments() == 0)
        return NEW_REF(new Err("No arguments provided for 'Add' method of the list object."));
    listObj->elements.push_back(argsObj->Get(0));
    return Ref::none;
}

///////////////////////////////////////////////////////////////////////////////

Type * List::t;

void List::InitType() {
    List::t = new Type("list");
    auto ot = List::t->opTable;
    ot->OpAdd    = &List_OpAdd;
    ot->Get      = &List_Get;
    ot->Set      = &List_Set;
    ot->GetField = &List_GetField;
}

List::List() : Obj{List::t} {
    Ref name = VM::GetConstant_Str("Add");
    Ref Add_Method_Invoker = NEW_REF(new Invoker(this, &List_Method_Add));
    fields[name] = Add_Method_Invoker;
}

void List::Mark() {
    for (auto r : elements) {
        Mem::MarkRef(r);
        Obj * obj = GET_OBJ(r);
        obj->Mark();
    }

    for (auto const & [key, val] : fields) {
        Mem::MarkRef(val);
        Obj * obj = GET_OBJ(val);
        obj->Mark();
    }
}
