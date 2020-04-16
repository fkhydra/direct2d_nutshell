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
	RECT rc;
	GetClientRect(Form1, &rc);
	D2D1_RECT_F rectangle;
	rectangle.left = 0;
	rectangle.top = 0;
	rectangle.right = 800;
	rectangle.bottom = 600;
	ID2D1LinearGradientBrush *mybrush;
	ID2D1GradientStopCollection *pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
	gradientStops[1].position = 1.0f;

	pRT->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops );

	pRT->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(0, 100),
			D2D1::Point2F(rectangle.right - 20, 150)),
		pGradientStops,
		&mybrush);
	
	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	pRT->FillRectangle(&rectangle, mybrush);
	pRT->EndDraw();
}
