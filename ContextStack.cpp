#include <cassert>
#include "ContextStack.h"

Context * ContextStack::Create() {
    auto * c = new Context();
    contextStack.push_back(c);
    return c;
}

void ContextStack::Push(Context * c) {
    contextStack.push_back(c);
}

Context * ContextStack::Last() {
    assert(!contextStack.empty());
    return contextStack.back();
}

void ContextStack::Pop() {
    assert(!contextStack.empty());

    Context * c = contextStack.back();
    delete c;
    contextStack.pop_back();
}

void ContextStack::Mark() {
    for (auto i : contextStack) {
        i->Mark();
    }
}

void ContextStack::Print() {
    std::cout << '\n';
    for (std::size_t i = 0; i < contextStack.size(); i++) {
        std::cout << "Context " << i << ":" << std::endl;
        contextStack[i]->Print();
    }
}
