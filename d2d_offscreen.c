#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")
#include <time.h>

#define KEPERNYO_WIDTH 1900
#define KEPERNYO_HEIGHT 1000

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;

D2D1_RECT_U terulet;
ID2D1Bitmap* memkeptarolo = NULL;
ID2D1BitmapRenderTarget *pD2D_offscreen_puffer = NULL;

#define HIBA_00 TEXT("Error:Program initialisation process.")
HINSTANCE hInstGlob;
int SajatiCmdShow;
char szClassName[] = "WindowsApp";
HWND Form1; //Ablak kezeloje

LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
void D2D_rajzolas(ID2D1HwndRenderTarget* pRT);

void create_main_buffer(void);
void cleanup_main_buffer(void);
void swap_main_buffer(void);
int getrandom(int maxnum);

//*********************************
//A windows program belépési pontja
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
//Ablak osztálypéldány elokészítése
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
//Ablak osztálypéldány regisztrációja
//*********************************
if (!RegisterClass(&wndclass0))
{
 MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
 return 0;
}

//*********************************
//Ablak létrehozása
//*********************************
Form1 = CreateWindow(TEXT("WIN0"),
 TEXT("Form1"),
 WINSTYLE_DIALOG,
 50,
 50,
 KEPERNYO_WIDTH,
 KEPERNYO_HEIGHT,
 NULL,
 NULL,
 hInstance,
 NULL);

//*********************************
//Ablak megjelenítése
//*********************************
ShowWindow(Form1, SajatiCmdShow);
UpdateWindow(Form1);

//*********************************
//Ablak üzenetkezelésének aktiválása
//*********************************
while (GetMessage(&msg, NULL, 0, 0))
{
 TranslateMessage(&msg);
 DispatchMessage(&msg);
}
return msg.wParam;
}

//*********************************
//Az ablak callback függvénye: eseménykezelés
//*********************************
LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HDC hdc;
PAINTSTRUCT ps;

switch (message)
{
 //*********************************
 //Ablak létrehozásakor közvetlenül
 //*********************************
case WM_CREATE:
 /*Init*/;
 D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
 pD2DFactory->CreateHwndRenderTarget(
  D2D1::RenderTargetProperties(),
  D2D1::HwndRenderTargetProperties(
   hwnd, D2D1::SizeU(KEPERNYO_WIDTH, KEPERNYO_HEIGHT)),
  &pRT);
 pRT->CreateCompatibleRenderTarget(
  D2D1::SizeF(KEPERNYO_WIDTH, KEPERNYO_HEIGHT),
  &pD2D_offscreen_puffer);
 srand((unsigned)time(NULL));
 create_main_buffer();
 return 0;
 //*********************************
 //Ablak kliens területének újrarajzolása
 //*********************************
case WM_PAINT:
 hdc = BeginPaint(hwnd, &ps);
 EndPaint(hwnd, &ps);
 D2D_rajzolas(pRT);
 return 0;
 //*********************************
 //Ablak bezárása, eroforrások felszabadítása
 //*********************************
case WM_CLOSE:
 pRT->Release();
 pD2DFactory->Release();
 pD2D_offscreen_puffer->Release();
 DestroyWindow(hwnd);
 return 0;
 //*********************************
 //Ablak megsemmisítése
 //*********************************
case WM_DESTROY:
 PostQuitMessage(0);
 return 0;
}
return DefWindowProc(hwnd, message, wParam, lParam);
}

void D2D_rajzolas(ID2D1HwndRenderTarget* pRT)
{
cleanup_main_buffer();

/*Ide jöhetnek rajzkódok*/

swap_main_buffer();
}

int getrandom(int maxnum)
{
return (double)rand() / (RAND_MAX + 1) * maxnum;
}

void create_main_buffer(void)
{
pRT->CreateBitmap(D2D1::SizeU(KEPERNYO_WIDTH, KEPERNYO_HEIGHT),
 D2D1::BitmapProperties(D2D1::PixelFormat
 (DXGI_FORMAT_R8G8B8A8_UNORM,
  D2D1_ALPHA_MODE_IGNORE)), &memkeptarolo);
}

void cleanup_main_buffer(void)
{
pD2D_offscreen_puffer->BeginDraw();
pD2D_offscreen_puffer->Clear(D2D1::ColorF(D2D1::ColorF(0.9f,0.5f,0.5f,1.0f)));
pD2D_offscreen_puffer->EndDraw();
}

void swap_main_buffer(void)
{
terulet.left = 0;
terulet.top = 0;
terulet.right = KEPERNYO_WIDTH - 1;
terulet.bottom = KEPERNYO_HEIGHT - 1;

pD2D_offscreen_puffer->GetBitmap(&memkeptarolo);
pRT->BeginDraw();
pRT->DrawBitmap(memkeptarolo, D2D1::RectF(0.0f, 0.0f, KEPERNYO_WIDTH - 1, KEPERNYO_HEIGHT - 1), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, NULL);
pRT->EndDraw();
}
