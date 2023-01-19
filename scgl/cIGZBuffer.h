#pragma once
#include <cIGZUnknown.h>

static const uint32_t GZIID_cIGZBuffer = 0x20732180;

struct cRZRect {
	int x, y, w, h;
};

class cIGZBuffer : public cIGZUnknown
{
public:
	virtual uint32_t Init(uint32_t, uint32_t, uint32_t, uint32_t) = 0;
	virtual uint32_t Shutdown() = 0;
	virtual uint32_t Uninitialize() = 0;
	virtual uint32_t Lock(uint32_t) = 0;
	virtual uint32_t Unlock(uint32_t) = 0;
	virtual bool IsLocked() = 0;
	virtual uint32_t Width() = 0;
	virtual uint32_t Height() = 0;
	virtual uint32_t GetBufferArea() = 0;
	virtual uint32_t GetBufferArea(void* rect) = 0;
	virtual uint32_t GetMemoryUsage() = 0;
	virtual uint32_t GetColorType() = 0;
	virtual uint32_t GetColorType(void*) = 0;
	virtual uint32_t GetBitsPerPixel() = 0;
	virtual uint32_t GetBytesPerPixel() = 0;
	virtual uint32_t Fill(uint32_t, const cRZRect*, cRZRect*) = 0;
	virtual uint32_t Fill(uint32_t, int32_t, int32_t, int32_t, int32_t, cRZRect*) = 0;
	virtual uint32_t Fill(uint32_t, cRZRect*) = 0;
	virtual uint32_t GetPixel(uint32_t, uint32_t) = 0;
	virtual uint32_t SetPixel(uint32_t, uint32_t, uint32_t) = 0;
	virtual uint32_t SetTransparency(uint32_t) = 0;
	virtual uint32_t GetTransparency() = 0;
	virtual bool IsPixelTransparent(uint32_t, uint32_t) = 0;
	virtual bool GetTransparentColor(uint32_t&) = 0;
	virtual void RemoveTransparency() = 0;
	virtual uint32_t DrawLine(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t) = 0;
	virtual uint32_t Blt(cIGZBuffer*, const cRZRect*, const cRZRect*, const cRZRect*) = 0;
	virtual uint32_t ConvertRGBValueToNative(uint8_t, uint8_t, uint8_t) = 0;
	virtual uint32_t ConvertNativeValueToRGB(uint32_t, uint8_t&, uint8_t&, uint8_t&) = 0;
	virtual uint32_t ConvertRGBAValueToNative(uint8_t, uint8_t, uint8_t, uint8_t) = 0;
	virtual uint32_t ConvertNativeValueToRGBA(uint32_t, uint8_t&, uint8_t&, uint8_t&, uint8_t&) = 0;
	virtual uint32_t GetColorSurfaceBits() = 0;
	virtual uint32_t GetColorSurfaceStride() = 0;
	virtual uint32_t GetChangeCounter() = 0;
	virtual uint32_t GetHardwareCache() = 0;
	virtual uint32_t SetHardwareCache(void*) = 0;
	virtual bool IsReady() = 0;
	virtual ~cIGZBuffer() = 0;

	// TODO: there are virtual functions below the destructors, and I'm not positive about their offsets.


};