#ifndef WGLEFP_H
#define WGLEFP_H

#include "GL/gl.h"
#include "glext/wglext.h"

// WGL function pointer (OGL extensions)
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 0;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;

#endif // WGLEFP_H
