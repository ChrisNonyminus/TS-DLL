
#include <cstdint>
#include <stdio.h>
#include <Windows.h>
#include "cTest.h"


bool cTest::QueryInterface(uint32_t riid, void **ppvObj) {
    if (riid == 1) {
        MessageBoxA(NULL, "cTest::QueryInterface", "cTest::QueryInterface", MB_OK);
        *ppvObj = static_cast<cIGZUnknown *>(this);
        AddRef();
        return true;
    }
    return false;
}

uint32_t cTest::AddRef(void) {
    return ++refCount;
}

uint32_t cTest::Release(void) {
    if (refCount < 2) {
        delete this;
        return 0;
    }
    else {
        return --refCount;
    }
}

cTest::cTest() {
    MessageBoxA(NULL, "cTest::cTest", "cTest::cTest", MB_OK);
}

cTestDirector::cTestDirector() {
    MessageBoxA(NULL, "cTestDirector::cTestDirector", "cTestDirector::cTestDirector", MB_OK);
    AddCls(GZCLSID_cTest(), cTestDirector::CreateTest);



}

uint32_t cTestDirector::GetDirectorID() const {
    return GZCLSID_cTestDirector;
}

cTestDirector::~cTestDirector() = default;

cIGZUnknown *cTestDirector::CreateTest() {
    MessageBoxA(NULL, "cTestDirector::CreateTest", "cTestDirector::CreateTest", MB_OK);
    return new cTest();
}

cRZCOMDllDirector *RZGetCOMDllDirector() {
    return new cTestDirector();
}
