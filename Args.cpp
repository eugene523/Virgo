#include "Args.h"
#include "Type.h"

Type * Args::t;

void Args::InitType() {
    t = new Type("Args");
}

Args::Args() : Obj{Args::t} {}

Args::~Args() = default;

void Args::Add(Ref arg) {
    args.push_back(arg);
}

uint Args::NumOfArguments() {
    return args.size();
}

Ref Args::Get(uint index) {
    return args.at(index);
}

void Args::Mark() {
    for (auto i : args)
        Mem::MarkRef(i);
}