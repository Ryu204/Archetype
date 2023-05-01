#ifndef ARCHETYPE_MACRO_HPP
#define ARCHETYPE_MACRO_HPP

/*
* ARCHRTYPE_DLL         :       Define if building library from source
* ARCHETYPE_DEBUG       :       Should be defined if built / used in debug mode;
*                               Once defined, the library requires linking to "Dbghelp" when built with MSVC
*/
#include <cassert>
#include <iostream>

#ifdef ARCHETYPE_DLL
    #define ARCHETYPE_API __declspec(dllexport)
#else
    #define ARCHETYPE_API __declspec(dllimport)
#endif // ARCHETYPE_DLL

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#ifdef ARCHETYPE_DEBUG
#include "Backtrack.hpp"
// if not X then write Y and exit
#define ECS_ASSERT(X, Y){\
if (X) \
{} \
else \
{\
    std::cerr << "ECS: Assertion: " << Y << '\n';\
    std::cerr << "In function: " << __PRETTY_FUNCTION__ << '\n';\
    std::cerr << "At: " << __FILE__ << ", line " << __LINE__ << '\n';\
    std::cerr << "Last called function(s):\n";\
    printStackTrace();\
    exit(-1);\
}\
}
#else
#define ECS_ASSERT(X, Y) {}
#endif // ARCHETYPE_DEBUG

#endif // ARCHETYPE_MACRO_HPP
