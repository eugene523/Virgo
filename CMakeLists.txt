cmake_minimum_required(VERSION 3.15)
project(Virgo)

set(CMAKE_CXX_STANDARD 17)

add_executable(Virgo
        ErrorMessages.h ErrorMessages.cpp
        Type.h Type.cpp
        Obj.h Obj.cpp
        Mem.h Mem.cpp
        None.h None.cpp
        #[[
        Err.h Err.cpp
        Bool.h Bool.cpp
        Int.h Int.cpp
        Real.h Real.cpp
        Str.h Str.cpp
        List.h List.cpp
        Iter.h Iter.cpp
        Class.h Class.cpp
        Object.h Object.cpp
        IDefObj.h
        Skip.h Skip.cpp
        Break.h Break.cpp
        Return.h Return.cpp
        ArgPair.h ArgPair.cpp
        Args.h Args.cpp
        Seg.h Seg.cpp
        Expr.h Expr.cpp
        Fun.h Fun.cpp
        NFun.h NFun.cpp
        Invoker.h Invoker.cpp
        Builtins.h Builtins.cpp
        Script.h Script.cpp
        Context.h Context.cpp
        ContextStack.h ContextStack.cpp
        VM.h VM.cpp
        Tokenizer.h Tokenizer.cpp
        Parser.h Parser.cpp]]
        main.cpp)