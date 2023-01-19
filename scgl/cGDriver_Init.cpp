#include <string>
#include "cGDriver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace nSCGL
{
#ifndef NDEBUG
	static void GLAPIENTRY LogGLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
			// We know we're using the fixed function pipeline.
			// FUTURE: but what if we weren't?
			return;
		}

		static FILE* glMessageLog = nullptr;
		if (glMessageLog == nullptr) {
			glMessageLog = fopen("cGDriver.opengl.log", "w");
		}

		fprintf(glMessageLog, "[GLerr] [source: %x] [type: %x] [id: %x] [severity: %d] %s\n", source, type, id, severity, message);
		fflush(glMessageLog);
	}
#endif

	bool cGDriver::Init(void) {
		if (glfwInit() != GLFW_TRUE) {
			const char* errorDescription;
			int errorCode = glfwGetError(&errorDescription);

			char* error = new char[strlen(errorDescription) + 64];
			sprintf(error, "Failed to initialize GLFW (error code: %d)\n%s", errorCode, errorDescription);
			MessageBoxA(NULL, error, "SCGL failed to start", MB_ICONERROR);
			delete[] error;

			SetLastError(DriverError::CREATE_CONTEXT_FAIL);
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		/*glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef NDEBUG
		glfwWindowHint(GLFW_NO_ERROR, GLFW_TRUE);
#endif
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		glewExperimental = GL_TRUE;
		window = glfwCreateWindow(640, 480, "GDriverWindow--OpenGL", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwHideWindow(window);
		glewInit();

#ifndef NDEBUG
		if (GLEW_KHR_debug) {
			glDebugMessageCallback(LogGLMessage, nullptr);
			glEnable(GL_DEBUG_OUTPUT);
		}
#endif

		deviceContext = GetDC(glfwGetWin32Window(window));
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, reinterpret_cast<GLint*>(&maxTextureUnits));

		char const* vendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
		char const* renderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
		char const* version = reinterpret_cast<char const*>(glGetString(GL_VERSION));

		static const char unknownDriverName[] = "UnknownDriverName";
		static const char unknownCardVersion[] = "UnknownCardVersion";

		driverInfo.append(unknownDriverName, sizeof(unknownDriverName) - 1);
		driverInfo.append("\n", 1);
		driverInfo.append(version, strlen(version));
		driverInfo.append("\n", 1);
		driverInfo.append(renderer, strlen(renderer));
		driverInfo.append("\n", 1);
		driverInfo.append(unknownCardVersion, sizeof(unknownCardVersion) - 1);
		driverInfo.append("\n", 1);
		driverInfo.append(renderer, strlen(renderer));
		driverInfo.append("\n", 1);

		InitializeVideoModeVector();
		return true;
	}

	bool cGDriver::Shutdown(void) {
		if (window != nullptr) {
			glfwDestroyWindow(window);
			window = nullptr;
		}

		glfwTerminate();
		return true;
	}

	int32_t cGDriver::InitializeVideoModeVector(void) {
		const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &videoModeCount);
		if (modes == nullptr || videoModeCount == 0) {
			return 0;
		}

		sGDMode tempMode{};

		// If not set, SC4 throws the "Could not initialize the hardware driver" error and switches to software mode.
		tempMode.isInitialized = true;

		// TODO: do we actually need to check all these extensions or are they all part of the OpenGL 3.0 core profile?
		tempMode.supportsStencilBuffer = true;
		tempMode.supportsTextureCompression = true;
		tempMode.supportsMultitexture = true;
		tempMode.supportsTextureEnvCombine = true;
		tempMode.supportsNvTextureEnvCombine4 = true;
		tempMode.supportsFogCoord = true;
		tempMode.supportsDxtTextures = true;

		// TODO: what are these flags for and why does the game's OpenGL driver set them?
		tempMode.__unknown2 = true;
		tempMode.__unknown5[0] = false;
		tempMode.__unknown5[1] = false;
		tempMode.__unknown5[2] = false;

		int i = 0;
		while (i < videoModeCount) {
			int depth = modes[i].redBits + modes[i].greenBits + modes[i].blueBits;
			if (depth == 24) {
				// GLFW doesn't list the alpha bits. If we have 24 color bits,
				// assume that we can use 8 alpha bits and add them. SC4 is
				// expecting color depth to say 32-bit here, not 24-bit.
				depth += 8;
			}

			tempMode.index = i++;
			tempMode.width = modes[i].width;
			tempMode.height = modes[i].height;
			tempMode.depth = depth;

			tempMode.isFullscreen = true;

			videoModes.push_back(tempMode);

			tempMode.index = i++;
			tempMode.isFullscreen = false;

			videoModes.push_back(tempMode);
		}

		return videoModeCount;
	}
}