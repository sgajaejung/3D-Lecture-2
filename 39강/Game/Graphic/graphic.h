
#pragma once


#include <d3d9.h>
#include <d3dx9.h>

#include "../Common/common.h"
using namespace common;

#include "../wxMemMonitorLib/wxMemMonitor.h" // debug library


#include "base/graphicdef.h"
#include "base/material.h"
#include "base/light.h"
#include "base/vertexbuffer.h"
#include "base/indexbuffer.h"
#include "base/texture.h"
#include "base/vertexformat.h"
#include "base/grid.h"
#include "base/grid2.h"

#include "model/rawmesh.h"
#include "model/rawani.h"
#include "model/track.h"
#include "model/mesh.h"
#include "model/rigidmesh.h"
#include "model/skinnedmesh.h"
#include "model/model.h"
#include "model/bonemgr.h"
#include "model/bonenode.h"
#include "model/character.h"

#include "sprite/sprite.h"

#include "manager/Renderer.h"
#include "manager/resourcemanager.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
