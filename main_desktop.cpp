#include "gui_glut.h"
#include <windows.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Win32App app;
    if (!app.InitMySQL("127.0.0.1", "root", "134236tarU_", "imagecrypto")) {
        MessageBoxA(NULL, "Database connection failed.", "Error", MB_OK);
        return 1;
    }
    app.Run(hInstance);
    return 0;
}