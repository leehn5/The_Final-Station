#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <fstream>
#include <string>
#include<iomanip>

using namespace std;
#include "Box2D/Box2D.h"
#ifdef _DEBUG
#pragma comment(lib, "lib/x86_64/Debug/Box2D.lib")
#else
#pragma comment(lib, "lib/x86_64/Release/Box2D.lib")
#endif
//rrr
#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "utils.h"
#include "timeManager.h"
#include "txtData.h"
//#include"rapidjson/document.h"
//#include"rapidjson/writer.h"
//#include"nlohmann/json.hpp"
//#include"nlohmann/json_fwd.hpp"
#include"lib/single_include/nlohmann/json.hpp"
using json = nlohmann::json;
#include"iniDataManager.h"
#include"lib/single_include/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace SEVENTEEN_UTIL;

//=========================================
// ## 19.10.30 ## - 디파인문 -
//=========================================

#define WINNAME (LPCTSTR)(TEXT("The Final Station"))
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 1280
#define WINSIZEY 720
#define WINSIZE Vector2(WINSIZEX, WINSIZEY)
#define DesignResolution Vector2(1280,720)

#define WINSTYLE WS_CAPTION | WS_SYSMENU

#include "soundManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "sceneManager.h"
#include "BoxWorldManager.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include"ObjectManager.h"
#include "MousePointer.h"
#include "Ability.h"
#include "Image.h"

#define RND randomFunction::GetInstace()
#define KEYMANAGER keyManager::GetInstance()
#define TIMEMANAGER timeManager::GetInstance()
#define TXTDATA txtData::GetInstance()
#define SOUNDMANAGER soundManager::GetInstance()
#define SCENEMANAGER sceneManager::GetInstance()
#define BOXWORLDMANAGER BoxWorldManager::GetInstance()
#define PHYSICSMANAGER PhysicsManager::GetInstance()
#define GRAPHICMANAGER GraphicsManager::GetInstance()
#define CAMERA Camera::GetInstance()
#define OBJECTMANAGER ObjectManager::GetInstace()
#define INIDATAMANAGER iniDataManager::GetInstance()
#define MOUSEPOINTER MousePointer::GetInstance()


#define SAFE_DELETE(p) {if(p) {delete(p); (p)=NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p) = NULL;}}
#define SAFE_OBJECT_RELEASE(p) {if(p) {(p)->Release(); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}

const short CATEGORY_PLAYER = 0x0001;		// 0000000000000001 in binary
const short CATEGORY_ENEMY = 0x0002;		// 0000000000000010 in binary
const short CATEGORY_SCENERY = 0x0004;		// 0000000000000100 in binary
const short CATEGORY_PISTOL_SHELL = 0x0008;	// 0000000000001000 in binary

const short MASK_PLAYER = ~CATEGORY_ENEMY | CATEGORY_SCENERY;
const short MASK_ENEMY = CATEGORY_PLAYER | CATEGORY_SCENERY;
const short MASK_SCENERY = -1;
const short MASK_PISTOL_SHELL = CATEGORY_PISTOL_SHELL | CATEGORY_SCENERY;

template <typename T>
inline void SafeRelease(T* &p) { if (p) p->Release(); p = NULL; }

//==========================================
// ## 19.10.31 ## Extern
//==========================================
extern HINSTANCE				_hInstance;
extern HWND						_hWnd;
extern Vector2					_ptMouse;
extern bool						_leftBtnDown;

//==========================================
// ## 19.22.22 ## 엔진 첫 걸음
//==========================================
#define world2pWorld 10.f/800
#define pWorld2world 800.f/10
#define RadToDeg (float)180/PI
#define DegToRad (float)PI/180



#include"omega.h"