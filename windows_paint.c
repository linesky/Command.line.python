#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
//./gcc paint.c -o paint -lgdi32 -luser32 -mwindows
#define PIXEL_SIZE 8
#define GRID_SIZE 32
#define NUM_COLORS 16
// Definição da paleta VGA
COLORREF PALETA_VGA[NUM_COLORS] = {
    RGB(0, 0, 0), RGB(0, 0, 170), RGB(0, 170, 0), RGB(0, 170, 170),
    RGB(170, 0, 0), RGB(170, 0, 170), RGB(170, 85, 0), RGB(170, 170, 170),
    RGB(85, 85, 85), RGB(85, 85, 255), RGB(85, 255, 85), RGB(85, 255, 255),
    RGB(255, 85, 85), RGB(255, 85, 255), RGB(255, 255, 85), RGB(255, 255, 255)
};
// Variáveis globais
COLORREF bitmap[GRID_SIZE][GRID_SIZE];
COLORREF current_color = RGB(255, 255, 255);
HINSTANCE hInst;
HWND hwndCanvas;
HWND hwndColorButton[20];
int tt=0;
int ttt=0;
HDC hdc ;
HBRUSH brush;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawGrid(HDC hdc);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    HWND hwndColorButton;
    HBRUSH brush;
    
    // Registro da classe da janela
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "SimpleButtonWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Criação da janela
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "SimpleButtonWindowClass",
        "Simple Window with Button",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
     UpdateWindow(hwnd);
           
    
    
    

    // Loop de mensagens
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}



void DrawGrid(HDC hdc) {
    HBRUSH brush;
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            brush = CreateSolidBrush(bitmap[y][x]);
            RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x + 1) * PIXEL_SIZE, (y + 1) * PIXEL_SIZE };
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
        }
    }
}

void SaveBitmaps(HWND hwnd) {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "bmp";

    if (GetSaveFileName(&ofn)) {
        BITMAPFILEHEADER fileHeader;
        BITMAPINFOHEADER infoHeader;
        FILE *file;

        fileHeader.bfType = 0x4D42; // BM
        fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + GRID_SIZE * GRID_SIZE * 3;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        infoHeader.biSize = sizeof(BITMAPINFOHEADER);
        infoHeader.biWidth = GRID_SIZE;
        infoHeader.biHeight = -GRID_SIZE;
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = 24;
        infoHeader.biCompression = BI_RGB;
        infoHeader.biSizeImage = 0;
        infoHeader.biXPelsPerMeter = 0;
        infoHeader.biYPelsPerMeter = 0;
        infoHeader.biClrUsed = 0;
        infoHeader.biClrImportant = 0;

        file = fopen(fileName, "wb");
        fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
        fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                COLORREF color = bitmap[y][x];
                BYTE rgb[3] = { GetBValue(color), GetGValue(color), GetRValue(color) };
                fwrite(rgb, sizeof(BYTE), 3, file);
            }
        }
        fclose(file);
    }
}

void LoadBitmaps(HWND hwnd) {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "bmp";

    if (GetOpenFileName(&ofn)) {
        FILE *file = fopen(fileName, "rb");
        if (file) {
            BITMAPFILEHEADER fileHeader;
            BITMAPINFOHEADER infoHeader;
            fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
            fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
            hdc = GetDC(hwndCanvas);
            for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                    BYTE rgb[3];
                    fread(rgb, sizeof(BYTE), 3, file);
                    bitmap[y][x] = RGB(rgb[2], rgb[1], rgb[0]);
                    hdc = GetDC(hwndCanvas);
                    brush = CreateSolidBrush( RGB(rgb[2], rgb[1], rgb[0]));
               
                        
                        RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x+1) * PIXEL_SIZE, (y+1) * PIXEL_SIZE };
                        FillRect(hdc, &rect, brush);
                        DeleteObject(brush);
                    }
                }
                
            ReleaseDC(hwnd, hdc);
                
            }
            fclose(file);
            InvalidateRect(hwndCanvas, NULL, TRUE);
        
    }
}

LRESULT CALLBACK  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // WndProc
   
    
    
    
    char sss[100];
    switch (msg) {
        case WM_CREATE:
            hwndCanvas = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE,
                10, 10, GRID_SIZE * PIXEL_SIZE, GRID_SIZE * PIXEL_SIZE, hwnd, NULL, hInst, NULL);
                
            for (int i = 0; i < NUM_COLORS; i++) {
                sprintf(sss,"%d",i);
                hwndColorButton[i] = CreateWindow("BUTTON", "", WS_CHILD |  WS_VISIBLE | BS_PUSHBUTTON,
                    10 + i * 30, GRID_SIZE * PIXEL_SIZE + 20, 20, 20, hwnd, (HMENU)(i + 1), hInst, NULL);
                hdc = GetDC(hwndColorButton[i]);
                brush = CreateSolidBrush(PALETA_VGA[i]);
                RECT rect = { 0, 0, 20, 20 };
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);
                ReleaseDC(hwndColorButton[i], hdc);
                
            }

            CreateWindow( "BUTTON", "New", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, GRID_SIZE * PIXEL_SIZE + 50, 60, 30, hwnd, (HMENU)100, hInst, NULL);
            CreateWindow( "BUTTON", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                80, GRID_SIZE * PIXEL_SIZE + 50, 60, 30, hwnd, (HMENU)101, hInst, NULL);
            CreateWindow( "BUTTON", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, GRID_SIZE * PIXEL_SIZE + 50, 60, 30, hwnd, (HMENU)102, hInst, NULL);
                hdc = GetDC(hwndCanvas);
                brush = CreateSolidBrush(current_color);
                for (int y = 0; y < GRID_SIZE; y++) {
                    for (int x = 0; x < GRID_SIZE; x++) {
                        bitmap[y][x] = RGB(0, 0, 0);
                        RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x+1) * PIXEL_SIZE, (y+1) * PIXEL_SIZE };
                        FillRect(hdc, &rect, brush);
                    }
                }
                DeleteObject(brush);
            ReleaseDC(hwnd, hdc);
            tt=1;
            ttt=1;
            break;
        case WM_PAINT:
        if (tt==1){
                for (int i = 0; i < NUM_COLORS; i++) {
                hdc = GetDC(hwndColorButton[i]);
                brush = CreateSolidBrush(PALETA_VGA[i]);
                RECT rect = { 0, 0, 20, 20 };
                FillRect(hdc, &rect, brush);
                DeleteObject(brush);
                ReleaseDC(hwndColorButton[i], hdc);
                }
        }
        if (ttt==1){
            ttt=2;
              hdc = GetDC(hwndCanvas);
                brush = CreateSolidBrush(current_color);
                for (int y = 0; y < GRID_SIZE; y++) {
                    for (int x = 0; x < GRID_SIZE; x++) {
                        bitmap[y][x] = current_color;
                        RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x+1) * PIXEL_SIZE, (y+1) * PIXEL_SIZE };
                        FillRect(hdc, &rect, brush);
                    }
                }
                DeleteObject(brush);
            ReleaseDC(hwnd, hdc);
        
        }
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) >= 1 && LOWORD(wParam) <= NUM_COLORS) {
                current_color = PALETA_VGA[LOWORD(wParam) - 1];
            } else if (LOWORD(wParam) == 100) {
                // New bitmap
                hdc = GetDC(hwndCanvas);
                brush = CreateSolidBrush(current_color);
                for (int y = 0; y < GRID_SIZE; y++) {
                    for (int x = 0; x < GRID_SIZE; x++) {
                        bitmap[y][x] = RGB(0, 0, 0);
                        RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x+1) * PIXEL_SIZE, (y+1) * PIXEL_SIZE };
                        FillRect(hdc, &rect, brush);
                    }
                }
                DeleteObject(brush);
                ReleaseDC(hwnd, hdc);
                //InvalidateRect(hwndCanvas, NULL, TRUE);
            } else if (LOWORD(wParam) == 101) {
                // Save bitmap
                SaveBitmaps(hwnd);
            } else if (LOWORD(wParam) == 102) {
                // Load bitmap
                LoadBitmaps(hwnd);
            }
            break;

        
        case WM_LBUTTONDOWN:
        
            if (wParam & MK_LBUTTON) {
                int x = (LOWORD(lParam)-10) / PIXEL_SIZE;
                int y = (HIWORD(lParam)-10) / PIXEL_SIZE;
                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    bitmap[y][x] = current_color;
                    HDC hdc = GetDC(hwndCanvas);
                    HBRUSH brush = CreateSolidBrush(current_color);
                    RECT rect = { x * PIXEL_SIZE, y * PIXEL_SIZE, (x+1) * PIXEL_SIZE, (y+1) * PIXEL_SIZE };
                    FillRect(hdc, &rect, brush);
                    DeleteObject(brush);
                }
            }
            
            break;
            case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}