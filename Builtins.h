#ifndef PROTON_BUILTINS_H
#define PROTON_BUILTINS_H

#include <map>
#include <iostream>
#include <cmath>
#include "Fun.h"
#include "Utils.h"
#include "Obj.h"
#include "Mem.h"
#include "NFun.h"
#include "Real.h"

namespace Builtins {

class Assert : public NFun {
public:
    Assert();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class Print : public NFun {
public:
    Print();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class Near : public NFun {
    static v_real Get_p_flt(Ref numObj);
public:
    Near();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class Random : public NFun {
public:
    Random();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class Len : public NFun {
public:
    Len();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class CSeg : public NFun {
public:
    CSeg();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class VmStat : public NFun {
public:
    VmStat();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class CtxStat : public NFun {
public:
    CtxStat();
    Ref Execute(Ref args) override;
};

///////////////////////////////////////////////////////////////////////////////

class ZeroNamespace {
    static std::map<Ref, Ref> builtins;
public:
    static void Init();
    static void CreateBuiltinObj(const std::string & name, Obj * obj);
    static Ref Get(Ref name);
};
}

#endif //PROTON_BUILTINS_H
