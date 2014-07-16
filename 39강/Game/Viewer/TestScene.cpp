
#include "stdafx.h"
#include "TestScene.h"



cTestScene::cTestScene(LPD3DXSPRITE sprite) :
	framework::cWindow(sprite, 0, "testScene")
{
	Create("The-Waters-Edge.jpg");

}

cTestScene::~cTestScene()
{

}

