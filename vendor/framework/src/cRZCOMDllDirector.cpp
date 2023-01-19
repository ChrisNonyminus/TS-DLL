#include "../include/cIGZCOM.h"
#include "../include/cIGZString.h"
#include "../include/cRZCOMDllDirector.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>

#if defined(_WIN32)
#define EXPORT __declspec(dllexport)
#elif defined(__APPLE__)
#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" EXPORT cIGZCOMDirector* GZDllGetGZCOMDirector(void) {
	return static_cast<cIGZCOMDirector*>(RZGetCOMDllDirector());
}

cIGZCOM* GZCOM(void) {
	return RZGetCOMDllDirector()->GZCOM();
}


cRZCOMDllDirector::cRZCOMDllDirector()
{
}

cRZCOMDllDirector::~cRZCOMDllDirector()
{
}

bool cRZCOMDllDirector::QueryInterface(GZIID iid, void** outPtr)
{
    switch (iid)
    {
        case /*GZIID_cIGZCOMDirector*/0xA21EE941:
            *outPtr = static_cast<cIGZCOMDirector*>(this);
            AddRef();
            return true;


        case GZIID_cIGZUnknown:
            *outPtr = static_cast<cIGZUnknown*>(static_cast<cIGZCOMDirector*>(this));
            AddRef();
            return true;
    }

    return false;
}

uint32_t cRZCOMDllDirector::AddRef()
{
    return ++refCount;
}

uint32_t cRZCOMDllDirector::Release()
{
    return RemoveRef();
}

bool cRZCOMDllDirector::InitializeCOM(cIGZCOM* com, const cIGZString& libraryPath)
{
    if (com != NULL)
    {
        this->com = com;
        this->framework = com->FrameWork();
        this->libraryPath = libraryPath;

        for (ChildDirectorArray::iterator it = childDirectors.begin(); it != childDirectors.end(); ++it)
        {
            cRZCOMDllDirector* const director = *it;
            director->InitializeCOM(com, libraryPath);
        }
    }

    return false;
}

bool cRZCOMDllDirector::OnStart(cIGZCOM* pCOM)
{
    return true;
}

void cRZCOMDllDirector::EnumClassObjects(ClassObjectEnumerationCallback callback, void* context)
{
    for (ChildDirectorArray::iterator it = childDirectors.begin(); it != childDirectors.end(); ++it)
    {
        cRZCOMDllDirector* const director = *it;
        director->EnumClassObjects(callback, context);
    }

    for (ClassObjectMap::iterator it2 = classObjectMap.begin(); it2 != classObjectMap.end(); ++it2)
    {
        uint32_t clsid = (*it2).first;
        callback(clsid, 0, context);
    }
}

bool cRZCOMDllDirector::GetClassObject(GZGUID clsid, GZIID iid, void** outPtr)
{
    for (ChildDirectorArray::iterator it = childDirectors.begin(); it != childDirectors.end(); ++it)
    {
        cRZCOMDllDirector* const director = *it;
        if (director->GetClassObject(clsid, iid, outPtr))
        {
            return true;
        }
    }

    ClassObjectMap::iterator it2 = classObjectMap.find(clsid);
    if (it2 != classObjectMap.end())
    {
        FactoryFuncRecord& ffr = (*it2).second;
        if (ffr.second == kFactoryFunctionType1)
        {
            FactoryFunctionPtr1 const ffp1 = reinterpret_cast<FactoryFunctionPtr1>(ffr.first);
            cIGZUnknown* const newObj = ffp1();

            if (newObj->QueryInterface(iid, outPtr))
            {
                return true;
            }

            newObj->Release();
        }
        else if (ffr.second == kFactoryFunctionType2)
        {
            FactoryFunctionPtr2 ffp2 = reinterpret_cast<FactoryFunctionPtr2>(ffr.first);
            return ffp2(iid, outPtr);
        }
        else
        {
            assert(false);
        }
    }

    return false;
}

bool cRZCOMDllDirector::CanUnloadNow(void)
{
    if (refCount == 0)
    {
        for (ChildDirectorArray::iterator it = childDirectors.begin(); it != childDirectors.end(); ++it)
        {
            cRZCOMDllDirector* const director = *it;
            if (!director->CanUnloadNow())
            {
                return false;
            }
        }
    }

    return true;
}

bool cRZCOMDllDirector::OnUnload(void)
{
    return true;
}

uint32_t cRZCOMDllDirector::RefCount(void)
{
    return refCount;
}

uint32_t cRZCOMDllDirector::RemoveRef(void)
{
    if (refCount > 0)
    {
        --refCount;
    }

    return refCount;
}

cIGZFrameWork* cRZCOMDllDirector::FrameWork(void)
{
    return framework;
}

cIGZCOM* cRZCOMDllDirector::GZCOM(void)
{
    return com;
}

bool cRZCOMDllDirector::GetLibraryPath(cIGZString& path)
{
    path = this->libraryPath;
    return true;
}

uint32_t cRZCOMDllDirector::GetHeapAllocatedSize(void)
{
    return 0;
}

void cRZCOMDllDirector::AddCls(GZCLSID clsid, FactoryFunctionPtr1 ffp1)
{
    DummyFunctionPtr ffp = reinterpret_cast<DummyFunctionPtr>(ffp1);
    FactoryFuncRecord ffr(ffp, kFactoryFunctionType1);

    classObjectMap.insert(std::pair<GZCLSID, FactoryFuncRecord>(clsid, ffr));
}

void cRZCOMDllDirector::AddCls(GZCLSID clsid, FactoryFunctionPtr2 ffp2)
{
    DummyFunctionPtr ffp = reinterpret_cast<DummyFunctionPtr>(ffp2);
    FactoryFuncRecord ffr(ffp, kFactoryFunctionType2);

    classObjectMap.insert(std::pair<GZCLSID, FactoryFuncRecord>(clsid, ffr));
}

void cRZCOMDllDirector::AddDirector(cRZCOMDllDirector* director)
{
    director->InitializeCOM(GZCOM(), libraryPath);

    for (ChildDirectorArray::iterator it = director->childDirectors.begin(); it != director->childDirectors.end(); ++it)
    {
        cRZCOMDllDirector* const childDirector = *it;
        AddDirector(childDirector);
    }

    childDirectors.push_back(director);
}

bool cRZCOMDllDirector::PreFrameworkInit(void)
{
    return true;
}

bool cRZCOMDllDirector::PreAppInit(void)
{
    return true;
}

bool cRZCOMDllDirector::PostAppInit(void)
{
    return true;
}

bool cRZCOMDllDirector::PreAppShutdown(void)
{
    return true;
}

bool cRZCOMDllDirector::PostAppShutdown(void)
{
    return true;
}

bool cRZCOMDllDirector::PostSystemServiceShutdown(void)
{
    return true;
}

bool cRZCOMDllDirector::AbortiveQuit(void)
{
    return true;
}

bool cRZCOMDllDirector::OnInstall(void)
{
    return true;
}