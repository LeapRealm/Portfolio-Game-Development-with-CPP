#include "App.h"

#include "iWindow.h"
#include "game.h"

static int win_border_width, win_border_height;

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

    WCHAR szTitle[20] = L"LeapRealm";
    WCHAR szWindowClass[20] = L"LeapRealm";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
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

	int mW = GetSystemMetrics(SM_CXSCREEN);
	int mH = GetSystemMetrics(SM_CYSCREEN);
    float wndWidth	= mW * 2 / 3;
    float wndHeight = mH * 2 / 3;

    hWnd = CreateWindowW(szWindowClass, szTitle, 
				         WS_OVERLAPPEDWINDOW,
				         wndWidth / 4, wndHeight / 4, 
				         wndWidth, wndHeight, 
				         nullptr, nullptr, hInstance, nullptr);

    hDC = GetDC(hWnd);
	loadOpenGL(hDC);

	// wW : wH = devsize.width : devsize.height
	int wH = wndHeight + win_border_height;
	int wW = wndHeight * devSize.width / devSize.height + win_border_width;
	MoveWindow(hWnd, (mW - wW) / 2, (mH - wH) / 2, wW, wH, TRUE);

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    prevTime = GetTickCount();
    keys = new bool[128];
    memset(keys, false, sizeof(bool) * 128);
    srand(time(nullptr));

    loadGame();
	loadCursor();

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

			drawOpenGL(delta, drawGame);
        }
    }

    freeGame();
	freeCursor();

	freeOpenGL();
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

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
    	if (keys[wParam] == false)
		{
			setKeyPressed(true, wParam);
			keys[wParam] = true;
		}
		break;
    }

    case WM_KEYUP:
		setKeyPressed(false, wParam);
        keys[wParam] = false;
        break;

    case WM_LBUTTONDOWN:
        keyGame(iKeyStateBegan, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;

    case WM_MOUSEMOVE:
	{
		iPoint c = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
		setCursor(c);
		keyGame(iKeyStateMoved, c);
		break;
	}

    case WM_LBUTTONUP:
        keyGame(iKeyStateEnded, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;

    case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		switch (zDelta)
		{
		case  120:
			printf("+1\n");
			break;

		case -120:
			printf("-1\n");
			break;
		}
		break;
	}

	case WM_SETCURSOR:
		if (updateCursor(LOWORD(lParam) == HTCLIENT))
			return true;
		break;

    case WM_CREATE:
    {
        if (hWnd == nullptr)
            hWnd = hwnd;

        RECT rt0, rt1;
        GetWindowRect(hWnd, &rt0);
        GetClientRect(hWnd, &rt1);
        win_border_width = (rt0.right - rt0.left) - (rt1.right - rt1.left);
        win_border_height = (rt0.bottom - rt0.top) - (rt1.bottom - rt1.top);
        break;
    }

    case WM_GETMINMAXINFO:
    {
        float height = GetSystemMetrics(SM_CYSCREEN) / 3;
        int width = height * devSize.width / devSize.height;

        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = width + win_border_width;
        info->ptMinTrackSize.y = height + win_border_height;
        break;
    }

    case WM_MOVE:
    case WM_SIZING:
    {
        RECT& rect = *reinterpret_cast<LPRECT>(lParam);
        enforceResolution(int(wParam), rect, win_border_width, win_border_height);
        break;
    }

    case WM_SIZE:
    {
		resizeOpenGL(LOWORD(lParam), HIWORD(lParam));
		if (runApp)
			drawOpenGL(0.0f, drawGame);
        break;
    }

    case WM_CLOSE:
    {
        int result = MessageBox(nullptr, L"Exit?", L"Exit", MB_YESNO);
        if (result == IDYES)
            runApp = false;
        else
            return 0;
        break;
    }

    case WM_DESTROY:
	{
        PostQuitMessage(0);
        return 0;
	}
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height)
{
    switch (edge)
    {
	    case WMSZ_BOTTOM:
	    case WMSZ_TOP:
	    {
	        int h = (rect.bottom - rect.top) - win_border_height;
	        int w = h * devSize.width / devSize.height + win_border_width;
	        rect.left = (rect.left + rect.right) / 2 - w / 2;
	        rect.right = rect.left + w;
	        break;
	    }

	    case WMSZ_BOTTOMLEFT:
	    {
	        int w = (rect.right - rect.left) - win_border_width;
	        int h = (rect.bottom - rect.top) - win_border_height;

	        if (w * devSize.height > devSize.width * h)
	        {
	            w = h * devSize.width / devSize.height + win_border_width;
	            rect.left = rect.right - w;
	        }
	        else
	        {
	            h = w * devSize.height / devSize.width + win_border_height;
	            rect.bottom = rect.top + h;
	        }
	        break;
	    }

	    case WMSZ_BOTTOMRIGHT:
	    {
	        int w = (rect.right - rect.left) - win_border_width;
	        int h = (rect.bottom - rect.top) - win_border_height;

    		if (w * devSize.height > devSize.width * h)
	        {
	            w = h * devSize.width / devSize.height + win_border_width;
	            rect.right = rect.left + w;
	        }
	        else
	        {
	            h = w * devSize.height / devSize.width + win_border_height;
	            rect.bottom = rect.top + h;
	        }
	        break;
	    }

	    case WMSZ_LEFT:
	    case WMSZ_RIGHT:
	    {
	        int w = (rect.right - rect.left) - win_border_width;
	        int h = w * devSize.height / devSize.width + win_border_height;
	        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
	        rect.bottom = rect.top + h;
	        break;
	    }

	    case WMSZ_TOPLEFT:
	    {
	        int w = (rect.right - rect.left) - win_border_width;
	        int h = (rect.bottom - rect.top) - win_border_height;
	        if (w * devSize.height > devSize.width * h)
	        {
	            w = h * devSize.width / devSize.height + win_border_width;
	            rect.left = rect.right - w;
	        }
	        else
	        {
	            h = w * devSize.height / devSize.width + win_border_height;
	            rect.top = rect.bottom - h;
	        }
	        break;
	    }

	    case WMSZ_TOPRIGHT:
	    {
	        int w = (rect.right - rect.left) - win_border_width;
	        int h = (rect.bottom - rect.top) - win_border_height;

    		if (w * devSize.height > devSize.width * h)
	        {
	            w = h * devSize.width / devSize.height + win_border_width;
	            rect.right = rect.left + w;
	        }
	        else
	        {
	            h = w * devSize.height / devSize.width + win_border_height;
	            rect.top = rect.bottom - h;
	        }
	        break;
	    }
    }
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
