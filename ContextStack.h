#ifndef PROTON_CONTEXTSTACK_H
#define PROTON_CONTEXTSTACK_H

#include <vector>
#include <iostream>
#include "Context_.h"

struct ContextStack {
    std::vector<Context*> contextStack;

    Context * Create();
    void Push(Context * c);
    Context * Last();
    void Pop();
    void Mark();
    void Print();
};

#endif //PROTON_CONTEXTSTACK_H
