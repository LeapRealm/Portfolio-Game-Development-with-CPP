#if (WIN32CTRL == 1)

#include "App.h"

#include "iWindow.h"
#include "iStd.h"

HWND hWnd;
HDC hDC;

bool runApp = false;
DWORD prevTime;
bool* keys;
 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 
					  _In_opt_ HINSTANCE hPrevInstance, 
					  _In_ LPWSTR lpCmdLine, 
					  _In_ int nShowCmd)
{
	ULONG_PTR token = iGraphics::startGdiplus();

	WCHAR szTitle[20] = TEXT("LeapRealm");
	WCHAR szWindowClass[20] = TEXT("LeapRealm");

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEAPREALMOPENGL3X));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	float wndWidth	= 1280;
	float wndHeight = 640;

	hWnd = CreateWindow(szWindowClass, szTitle, 
						WS_OVERLAPPEDWINDOW,
						wndWidth / 4, wndHeight / 4, 
						wndWidth, wndHeight, 
						nullptr, nullptr, hInstance, nullptr);

	initCtrl();
	void initTool();
	initTool();

	prevTime = GetTickCount();
	keys = new bool[128];
	memset(keys, false, sizeof(bool) * 128);
	srand(time(nullptr));

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	runApp = true;
	MSG msg;
	while (runApp)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD now = GetTickCount();
			float delta = (now - prevTime) / 1000.f;
			prevTime = now;

		}
	}

	freeCtrl();
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);

	delete keys; 
	iGraphics::stopGdiplus(token);
	return (int)msg.wParam;
}

iPoint convertCoordinate(float x, float y)
{
	RECT rt;
	GetClientRect(hWnd, &rt);
	x -= rt.left;
	y -= rt.top;

	return iPointMake(
		(x - viewport.origin.x) / viewport.size.width  * devSize.width,
		(y - viewport.origin.y) / viewport.size.height * devSize.height);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DROPFILES:
		updateDrag(wParam, lParam);
		break;

	case WM_COMMAND:
		updateCtrl(wParam, lParam);
		break;

	case WM_CLOSE:
	{
		int result = MessageBox(nullptr, L"종료?", L"종료?", MB_YESNO);

		if (result == IDYES)
			runApp = false;
		else
			return 0;

		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

static iImage* imgCursor;
iPoint positionCursor;
bool bCursor;
void loadCursor()
{
	iImage* img = new iImage();

	Texture* tex = createTexture("assets/cursor.png");
	img->addTexture(tex);
	freeTexture(tex);

	img->position = iPointMake(-14, -14);
	img->scale = 0.5f;
	imgCursor = img;
}

void freeCursor()
{
	delete imgCursor;
}

void setCursor(iPoint position)
{
	positionCursor = position;
}

bool updateCursor(bool inClient)
{
	if (bCursor == inClient)
		return false;
	bCursor = inClient;

	if (bCursor)
	{
		while (true)
		{
			int n = ShowCursor(FALSE);
			if (n < 0)
				break;
		}
	}
	else
	{
		while (true)
		{
			int n = ShowCursor(TRUE);
			if (n > -1)
				break;
		}
	}

	return true;
}

void drawCursor(float dt)
{
	if (bCursor)
		imgCursor->paint(dt, positionCursor);
}

RECT rtPrev;
void setCurrentMonitor(RECT& rt);

void setFullscreen(bool isFullscreen)
{
	if (isFullscreen)
	{
		GetWindowRect(hWnd, &rtPrev);

		RECT rt;
		setCurrentMonitor(rt);

		int x = rt.left, y = rt.top;
		int w = rt.right - rt.left, h = rt.bottom - rt.top;
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
	}
	else
	{
		int x = rtPrev.left, y = rtPrev.top;
		int w = rtPrev.right - rtPrev.left, h = rtPrev.bottom - rtPrev.top;
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, x, y, w, h, SWP_SHOWWINDOW);
	}
}

void setCurrentMonitor(RECT& rt)
{
	GetWindowRect(hWnd, &rt);

	HMONITOR hMonitor = MonitorFromRect(&rt, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	if (mi.dwFlags == MONITORINFOF_PRIMARY)
	{
		rt.left = 0;
		rt.top = 0;
		rt.right = GetSystemMetrics(SM_CXSCREEN);
		rt.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		memcpy(&rt, &mi.rcWork, sizeof(RECT));
	}
}

//////////////////////////////////////////////////////////////

void initTool()
{
	createWndOpenGL(0, 0, 100, 100);
}

#endif
