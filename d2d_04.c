#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")

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

	ID2D1SolidColorBrush *mybrush;
	pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black, 1.0f),
		& mybrush);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(100.f, 100.f),
		75.f,
		50.f
	);
	pRT->SetTransform(D2D1::Matrix3x2F::Translation(0, 0));
	pRT->FillEllipse(ellipse, mybrush);
	pRT->SetTransform(D2D1::Matrix3x2F::Translation(200, 10));
	pRT->DrawEllipse(ellipse, mybrush, 1.0f);

	pRT->EndDraw();
}

