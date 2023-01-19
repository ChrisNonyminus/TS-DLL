#include "../cGDriver.h"

namespace nSCGL
{
	char const* cGDriver::GetVertexBufferName(uint32_t gdVertexFormat) {
		//NOTIMPL();
		PRINT_VALUES(gdVertexFormat);

		return nullptr;
	}

	uint32_t cGDriver::VertexBufferType(uint32_t a) {
		//NOTIMPL();
		PRINT_VALUES(a);

		return 0;
	}

	uint32_t cGDriver::MaxVertices(uint32_t a) {
		//NOTIMPL();
		PRINT_VALUES(a);
		return 0;
	}

	uint32_t cGDriver::GetVertices(int32_t a, bool b) {
		//NOTIMPL();
		PRINT_VALUES(a, b);
		return 0;
	}

	uint32_t cGDriver::ContinueVertices(uint32_t a, uint32_t b) {
		//NOTIMPL();
		PRINT_VALUES(a, b);
		return 0;
	}

	void cGDriver::ReleaseVertices(uint32_t a) {
		//NOTIMPL();
		PRINT_VALUES(a);
	}

	void cGDriver::DrawPrims(uint32_t a, uint32_t gdPrimType, void* b, uint32_t c) {
		//NOTIMPL();
		PRINT_VALUES(a, gdPrimType, b, c);
	}

	void cGDriver::DrawPrimsIndexed(uint32_t a, uint32_t gdPrimType, uint32_t b, uint16_t* c, void* d, uint32_t e) {
		//NOTIMPL();
		PRINT_VALUES(a, gdPrimType, b, c);
		PRINT_VALUES(d, e);
	}

	void cGDriver::Reset(void) {
		NOTIMPL();
	}
}