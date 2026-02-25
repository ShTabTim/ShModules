#include "highgl.hpp"
#include "highgl_initialisation.hpp"

#define high_gl_create(output, func, ...) func##_proc func= nullptr;

HIGH_GL_USING_LIST(high_gl_create)

#undef high_gl_create

#define high_gl_find(output, func, ...) \
    func = (func##_proc)wglGetProcAddress(#func); \
    if (func == nullptr) { \
        MessageBoxW(nullptr, TEXT(#func), TEXT("НАЕБНУЛАСЬ ФУНКЦИЯ"), MB_ICONERROR | MB_OK);\
    }


void high_gl_init() {
    HIGH_GL_USING_LIST(high_gl_find)
}

HGLRC get_gl(HDC dc) {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		24,
		8,
		0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};
	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	HGLRC temp = wglCreateContext(dc);
	wglMakeCurrent(dc, temp);

	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) {
		MessageBoxW(nullptr, TEXT("ОШИБКА ЕРОР"), TEXT("Не поддерживается новый опенгл."), MB_ICONERROR | MB_OK);
		return temp;
	}

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB   0x00000002

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, SH_OGL_MAJOR_VERSION,
		WGL_CONTEXT_MINOR_VERSION_ARB, SH_OGL_MINOR_VERSION,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef  USE_OGL_DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif//USE_OGL_DEBUG
		0
	};

	HGLRC modernContext = wglCreateContextAttribsARB(dc, 0, attribs);

	if (modernContext) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(temp);
		wglMakeCurrent(dc, modernContext);
		return modernContext;
	}

	MessageBoxW(nullptr, TEXT("ОШИБКА ЕРОР"), TEXT("Я не знаю что пошло не так."), MB_ICONERROR | MB_OK);
	return temp;
}

#undef high_gl_find