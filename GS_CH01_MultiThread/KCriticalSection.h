/*
이 소스의 출처는 https://github.com/GP101/DiconServer/blob/master/prerequisite/Win32Thread/KCriticalSection.h 입니다.

*/


#pragma once
#include <boost/assert.hpp>
#include "KGen.h"

struct KCriticalSection : public CRITICAL_SECTION
{
    CONSTRUCTOR         KCriticalSection() { InitializeCriticalSection(this); }
    DESTRUCTOR          ~KCriticalSection() { DeleteCriticalSection(this); }
};

class KCriticalSectionLock
{
public:
    CONSTRUCTOR         KCriticalSectionLock(CRITICAL_SECTION& cs) : m_pcs(&cs)
    {
        //printf( "constructor\r\n" );
        BOOST_ASSERT(m_pcs != nullptr);
        EnterCriticalSection(m_pcs);
    }

    DESTRUCTOR          ~KCriticalSectionLock()
    {
        //printf( "destructor\r\n" );
        if (m_pcs != nullptr)
            LeaveCriticalSection(m_pcs);
    }
    explicit            operator bool() { return true; }
protected:
    CRITICAL_SECTION* m_pcs;
};

#define CSLOCK( cs_ )   if( KCriticalSectionLock CONCATENATE( lock_, __LINE__ ) = cs_ )