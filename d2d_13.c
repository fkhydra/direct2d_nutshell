#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")
#include <Wincodec.h>
#pragma comment(lib, "Windowscodecs")

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;

#define HIBA_00 TEXT("Error:Program initialisation process.")
HINSTANCE hInstGlob;
int SajatiCmdShow;
char szClassName[] = "WindowsApp";
HWND Form1; //Window handler

LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
void D2D_drawing(ID2D1HwndRenderTarget* pRT);

//*********************************
//Program entry point
//*********************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
 static TCHAR szAppName[] = TEXT("StdWinClassName");
 HWND hwnd;
 MSG msg;
 WNDCLASS wndclass0;
 SajatiCmdShow = iCmdShow;
 hInstGlob = hInstance;

 //*********************************
 //Preparing the window instance
 //*********************************
 wndclass0.style = CS_HREDRAW | CS_VREDRAW;
 wndclass0.lpfnWndProc = WndProc0;
 wndclass0.cbClsExtra = 0;
 wndclass0.cbWndExtra = 0;
 wndclass0.hInstance = hInstance;
 wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
 wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
 wndclass0.lpszMenuName = NULL;
 wndclass0.lpszClassName = TEXT("WIN0");

 //*********************************
 //Registering the application window
 //*********************************
 if (!RegisterClass(&wndclass0))
 {
 MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
return 0;
 }

 //*********************************
 //Creating the window
 //*********************************
 Form1 = CreateWindow(TEXT("WIN0"),
  TEXT("Form1"),
  (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
  50,
  50,
  800,
  600,
  NULL,
  NULL,
  hInstance,
  NULL);

 //*********************************
 //Display the window
 //*********************************
 ShowWindow(Form1, SajatiCmdShow);
 UpdateWindow(Form1);

 //*********************************
 //Message handling loop
 //*********************************
 while (GetMessage(&msg, NULL, 0, 0))
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return msg.wParam;
}

//*********************************
//The window’s callback function
//*********************************
LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 HDC hdc;
 PAINTSTRUCT ps;

 switch (message)
 {
 //*********************************
 //When creating the window
 //*********************************
 case WM_CREATE:
  /*Init*/;
  D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,   &pD2DFactory);
  pD2DFactory->CreateHwndRenderTarget(
   D2D1::RenderTargetProperties(),
   D2D1::HwndRenderTargetProperties(
    hwnd,D2D1::SizeU(800,600) ),
   &pRT);
  return 0;
 //*********************************
 //Redrawing the client area of the window
 //*********************************
 case WM_PAINT:
  hdc = BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);
  D2D_drawing(pRT);
  return 0;
 //*********************************
 //Closing the window, releasing ressources
 //*********************************
 case WM_CLOSE:
  pRT->Release();
  pD2DFactory->Release();
  DestroyWindow(hwnd);
  return 0;
 //*********************************
 //Destroy window
 //*********************************
 case WM_DESTROY:
  PostQuitMessage(0);
  return 0;
 }
 return DefWindowProc(hwnd, message, wParam, lParam);
}

void D2D_drawing(ID2D1HwndRenderTarget* pRT)
{
	IWICImagingFactory *pFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;

	RECT rc; GetClientRect(Form1, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(Form1, size),
		&pRT);

	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pFactory
	);

	pFactory->CreateDecoderFromFilename(
		L"bitmap2.jpg",
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);

	IWICBitmapFrameDecode *pSource;
	IWICFormatConverter *pConverter;
	pDecoder->GetFrame(0, &pSource);
	pFactory->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	ID2D1Bitmap *ppBitmap;
	pRT->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&ppBitmap
	);

	ID2D1BitmapBrush *m_pBitmapBrush;
	pRT->CreateBitmapBrush(
		ppBitmap,
		&m_pBitmapBrush
	);
	m_pBitmapBrush->SetExtendModeX(D2D1_EXTEND_MODE_MIRROR);

	D2D1_RECT_F rc2;
	rc2.left = rc.left;
	rc2.top = rc.top;
	rc2.right = rc.right;
	rc2.bottom = rc.bottom;
	pRT->FillRectangle(&rc2, m_pBitmapBrush);

	pRT->EndDraw();
}
