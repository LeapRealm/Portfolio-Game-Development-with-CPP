#include "App.h"

#include "game.h"
#include "iWindow.h"

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEAPREALMOPENGL1X));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	int mW = GetSystemMetrics(SM_CXSCREEN);
	int mH = GetSystemMetrics(SM_CYSCREEN);
    float wndWidth = mW * 2 / 3;
    float wndHeight = mH * 2 / 3;

    hWnd = CreateWindowW(szWindowClass, szTitle, 
				         WS_OVERLAPPEDWINDOW,
				         wndWidth / 4, wndHeight / 4, 
				         wndWidth, wndHeight, 
				         nullptr, nullptr, hInstance, nullptr);

    hDC = GetDC(hWnd);
	loadOpenGL(hDC);

	int wH = wndHeight + win_border_height;
	// wW : wH = devsize.width : devsize.height
	int wW = wndHeight * devSize.width / devSize.height + win_border_width;
	MoveWindow(hWnd, (mW - wW) / 2, (mH - wH) / 2, wW, wH, true);

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
        (x - viewport.origin.x) / viewport.size.width * devSize.width,
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
		case 120:
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

  //  	if (runApp == false)
		//	break;

		//resizeOpenGL((rect.right - rect.left) - win_border_width, 
		//			 (rect.bottom - rect.top) - win_border_height);
  //  	drawOpenGL(0.0f, drawGame);
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

HGLRC hRC;
void loadOpenGL(void* hdc)
{
	//
	// setup OpenGL
	//
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
				  PFD_SUPPORT_OPENGL |
				  PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK == err)
		printf("GLEW %s, GL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));
	else
		printf("Error: %s\n", glewGetErrorString(err));

	//
	// init OpenGL
	//

	// Drawing Region
	devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
	// viewport -> resizeOpenGL

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, devSize.width, devSize.height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	setGlBlendFunc(iBlendFuncAlpha);

	// Settings of Texture
	setTexture(Linear, Clamp);

	// Back or Front Buffer(Frame Buffer Object)
	fbo = new iFBO(devSize.width, devSize.height);
}

void freeOpenGL()
{
	// setup OpenGL
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);

	delete fbo;
}

struct iZoom
{
	iPoint center;
	float scale, scaleS, scaleE;
	float zoomDt, _zoomDt;
	float delayDt, _delayDt;

	bool order;

	iZoom()
	{
		center = iPointZero;
		scale = scaleS = scaleE = 1.0f;
		zoomDt = _zoomDt = delayDt = _delayDt = 0.0f;
		order = false;
	}

	void set(iPoint c, float s, float zDt, float dDt)
	{
		center = c;
		scaleS = scale;
		scaleE = s;
		_zoomDt = zDt;
		zoomDt = 0.00001f;
		_delayDt = dDt;
		delayDt = 0.0f;

		order = true;
	}

	float update(float dt)
	{
		if (zoomDt == 0.0f)
			return 1.0f;

		// 줌인중...
		if (zoomDt < _zoomDt)
		{
			zoomDt += dt;
			if (zoomDt > _zoomDt)
			{
				zoomDt = _zoomDt;
				if (order == false)
				{
					zoomDt = 0.0f; // stop
					scale = scaleS;
					return 1.0f;
				}
			}

			if (order)
				scale = linear(scaleS, scaleE, zoomDt / _zoomDt);
			else
				scale = linear(scaleE, scaleS, zoomDt / _zoomDt);
		}
		// 줌이 된 상태
		else if (delayDt < _delayDt)
		{
			scale = scaleE;

			delayDt += dt;
			if (delayDt > _delayDt)
			{
				delayDt = _delayDt;

				zoomDt = 0.00001f;
				order = false;
			}
		}
		return scale;
	}
};
static iZoom zoom;

void setZoom(iPoint center, float scale, float zoomDt, float delayDt)
{
	zoom.set(center, scale, zoomDt, delayDt);
}

void drawOpenGL(float dt, MethodDraw m)
{
	setGlBlendFunc(iBlendFuncAlpha);
	fbo->bind();
	iFBO::clear(0, 0, 0, 0);
	m(dt); // drawGame
	drawCursor(dt);
	fbo->unbind();

	setGlBlendFunc(iBlendFuncPreMultipliedAlpha);
	iFBO::clear(0, 0, 0, 0);
	setRGBA(1, 1, 1, 1);
	Texture* t = fbo->tex;

	float s = zoom.update(dt);
	if (s == 1.0f)
	{
		drawTexture(t, 0, 0, 1, 1,
				  TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_VER);
	}
	else
	{
		iPoint p = zoom.center * (1.0f - s);
		drawTexture(t, p.x, p.y, s, s,
				  TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_VER);
	}

	SwapBuffers(hDC);
}

void resizeOpenGL(int width, int height)
{
	iSize wndSize = iSizeMake(width, height);

	float r0 = wndSize.width / devSize.width;
	float r1 = wndSize.height / devSize.height;
	if (r0 < r1)
	{
		viewport.origin.x = 0;
		viewport.size.width = wndSize.width;

		float r = wndSize.width / devSize.width;
		viewport.size.height = devSize.height * r;
		viewport.origin.y = (wndSize.height - viewport.size.height) / 2;
	}
	else if (r0 > r1)
	{
		viewport.origin.y = 0;
		viewport.size.height = wndSize.height;

		float r = wndSize.height / devSize.height;
		viewport.size.width = devSize.width * r;
		viewport.origin.x = (wndSize.width - viewport.size.width) / 2;
	}
	else
	{
		viewport.origin = iPointZero;
		viewport.size = wndSize;
	}

	glViewport(viewport.origin.x, viewport.origin.y,
			   viewport.size.width, viewport.size.height);
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
