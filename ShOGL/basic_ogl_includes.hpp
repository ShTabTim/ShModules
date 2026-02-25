#ifndef SH_OGL_BASIC_OGL_INCLUDES_HPP
#define SH_OGL_BASIC_OGL_INCLUDES_HPP

#include <cinttypes>

#include <Windows.h>
#include <gl/GL.h>

#define ERROR_ERROR_NAME L"Ошибка"
#define error_named(e, n) MessageBoxW(NULL, e, n, MB_ICONERROR|MB_OK)
#define error(e) MessageBoxW(NULL, e, ERROR_ERROR_NAME, MB_ICONERROR|MB_OK)

#endif//SH_OGL_BASIC_OGL_INCLUDES_HPP