#pragma once


#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace graphic {
	LPDIRECT3DDEVICE9 GetDevice();
}

// 매크로 정의
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (p) { delete p; p=NULL;} }
#endif
#ifndef SAFE_DELETEA
#define SAFE_DELETEA(p) {if (p) { delete[] p; p=NULL;} }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if (p) { p->Release(); p=NULL;} }
#endif

#define RET(exp)		{if((exp)) return; }			// exp가 true이면 리턴
#define RETV(exp,val)	{if((exp)) return val; }

#include "../math/Math.h"

#include "DxInit.h"
#include "indexbuffer.h"
#include "light.h"
#include "material.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "vertexformat.h"
#include "line.h"
#include "grid.h"
#include "grid2.h"
#include "cube.h"
#include "cubetex.h"
#include "shader.h"
