#include <cRZCOMDllDirector.h>
#include <cIGZCOM.h>
#include "../cGDriver.h"

#include <GL/glew.h>

namespace nSCGL
{
	cIGZBuffer* cGDriver::CopyColorBuffer(int32_t a2, int32_t a3, int32_t a4, int32_t a5, cIGZBuffer* a6) {
		PRINT_VALUES(a2, a3, a4, a5);
		PRINT_VALUES(a6);

		if (a6 && a6->IsReady()) {
			int width = a6->Width();
			int height = a6->Height();
			if (a4 >= width)
				width = a4;
			a4 = width;
			if (a5 >= height)
				height = a5;
			a5 = height;
		}

		if (!a6) {
			// create and get cIGZBuffer object
			cIGZBuffer* buffer;
			if (!RZGetCOMDllDirector()->GZCOM()->GetClassObject(0xC470D325, GZIID_cIGZBuffer, (void**)&buffer)) return nullptr;
			a6 = buffer;
			if (!a6) return nullptr;
		}

		if (a6->IsReady()) {
			if (a6->Init(a4, a5, 9, 32))
			{
				int v15 = a2;
				if (a2 < 0) v15 = 0;
				int v27 = v15;
				int v16 = a3;
				if (a3 < 0)
					v16 = 0;
				int v29 = v16;
				if (a6->Lock(32769)) {
					a6->Fill(0xFF000000, nullptr);
					a6->Unlock(32769);
				}
				if (!a6->Lock(32769)) return a6;
				uint32_t v33 = a6->GetColorSurfaceStride();
				uint32_t v18 = a6->GetColorSurfaceBits();

				// copy pixels
				glReadPixels(0, 0, a4, a5, GL_BGRA, GL_UNSIGNED_BYTE, (void*)v18);

				a6->Unlock(32769);

				return a6;
			}
		}


		return nullptr;
	}
}