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

// ��ũ�� ����
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (p) { delete p; p=NULL;} }
#endif
#ifndef SAFE_DELETEA
#define SAFE_DELETEA(p) {if (p) { delete[] p; p=NULL;} }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if (p) { p->Release(); p=NULL;} }
#endif

#include "../math/Math.h"

#include "indexbuffer.h"
#include "light.h"
#include "material.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "vertexformat.h"
#include "Line.h"

