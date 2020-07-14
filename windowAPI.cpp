#include "stdafx.h"
#include"sceneManager.h"
#include"Scene.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//API : Application Programming Interface

HINSTANCE	_hInstance;
HWND		_hWnd;

Vector2		_ptMouse;
bool		_leftBtnDown = false;

//함수의 프로토타입 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG			message;
	WNDCLASS	wndClass;

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	ShowWindow(_hWnd, cmdShow);

	GRAPHICMANAGER->init();
	Graphic::SetRendertarget();
	SCENEMANAGER->init();
	OBJECTMANAGER->Init();
	CAMERA->Init();
	KEYMANAGER->init();
	TXTDATA->init();
	TIMEMANAGER->init();
	INIDATAMANAGER->init();
	//SetTimer(_hWnd, 1, 10, NULL);
	//메시지 루프 돌기이전에
	//sceneManager::getSingleton()->GetNowScene()->Init();

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			sceneManager::GetInstance()->GetNowScene()->PhysicsUpdate();
			sceneManager::GetInstance()->GetNowScene()->Update();
			CAMERA->Update();
			SOUNDMANAGER->update();

			ID2D1RenderTarget* renderTarget = GRAPHICMANAGER->GetRenderTarget();
			renderTarget->BeginDraw();
			renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			//renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			renderTarget->Clear(D2D1::ColorF(GRAPHICMANAGER->GetRenderTargetColor()));
			//==================================================

			sceneManager::GetInstance()->GetNowScene()->Render();

			//===================================================
			HRESULT hr = renderTarget->EndDraw();
			if (hr == D2DERR_RECREATE_TARGET) GRAPHICMANAGER->Reload();
		}
	}
	//KillTimer(_hWnd, 1);

	//루프문이 다돌면 씬 해제
	//sceneManager::getSingleton()->GetNowScene()->Release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//return sceneManager::getSingleton()->GetNowScene()->MainProc( hWnd,  iMessage,  wParam,  lParam);

	PAINTSTRUCT ps;
	HDC			hdc;

	char str[128];
	int len;

	switch (iMessage)
	{
	case WM_CREATE:

		break;
	case WM_LBUTTONDOWN:
		_leftBtnDown = true;
		break;
	case WM_LBUTTONUP:
		_leftBtnDown = false;
		break;
	case WM_MOUSEMOVE:
		_ptMouse.x = static_cast<float>(LOWORD(lParam));
		_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
	{
		int wheel = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f;
		CAMERA->SetScale(Vector2(wheel * 0.05f, wheel * 0.05f));
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//클라이언트 영역 재조정
void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	//실절적으로 클라이언트 영역 크기 셋팅 함수
	SetWindowPos(_hWnd, NULL, x, y,
		(winRect.right - winRect.left),
		(winRect.bottom - winRect.top),
		SWP_NOZORDER | SWP_NOMOVE);
}