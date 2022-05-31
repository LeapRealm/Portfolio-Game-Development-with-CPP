#include "iWindow.h"

#include "iStd.h"

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText)

	int length = MultiByteToWideChar(CP_UTF8, 0, szText, -1, nullptr, 0);
	wchar_t* ws = new wchar_t[length];
	MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText) + 1, ws, length);

	return ws;
}

char* utf16_to_utf8(const wchar_t* str)
{
	int length = WideCharToMultiByte(CP_UTF8, 0, str, lstrlen(str), nullptr, 0, nullptr, nullptr);
	char* s = new char[length + 1];
	WideCharToMultiByte(CP_UTF8, 0, str, lstrlen(str), s, length, nullptr, nullptr);
	s[length] = 0;

	return s;
}

extern HWND hWnd;

struct WndCtrl
{
	HWND hwnd;
	WndStyle style;
	MethodCtrlUpdate method;
};

WndCtrl* wc;
#define WcMax 1000
int wcNum = 0;

void initCtrl()
{
	InitCommonControls();

	wc = new WndCtrl[WcMax];
	wcNum = 0;
}

void freeCtrl()
{
	delete wc;
}

MethodDragFile methodDrag = nullptr;
void acceptDrag(HWND hWnd, MethodDragFile method)
{
	DragAcceptFiles(hWnd, TRUE);
	methodDrag = method;
}

void updateDrag(WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;

	wchar_t wstrPath[1024];
	DragQueryFile(hDrop, 0, wstrPath, 1024);

	char* path = utf16_to_utf8(wstrPath);
	if (methodDrag)
		methodDrag(path);
	delete path;
}

char* strOpenFileDialog = nullptr;
const char* openFileDialog(bool isOpen, LPCWSTR wFilter)
{
	wchar_t wstrPath[1024];

	OPENFILENAME ofn;
	memset(&ofn, 0x00, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = wFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.lpstrFile = wstrPath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	bool isSuccess = false;
	if (isOpen) isSuccess = GetOpenFileName(&ofn);
	else	    isSuccess = GetSaveFileName(&ofn);

	if (isSuccess)
	{
		wsprintf(wstrPath, TEXT("%s"), ofn.lpstrFile);
		if (strOpenFileDialog)
			delete strOpenFileDialog;
		strOpenFileDialog = utf16_to_utf8(wstrPath);
		return strOpenFileDialog;
	}
	return nullptr;
}

const char* openFileDialog(bool isOpen, const char* filter)
{
	wchar_t* wFilter = utf8_to_utf16(filter);
	const char* path = openFileDialog(isOpen, wFilter);
	delete wFilter;
	return path;
}

void showChooseColor(MethodChooseColor method)
{
	CHOOSECOLOR cc;
	memset(&cc, 0x00, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	COLORREF color[16];
	cc.lpCustColors = color;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (ChooseColor(&cc) == false)
		return;

	int r = (cc.rgbResult & 0x0000ff);
	int g = (cc.rgbResult & 0x00ff00) >> 8;
	int b = (cc.rgbResult & 0xff0000) >> 16;
	if (method)
		method(r, g, b);
}

void addCtrl(HWND hwnd, WndStyle style, MethodCtrlUpdate method)
{
	WndCtrl* c = &wc[wcNum];
	c->hwnd = hwnd;
	c->style = style;
	c->method = method;
	wcNum++;
}

void updateCtrl(WPARAM wParam, LPARAM lParam)
{
	int wndIndex = LOWORD(wParam);
	WndCtrl* c = &wc[wndIndex];
	switch (c->style)
	{
	case WndStyleStatic:
		break;

	case WndStyleButton:
	case WndStyleRadio:
	{
		if (c->method)
			c->method(c->hwnd);
		break;
	}

	case WndStyleCheckBox:
	{
		bool isChecked = getWndCheckBox(c->hwnd);
		setWndCheckBox(c->hwnd, !isChecked);

		if (c->method)
			c->method(c->hwnd);
		break;
	}

	case WndStyleComboBox:
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (c->method)
				c->method(c->hwnd);
		}
		break;
	}

	case WndStyleListBox:
	{
		if (HIWORD(wParam) == LBN_SELCHANGE)
		{
			if (c->method)
				c->method(c->hwnd);
		}
		else if (HIWORD(wParam) == LBN_DBLCLK)
		{
			printf("DBCLK = %d\n", indexWndListBox(c->hwnd));
		}
		break;
	}
	}
}

HWND createWndStatic(int x, int y, int width, int height, const char* str)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_STATIC, wstr,
		WS_CHILD | WS_VISIBLE | ES_CENTER,
		x, y, width, height, hWnd, (HMENU)wcNum, 
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addCtrl(hwnd, WndStyleStatic, nullptr);
	delete wstr;

	return hwnd;
}

HWND createWndButton(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addCtrl(hwnd, WndStyleButton, method);
	delete wstr;

	return hwnd;
}

HWND createWndCheckBox(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addCtrl(hwnd, WndStyleCheckBox, method);
	delete wstr;

	return hwnd;
}

bool getWndCheckBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

void setWndCheckBox(HWND hwnd, bool isChecked)
{
	SendMessage(hwnd, (UINT)BM_SETCHECK, isChecked ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

HWND createWndRadio(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addCtrl(hwnd, WndStyleRadio, method);
	delete wstr;

	return hwnd;
}

bool getWndRadio(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)BM_GETCHECK, (WPARAM)0, (LPARAM)0);
}

void setWndRadio(HWND hwnd, bool isChecked)
{
	SendMessage(hwnd, (UINT)BM_SETCHECK, isChecked ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
}

HWND createWndComboBox(int x, int y, int width, int height, const char** str, int strNum, MethodCtrlUpdate method)
{
	HWND hwnd = CreateWindow(WC_COMBOBOX, nullptr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | CBS_HASSTRINGS,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addWndComboBox(hwnd, str, strNum);
	setWndComboBox(hwnd, strNum - 1);
	addCtrl(hwnd, WndStyleComboBox, method);

	return hwnd;
}

void addWndComboBox(HWND hwnd, int index, const char* str)
{
	wchar_t* wstr = utf8_to_utf16(str);
	SendMessage(hwnd, (UINT)CB_INSERTSTRING, (WPARAM)index, (LPARAM)wstr);
	delete wstr;
}

void addWndComboBox(HWND hwnd, const char** str, int strNum)
{
	for (int i = 0; i < strNum; i++)
		addWndComboBox(hwnd, i, str[i]);
}

void removeWndComboBox(HWND hwnd, int index)
{
	SendMessage(hwnd, (UINT)CB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}

int indexWndComboBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

int CountWndComboBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

char* getWndComboBox(HWND hwnd, int index)
{
	wchar_t wstr[128];
	SendMessage(hwnd, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)wstr);
	return utf16_to_utf8(wstr);
}

void setWndComboBox(HWND hwnd, int index)
{
	SendMessage(hwnd, (UINT)CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

HWND createWndListBox(int x, int y, int width, int height, const char** str, int strNum, MethodCtrlUpdate method)
{
	HWND hwnd = CreateWindow(WC_LISTBOX, nullptr,
		WS_TABSTOP | 
		WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY |
		WS_HSCROLL | WS_VSCROLL,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addWndListBox(hwnd, str, strNum);
	setWndListBox(hwnd, strNum - 1);
	addCtrl(hwnd, WndStyleListBox, method);

	return hwnd;
}

void addWndListBox(HWND hwnd, int index, const char* str)
{
	wchar_t* wstr = utf8_to_utf16(str);
	SendMessage(hwnd, (UINT)LB_INSERTSTRING, (WPARAM)index, (LPARAM)wstr);
	delete wstr;
}

void addWndListBox(HWND hwnd, const char** str, int strNum)
{
	for (int i = 0; i < strNum; i++)
		addWndListBox(hwnd, i, str[i]);
}

void removeWndListBox(HWND hwnd, int index)
{
	SendMessage(hwnd, (UINT)LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
}

int indexWndListBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

int CountWndListBox(HWND hwnd)
{
	return SendMessage(hwnd, (UINT)LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
}

char* getWndListBox(HWND hwnd, int index)
{
	wchar_t wstr[128];
	SendMessage(hwnd, (UINT)LB_GETTEXT, (WPARAM)index, (LPARAM)wstr);
	return utf16_to_utf8(wstr);
}

void setWndListBox(HWND hwnd, int index)
{
	SendMessage(hwnd, (UINT)LB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

HWND createWndGroup(int x, int y, int width, int height, const char* str)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_BUTTON, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);
	addCtrl(hwnd, WndStyleButton, nullptr);
	delete wstr;

	return hwnd;
}

LRESULT CALLBACK editAllWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, WndEditStyle style)
{
	WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (wpOld)
	{
		switch (message)
		{
		case WM_NCDESTROY:
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)0);
			break;

		case WM_CHAR:
			if (style == WndEditStyleAll)
			{
				// ok
			}
			else if (style == WndEditStyleInt)
			{
				if (wParam == VK_RETURN || wParam == VK_BACK || wParam == '-' || wParam >= '0' && wParam <= '9')
				{
					// ok
				}
				else
				{
					return 0;
				}
			}
			else // if (style == WndEditStyleFloat)
			{
				if (wParam == '.')
				{
					wchar_t str[1024];
					int length = GetWindowTextLength(hwnd) + 1;
					GetWindowText(hwnd, str, length);

					for (int i = 0; i < length; i++)
					{
						if (str[i] == '.')
							return 0;
					}
				}
				else if (wParam == VK_RETURN || wParam == VK_BACK || wParam == '-' || wParam >= '0' && wParam <= '9')
				{
					// ok
				}
				else
				{
					return 0;
				}
			}

			break;
		}
	}
	return CallWindowProc(wpOld, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK editAllWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return editAllWndProc(hwnd, message, wParam, lParam, WndEditStyleAll);
}

LRESULT CALLBACK editIntWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return editAllWndProc(hwnd, message, wParam, lParam, WndEditStyleInt);
}

LRESULT CALLBACK editFloatWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return editAllWndProc(hwnd, message, wParam, lParam, WndEditStyleFloat);
}

HWND createWndEditBox(int x, int y, int width, int height, WndEditStyle style, 
	const char* str, int strLenMax, MethodCtrlUpdate method)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_EDIT, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);

	if (strLenMax)
		SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)strLenMax, (LPARAM)0);

	addCtrl(hwnd, WndStyleEditBox, method);
	delete wstr;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));

	typedef LRESULT(CALLBACK* MethodEdit)(HWND, UINT, WPARAM, LPARAM);
	MethodEdit m[3] = {editAllWndProc, editIntWndProc, editFloatWndProc };

	SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)m[style]);

	return hwnd;
}

LRESULT CALLBACK editMultilineWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wpOld = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (wpOld)
	{
		switch (message)
		{
		case WM_NCDESTROY:
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)wpOld);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)0);
			break;

		case WM_GETDLGCODE:
		{
			LRESULT ret = CallWindowProc(wpOld, hwnd, message, wParam, lParam);
			ret &= ~(DLGC_HASSETSEL | DLGC_WANTTAB);
			if (lParam && 
				((LPMSG)lParam)->message == WM_KEYDOWN && 
				((LPMSG)lParam)->wParam == VK_TAB)
				ret &= ~DLGC_WANTMESSAGE;
			return ret;
		}
		}
	}
	return CallWindowProc(wpOld, hwnd, message, wParam, lParam);
}

HWND createWndEditBoxMultiline(int x, int y, int width, int height, 
	const char* str, int strLenMax, MethodCtrlUpdate method)
{
	wchar_t* wstr = utf8_to_utf16(str);
	HWND hwnd = CreateWindow(WC_EDIT, wstr,
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, width, height, hWnd, (HMENU)wcNum,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);

	if (strLenMax)
		SendMessage(hwnd, EM_LIMITTEXT, (WPARAM)strLenMax, (LPARAM)0);
	addCtrl(hwnd, WndStyleEditBox, method);
	delete wstr;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, GetWindowLongPtr(hwnd, GWLP_WNDPROC));
	SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)editMultilineWndProc);

	return hwnd;
}

void enableWnd(HWND hwnd, bool isEnable)
{
	EnableWindow(hwnd, isEnable);
}

void setWndText(HWND hwnd, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	wchar_t* ws = utf8_to_utf16(szText);
	SetWindowText(hwnd, ws);
	delete ws;
}

char* getWndText(HWND hwnd)
{
	int length = GetWindowTextLength(hwnd) + 1;
	wchar_t* ws = new wchar_t[length];
	GetWindowText(hwnd, ws, length);
	char* s = utf16_to_utf8(ws);
	delete ws;

	return s;
}

int getWndInt(HWND hwnd)
{
	char* s = getWndText(hwnd);
	int n = atoi(s);
	delete s;
	return n;
}

float getWndFloat(HWND hwnd)
{
	char* s = getWndText(hwnd);
	float n = atof(s);
	delete s;
	return n;
}

class iCtrlOpenGL
{
public:
	iCtrlOpenGL(int x, int y, int width, int height)
	{
		hwnd = CreateWindow(WC_STATIC, nullptr,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x, y, width, height, hWnd, (HMENU)wcNum,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), nullptr);

		addCtrl(hwnd, WndStyleOpenGL, nullptr);

		hDC = GetDC(hwnd);

		// setup OpenGL
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
		if (GLEW_OK != err)
			printf("Error: %s\n", glewGetErrorString(err));

		if (wglewIsSupported("WGL_ARB_create_context"))
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(hRC);

			int attr[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 2,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0,
			};
			hRC = wglCreateContextAttribsARB(hDC, nullptr, attr);
			wglMakeCurrent(hDC, hRC);

			const char* strGL = (const char*)glGetString(GL_VERSION);
			const char* strGLSL = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			const char* strGLEW = (const char*)glewGetString(GLEW_VERSION);
			printf("GL(%s), GLSL(%s), GLEW(%s)\n", strGL, strGLSL, strGLEW);
		}

		// init OpenGL
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// default, for android
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_SRC);	// for ios, mac, window

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glFrontFace(GL_CCW); // GL_CW
		glDisable(GL_CULL_FACE);

		// Drawing Region
		devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
		// viewport -> resizeOpenGL

		glEnable(GL_BLEND);
		//setGlBlendFunc(iBlendFuncAlpha);
		glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Settings of Texture
		setTexture(Linear, Clamp);

		// Back or Front Buffer(Frame Buffer Object)
		fbo = new iFBO(width, height);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &vbe);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
		uint8 indices[6] = { 0,1,2, 1,2,3 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8) * 6, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		matrixProject = new iMatrix;
		matrixProject->loadIdentity();
		matrixProject->ortho(0, width, height, 0, -1, 1);

		matrixModelview = new iMatrix;
		matrixModelview->loadIdentity();
	}

	virtual ~iCtrlOpenGL()
	{

	}

	void paint(float dt)
	{
		// 점유

		// 화면 클리어

		if (methodUpdate)
			methodUpdate(dt);

		// 버퍼 스왑
	}

	void key(iKeyState state, iPoint p)
	{
		if (methodKey)
			methodKey(state, p);
	}

public:
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;

	iFBO* fbo;
	GLuint vao, vbo, vbe;
	iMatrix* matrixProject;
	iMatrix* matrixModelview;

	MethodUpdateGL methodUpdate;
	MethodKeyGL methodKey;
};

iCtrlOpenGL** openGLs = nullptr;
int numOpenGL;

HWND createWndOpenGL(int x, int y, int width, int height, MethodUpdateGL m0, MethodKeyGL m1)
{
	if (openGLs == nullptr)
	{
		openGLs = new iCtrlOpenGL * [100];
		numOpenGL = 0;
	}

	iCtrlOpenGL* ogl = new iCtrlOpenGL(x, y, width, height);
	ogl->methodUpdate = m0;
	ogl->methodKey = m1;
	openGLs[numOpenGL] = ogl;
	numOpenGL++;


	return ogl->hwnd;
}

void destroyAllWndOpenGL()
{
	for (int i = 0; i < numOpenGL; i++)
		delete openGLs[i];
	delete openGLs;
}

void drawWndOpenGL(float dt)
{
	for (int i = 0; i < numOpenGL; i++)
		openGLs[i]->paint(dt);
}

void keyWndOpenGL(iKeyState state, iPoint p)
{
	for (int i = 0; i < numOpenGL; i++)
	{
		iPoint newP; // from p int opengl[i]->hwnd
		openGLs[i]->key(state, newP);
	}
}
