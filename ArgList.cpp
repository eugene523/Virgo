#include "ArgList.h"
#include "Type.h"

Type * ArgList::t;

void ArgList::InitType() {
    t = new Type("ArgList");
}

ArgList::ArgList() : Obj{ArgList::t} {}

ArgList::~ArgList() = default;

void ArgList::Add(Ref arg) {
    args.push_back(arg);
}

uint ArgList::NumOfArguments() {
    return args.size();
}

Ref ArgList::Get(uint index) {
    return args.at(index);
}

void ArgList::Mark() {
    for (auto i : args)
        Mem::MarkRef(i);
}