#pragma warning (disable: 28251)

#include <windows.h>
#include <vector>
#include <string>

#ifndef UNICODE
#define UNICODE
#endif 

#define COLS 4
#define ROWS 3

using namespace std;

vector<wstring> strings;

VOID OnPaint(HWND hWnd, LONG clientWidth, LONG clientHeight)
{    
    int padding = 10;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    INT colWidth = (INT)(clientWidth / COLS);
    INT* rowHeights = new INT[ROWS + 1];

    RECT cellRect = RECT();
    int maxRowHeight = 0;
    int currRowHeight = 0;
    int stringIndex = 0;
    int topRowY = 0;
    for (int row = 0; row < ROWS; row++)
    {
        maxRowHeight = 10;
        int leftColumnX = 0;
        int rightColumnX = colWidth;
        for (int col = 0; col < COLS; col++)
        {
            cellRect.left = leftColumnX + padding;
            cellRect.right = rightColumnX - padding;
            cellRect.top = topRowY + padding;
            cellRect.bottom = topRowY + 1000;

            if (stringIndex < strings.size())
            {
                currRowHeight = DrawText(hdc, strings.at(stringIndex++).c_str(), -1, &cellRect, DT_WORDBREAK | DT_TOP);
            }                       
            
            if (currRowHeight > maxRowHeight)
            {
                maxRowHeight = currRowHeight;
            }

            leftColumnX += colWidth;
            rightColumnX += colWidth;
        }

        if (row == 0)
        {
            rowHeights[row] = maxRowHeight + padding + padding;
        }
        else
        {
            rowHeights[row] = rowHeights[row - 1] + maxRowHeight + padding + padding;
        }
        topRowY += maxRowHeight + padding + padding;
    }

    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, clientWidth, 0);

    for (int col = 1; col < COLS; col++)
    {
        MoveToEx(hdc, col * colWidth, 0, NULL);
        LineTo(hdc, col * colWidth, clientHeight);
    }

    for (int row = 0; row < ROWS; row++)
    {
        MoveToEx(hdc, 0, rowHeights[row], NULL);
        LineTo(hdc, clientWidth, rowHeights[row]);
    }

    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    strings.emplace_back(L"How much wood would a woodchuck chuck if a woodchuck could chuck wood? He would chuck, he would, as much as he could, and chuck as much wood As a woodchuck would if a woodchuck could chuck wood");
    strings.emplace_back(L"She sells seashells by the seashore");
    strings.emplace_back(L"Storm in a teacup");
    strings.emplace_back(L"I scream, you scream, we all scream for ice cream");
    strings.emplace_back(L"Red lorry, yellow lorry");
    strings.emplace_back(L"Black sheep");
    strings.emplace_back(L"How can a clam cram in a clean cream can?");
    strings.emplace_back(L"Peter Piper picked a peck of pickled peppers A peck of pickled peppers Peter Piper picked If Peter Piper picked a peck of pickled peppers Where’s the peck of pickled peppers Peter Piper picked?");
    strings.emplace_back(L"Pad kid poured curd pulled cod");
    strings.emplace_back(L"Six sick hicks nick six slick bricks with picks and sticks");
    strings.emplace_back(L"Until the cows come home");
    //strings.emplace_back(L"If two witches were watching two watches, which witch would watch which watch?");
    //strings.emplace_back(L"Tongue twisters don't twist your tongue, they twsit your brain");
    
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("Responsive table by Ralovets"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    static LONG  clientWidth = 800;
    static LONG  clientHeight = 600;

    switch (message)
    {
    case WM_PAINT:
    {
        OnPaint(hWnd, clientWidth, clientWidth);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        clientWidth = LOWORD(lParam);
        clientHeight = HIWORD(lParam);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}