#pragma once

#include <map>
#include <vector>
#include "cIGZCOMDirector.h"
#include "cIGZFrameWorkHooks.h"
#include "cIGZFrameWork.h"
#include "cRZString.h"

typedef uint32_t GZGUID;
typedef uint32_t GZIID;
typedef uint32_t GZCLSID;

class cRZCOMDllDirector : public cIGZCOMDirector
{
public:
    cRZCOMDllDirector();
    virtual ~cRZCOMDllDirector();

public:
    virtual bool QueryInterface(GZIID iid, void** outPtr);
    virtual uint32_t AddRef();
    virtual uint32_t Release();

public:
    typedef void (*DummyFunctionPtr)();
    typedef cIGZUnknown* (*FactoryFunctionPtr1)();
    typedef bool (*FactoryFunctionPtr2)(uint32_t, void**);

    virtual bool InitializeCOM(cIGZCOM* com, const cIGZString& libraryPath);
    virtual bool OnStart(cIGZCOM* com);

    virtual void EnumClassObjects(ClassObjectEnumerationCallback callback, void* context);
    virtual bool GetClassObject(GZGUID clsid, GZIID iid, void** outPtr);

    virtual bool CanUnloadNow(void);
    virtual bool OnUnload(void);

    virtual uint32_t RefCount(void);
    virtual uint32_t RemoveRef(void);

    virtual cIGZFrameWork* FrameWork(void);
    virtual cIGZCOM* GZCOM(void);

    virtual GZGUID GetDirectorID() const = 0;
    virtual bool GetLibraryPath(cIGZString& path);
    virtual uint32_t GetHeapAllocatedSize(void);

public:
    void AddCls(GZCLSID clsid, FactoryFunctionPtr1 ffp);
    void AddCls(GZCLSID clsid, FactoryFunctionPtr2 ffp);
    void AddDirector(cRZCOMDllDirector* director);

public:
    virtual bool PreFrameworkInit(void);
    virtual bool PreAppInit(void);
    virtual bool PostAppInit(void);
    virtual bool PreAppShutdown(void);
    virtual bool PostAppShutdown(void);
    virtual bool PostSystemServiceShutdown(void);
    virtual bool AbortiveQuit(void);
    virtual bool OnInstall(void);

protected:
    enum FactoryFunctionType
    {
        kFactoryFunctionType1 = 1,
        kFactoryFunctionType2 = 2,
    };

    typedef std::vector<cRZCOMDllDirector*> ChildDirectorArray;
    typedef std::pair<DummyFunctionPtr, FactoryFunctionType> FactoryFuncRecord;
    typedef std::map<GZCLSID, FactoryFuncRecord> ClassObjectMap;

    uint32_t refCount;
    cRZString libraryPath;
    cIGZFrameWork* framework;
    cIGZCOM* com;
    ChildDirectorArray childDirectors;
    ClassObjectMap classObjectMap;
};

cRZCOMDllDirector *RZGetCOMDllDirector();

inline cIGZFrameWork *RZGetFrameWork() { return RZGetCOMDllDirector()->FrameWork(); }

inline cIGZFrameWork *RZGetFramework() { return RZGetCOMDllDirector()->FrameWork(); }