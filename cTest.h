#pragma once
#include "vendor/framework/include/cIGZUnknown.h"
class cTest :
    public cIGZUnknown
{
public:
    cTest();

    bool QueryInterface(uint32_t riid, void **ppvObj) override;

    uint32_t AddRef(void) override;

    uint32_t Release(void) override;

private:
    uint32_t refCount{0};
};

static const uint32_t GZIID_cTest = 0x1c68ea3e;

static inline uint32_t GZCLSID_cTest() {
    return 0x0a233cba;
}



#include "vendor/framework/include/cRZCOMDllDirector.h"

class cTestDirector :
    public cRZCOMDllDirector
{
public:
    cTestDirector();
    virtual uint32_t GetDirectorID() const;
    ~cTestDirector() override;

    static cIGZUnknown* CreateTest();

};

static const uint32_t GZCLSID_cTestDirector = 0x38087083;