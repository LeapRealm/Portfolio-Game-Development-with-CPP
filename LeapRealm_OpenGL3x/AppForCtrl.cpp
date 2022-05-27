#if (WIN32CTRL == 1)

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

	float wndWidth	= 800;
	float wndHeight = 600;

	hWnd = CreateWindow(szWindowClass, szTitle, 
						WS_OVERLAPPEDWINDOW,
						wndWidth / 4, wndHeight / 4, 
						wndWidth, wndHeight, 
						nullptr, nullptr, hInstance, nullptr);

	initCtrl();
	void initMe();
	initMe();

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
			updateWndOpenGL(0.0f);
#if 0 /////////////////////////////////////////////////////////////////////////////////// remove
			DWORD now = GetTickCount();
			float delta = (now - prevTime) / 1000.f;
			prevTime = now;

			drawOpenGL(delta, drawGame);

#endif
		}
	}

#if 0 /////////////////////////////////////////////////////////////////////////////////// remove
	freeGame();
	freeCursor();

	freeOpenGL();
	ReleaseDC(hWnd, hDC);
#endif
	freeCtrl();
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

#if 1
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
#else

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

	case WM_LBUTTONDBLCLK:
		keyGame(iKeyStateDBCLK, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
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
		POINT p;
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		switch (zDelta)
		{
		case +120:
			
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			keyGame(iKeyStateWheelBtnUp, convertCoordinate(p.x, p.y));
			break;

		case -120:
			GetCursorPos(&p);
			ScreenToClient(hWnd, &p);
			keyGame(iKeyStateWheelBtnDown, convertCoordinate(p.x, p.y));
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
		runApp = false;
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

#endif

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

void cbDrag(const char* path)
{
	printf("drag & drop = %s\n", path);
	void callItem(const char* path);
	callItem(path);
}

struct ItemInfo
{
	char* name;
	int grade;
	int priceBuy, priceSell;
	int hp;
	int ap, dp;
};

iLinkedList* itemInfos;
void freeItemInfo(void* param)
{
	ItemInfo* ii = (ItemInfo*)param;
	delete ii->name;
	delete ii;
}

HWND hLbItem;
void cbLBItem(HWND hwnd);

HWND* hBtnItemAdd;
void cbBtnItemAdd(HWND hwnd);

HWND* hEbItemInfo;
void cbEBItemInfo(HWND hwnd);

HWND hCbItemGrade;
void cbCBItemGrade(HWND hwnd);

void initMe()
{
	acceptDrag(hWnd, cbDrag);

	itemInfos = new iLinkedList(freeItemInfo);

	const char* strLBItem[1] = { "End Of Item" };
	hLbItem = createWndListBox(10, 10, 120, 300, strLBItem, 1, cbLBItem);

	const char* strBtnItemAdd[6] = { "추가", "삭제", "아래로", "위로", "호출", "저장" };
	hBtnItemAdd = new HWND[4];
	for (int i = 0; i < 6; i++)
		hBtnItemAdd[i] = createWndButton(140 + 45 * i, 10, 40, 25, strBtnItemAdd[i], cbBtnItemAdd);

	const char* strInfo[7] = { "이름","등급","파는가격","사는가격","채력","공격력","방어력" };
	hEbItemInfo = new HWND[6];
	for (int i = 0; i < 7; i++)
	{
		int x = 140 + 150 * (i % 2), y = 40 + 30 * (i / 2);
		createWndStatic(x, y, 70, 25, strInfo[i]);

		if (i == 0)
		{
			hEbItemInfo[i] = createWndEditBox(x + 75, y, 60, 25, WndEditStyleAll, "name", 10, cbEBItemInfo);
		}
		else if (i == 1)
		{
			hEbItemInfo[i] = nullptr;
			const char* s[5] = { "아주불량","그냥불량","보통","고급","에픽" };
			hCbItemGrade = createWndComboBox(x + 75, y, 60, 140, s, 5, cbCBItemGrade);
			setWndComboBox(hCbItemGrade, 2);
		}
		else
		{
			hEbItemInfo[i] = createWndEditBox(x + 75, y, 60, 25, WndEditStyleInt, "0", 10, cbEBItemInfo);
		}
	}

}

void freeMe()
{
	delete itemInfos;
}

void cbLBItem(HWND hwnd)
{
	int selected = indexWndListBox(hwnd);
	int count = CountWndListBox(hwnd);
	if (selected == (count - 1))
	{
		setWndText(hEbItemInfo[0], "name");
		setWndComboBox(hCbItemGrade, 2);
		setWndText(hEbItemInfo[2], "%d", 0);
		setWndText(hEbItemInfo[3], "%d", 0);
		setWndText(hEbItemInfo[4], "%d", 0);
		setWndText(hEbItemInfo[5], "%d", 0);
		setWndText(hEbItemInfo[6], "%d", 0);
		return;
	}

	ItemInfo* ii = (ItemInfo*)itemInfos->getObjectByIndex(selected);

	setWndText(hEbItemInfo[0], ii->name);
	setWndComboBox(hCbItemGrade, ii->grade);
	setWndText(hEbItemInfo[2], "%d", ii->priceSell);
	setWndText(hEbItemInfo[3], "%d", ii->priceBuy);
	setWndText(hEbItemInfo[4], "%d", ii->hp);
	setWndText(hEbItemInfo[5], "%d", ii->ap);
	setWndText(hEbItemInfo[6], "%d", ii->dp);
}

void cbBtnItemAdd(HWND hwnd)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		if (hBtnItemAdd[i] == hwnd)
			break;
	}

	if (i == 0)
	{
		printf("add\n");

		ItemInfo* ii = new ItemInfo();
		ii->name = getWndText(hEbItemInfo[0]);
		ii->grade = indexWndComboBox(hCbItemGrade);
		ii->priceSell = getWndInt(hEbItemInfo[2]);
		ii->priceBuy = getWndInt(hEbItemInfo[3]);
		ii->hp = getWndInt(hEbItemInfo[4]);
		ii->ap = getWndInt(hEbItemInfo[5]);
		ii->dp = getWndInt(hEbItemInfo[6]);

		int selected = indexWndListBox(hLbItem);
		itemInfos->addObject(selected, ii);
		addWndListBox(hLbItem, selected, ii->name);
	}
	else if (i == 1)
	{
		printf("del\n");

		int selected = indexWndListBox(hLbItem);
		removeWndListBox(hLbItem, selected);
		setWndListBox(hLbItem, selected);
		itemInfos->removeObject(selected);
	}
	else if (i == 2)
	{
		printf("down\n");
	}
	else if (i == 3)
	{
		printf("up\n");
	}
	else if (i == 4)
	{
		void callItem();
		callItem();
	}
	else if (i == 5)
	{
		void saveItem();
		saveItem();
	}
}

void cbEBItemInfo(HWND hwnd)
{
	// do something...
}

void cbCBItemGrade(HWND hwnd)
{
	
}

#define FILTER TEXT("ITM Files(*.ITM, *.itm)\0*.ITM;*.itm\0")

void callItem(const char* path)
{
	FILE* pf = fopen(path, "rb");

	for (int i = 0; i < itemInfos->count; i++)
		removeWndListBox(hLbItem, 0);

	itemInfos->removeAllObjects();

	int num;
	fread(&num, sizeof(int), 1, pf);

	for (int i = 0; i < num; i++)
	{
		ItemInfo* ii = new ItemInfo();
		int len;
		fread(&len, sizeof(int), 1, pf);
		char* s = new char[len + 1]; s[len] = 0;
		fread(s, sizeof(char), len, pf);
		ii->name = s;

		fread(&ii->grade, sizeof(int), 1, pf);
		fread(&ii->priceSell, sizeof(int), 1, pf);
		fread(&ii->priceBuy, sizeof(int), 1, pf);
		fread(&ii->hp, sizeof(int), 1, pf);
		fread(&ii->ap, sizeof(int), 1, pf);
		fread(&ii->dp, sizeof(int), 1, pf);

		itemInfos->addObject(ii);
		addWndListBox(hLbItem, i, s);
	}

	fclose(pf);

	setWndListBox(hLbItem, num);
}

void callItem()
{
	const char* path = openFileDialog(true, FILTER);
	if (path == nullptr)
		return;
	callItem(path);


}

void saveItem()
{
	const char* path = openFileDialog(false, FILTER);
	if (path == nullptr)
		return;

	FILE* pf = fopen(path, "wb");

	fwrite(&itemInfos->count, sizeof(int), 1, pf);
	for (int i = 0; i < itemInfos->count; i++)
	{
		ItemInfo* ii = (ItemInfo*)itemInfos->getObjectByIndex(i);
		int len = strlen(ii->name);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(ii->name, sizeof(char), len, pf);
		fwrite(&ii->grade, sizeof(int), 1, pf);
		fwrite(&ii->priceSell, sizeof(int), 1, pf);
		fwrite(&ii->priceBuy, sizeof(int), 1, pf);
		fwrite(&ii->hp, sizeof(int), 1, pf);
		fwrite(&ii->ap, sizeof(int), 1, pf);
		fwrite(&ii->dp, sizeof(int), 1, pf);
	}

	fclose(pf);
}

#endif
