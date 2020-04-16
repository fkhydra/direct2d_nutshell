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
	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	IWICImagingFactory *pFactory = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&pFactory
	);

	pFactory->CreateDecoderFromFilename(
		L"bitmap.jpg",
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,//WICDecodeMetadataCacheOnDemand
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
		WICBitmapPaletteTypeMedianCut);

	ID2D1Bitmap *ppBitmap;
	pRT->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		&ppBitmap);

	pRT->DrawBitmap(
		ppBitmap,
		D2D1::RectF(
			100,
			100,
			555,
			420));

	pRT->EndDraw();
}
