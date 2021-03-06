/*
    이 소스의 출처는 https://github.com/GP101/DiconServer/blob/master/prerequisite/Win32Thread/KGen.h 입니다.
*/

#pragma once
#define _WINSOCKAPI_
#include <windows.h>
#include <cassert>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#ifndef COMMA
#define COMMA()         ,
#endif

#ifndef VA_ARGS
#define VA_ARGS( ... )  __VA_ARGS__
#endif

// to expand macro paramger
#ifndef EXPAND
#define EXPAND( x )     x
#endif

// convert identifier to string
#ifndef STRINGIZE
#define STRINGIZE( arg_ )    _STRINGIZE1( arg_ )
#define _STRINGIZE1( arg_ )   _STRINGIZE2( arg_ )
#define _STRINGIZE2( arg_ )   #arg_
#endif

#ifndef CONCATENATE
#define CONCATENATE( arg1, arg2 )   _CONCATENATE1( arg1, arg2 )
#define _CONCATENATE1( arg1, arg2 )  _CONCATENATE2( arg1, arg2 )
#define _CONCATENATE2( arg1, arg2 )  arg1##arg2
#endif

#define STRING_CAT(a, ...)          _STRING_CAT1(a, __VA_ARGS__)
#define _STRING_CAT1(a, ...)        _STRING_CAT2(a, __VA_ARGS__)
#define _STRING_CAT2(a, ...)        a ## __VA_ARGS__

#define DUMMY_STATEMENT     CONCATENATE( unsigned int _DUMMY, __LINE__ ) = 0; \
    CONCATENATE( _DUMMY, __LINE__ ) += 1;

// get number of elements in array
#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a_ )    ( sizeof( a_ ) / sizeof( ( a_ )[0] ) )
#endif

// get string of an identifier
#ifndef VAR_NAME
#define VAR_NAME( p )   ( #p )
#endif

#ifndef VAR_WNAME
#define VAR_WNAME( p )  ( L#p )
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef REF
#define REF
#endif

#ifndef CONSTRUCTOR
#define CONSTRUCTOR
#endif

#ifndef DESTRUCTOR
#define DESTRUCTOR
#endif

#ifndef PRIVATE
#define PRIVATE
#endif

#ifndef PROTECTED
#define PROTECTED
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#ifndef VIRTUAL
#define VIRTUAL
#endif

#ifndef STATIC
#define STATIC
#endif

// compile time assertion
// CASSERT( sizeof( int ) == 4 );
#ifndef CASSERT
#define CASSERT( expression )       typedef char __C_ASSERT__[( expression)?1:-1]
#endif

#define TOWIDESTRING( narrowstring_ )   _TOWIDESTRING1( narrowstring_ )
#define _TOWIDESTRING1( narrowstring_ )   L##narrowstring_

#define __WFUNCTION__   TOWIDESTRING(__FUNCTION__)

#define BEGIN_LOG( tag, header ) \
    std::wcout << L#tag \
    << L" " \
    << (LPCTSTR)CTime::GetCurrentTime( ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) \
    << L" : " << __WFUNCTION__ << L"(): " \
    << header << std::endl

#define LOG_NAMEVALUE( name ) \
    L#name << L" : " << name << std::endl

#define END_LOG         std::endl;

template<typename F>
struct ScopeExit
{
    ScopeExit(F f) : f(f) {}
    ~ScopeExit() { f(); }
    explicit operator bool() { return true; }
    F f;
};

template<typename F>
ScopeExit<F> MakeScopeExit(F f)
{
    return ScopeExit<F>(f);
};

#define SCOPE_EXIT( code )      auto CONCATENATE( scope_exit_, __LINE__ ) = MakeScopeExit( [=]() { code; } )
#define PREBLOCK( code )        if( SCOPE_EXIT( code ) )