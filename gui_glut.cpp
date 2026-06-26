#include "gui_glut.h"
#include "Encrypt.h"
#include "sender.h"
#include "receiver.h"
#include <iostream>
#include <ctime>

Win32App* Win32App::instance = nullptr;

Win32App::Win32App() {
    conn = mysql_init(NULL);
    instance = this;
}

bool Win32App::InitMySQL(const std::string& host, const std::string& user,
    const std::string& pass, const std::string& db) {
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), pass.c_str(),
        db.c_str(), 0, NULL, 0)) {
        std::cerr << "Database Connection Error: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::string q = "CREATE TABLE IF NOT EXISTS system_crypto_audit ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "timestamp VARCHAR(50), "
        "operation VARCHAR(50), "
        "status VARCHAR(50));";
    mysql_query(conn, q.c_str());
    return true;
}

void Win32App::LogToMySQL(const std::string& op, const std::string& status) {
    time_t now = time(0);
    std::string ts = ctime(&now);
    if (!ts.empty()) ts.pop_back();
    std::string q = "INSERT INTO system_crypto_audit (timestamp, operation, status) VALUES ('"
        + ts + "', '" + op + "', '" + status + "');";
    mysql_query(conn, q.c_str());
}

void Win32App::SetStatus(const std::string& msg) {
    SetWindowTextA(hStatus, msg.c_str());
}

void Win32App::CreateControls(HWND hw) {
    hwnd = hw;

    CreateWindowA("STATIC", "Password:", WS_VISIBLE | WS_CHILD,
        20, 20, 100, 20, hw, NULL, NULL, NULL);

    hPasswordInput = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
        130, 18, 300, 24, hw, NULL, NULL, NULL);

    CreateWindowA("STATIC", "Receiver IP:", WS_VISIBLE | WS_CHILD,
        20, 60, 100, 20, hw, NULL, NULL, NULL);

    hIPInput = CreateWindowA("EDIT", "127.0.0.1", WS_VISIBLE | WS_CHILD | WS_BORDER,
        130, 58, 300, 24, hw, NULL, NULL, NULL);

    hBtnReceive = CreateWindowA("BUTTON", "Start Receiver (Listen and Decrypt)",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        20, 100, 420, 35, hw, (HMENU)1, NULL, NULL);

    hBtnEncrypt = CreateWindowA("BUTTON", "Encrypt Image (test.jpg -> locked.bin)",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        20, 150, 420, 35, hw, (HMENU)2, NULL, NULL);

    hBtnSend = CreateWindowA("BUTTON", "Send Encrypted Image to Receiver",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        20, 200, 420, 35, hw, (HMENU)3, NULL, NULL);

    hStatus = CreateWindowA("STATIC", "Ready.", WS_VISIBLE | WS_CHILD,
        20, 250, 420, 20, hw, NULL, NULL, NULL);
}

void Win32App::HandleCommand(WPARAM wParam) {
    char buf[256];

    switch (LOWORD(wParam)) {
    case 1: {
        GetWindowTextA(hPasswordInput, buf, 256);
        password = buf;
        if (password.empty()) { SetStatus("Error: Password is empty."); return; }
        LogToMySQL("RECEIVER_START", "ACTIVE");
        SetStatus("Listening on port 8080...");
        NetworkReceiver receiver;
        if (receiver.ListenAndDecryptPayload(password, "final.jpg")) {
            LogToMySQL("DECRYPT", "SUCCESS");
            SetStatus("Decryption complete. final.jpg saved.");
        }
        else {
            LogToMySQL("DECRYPT", "FAILED");
            SetStatus("Error: Decryption failed.");
        }
        break;
    }
    case 2: {
        GetWindowTextA(hPasswordInput, buf, 256);
        password = buf;
        if (password.empty()) { SetStatus("Error: Password is empty."); return; }
        LogToMySQL("ENCRYPT", "ACTIVE");
        ImageEncryptor enc(password);
        if (enc.EncryptImage("test.jpg", "locked.bin")) {
            LogToMySQL("ENCRYPT", "SUCCESS");
            SetStatus("Encryption complete. locked.bin ready.");
        }
        else {
            LogToMySQL("ENCRYPT", "FAILED");
            SetStatus("Error: test.jpg not found.");
        }
        break;
    }
    case 3: {
        GetWindowTextA(hIPInput, buf, 256);
        receiverIP = buf;
        if (receiverIP.empty()) { SetStatus("Error: Receiver IP is empty."); return; }
        LogToMySQL("SEND", "ACTIVE");
        NetworkSender sender;
        if (sender.DispatchPayload("locked.bin", receiverIP)) {
            LogToMySQL("SEND", "SUCCESS");
            SetStatus("Image sent successfully.");
        }
        else {
            LogToMySQL("SEND", "FAILED");
            SetStatus("Error: Send failed.");
        }
        break;
    }
    }
}

LRESULT CALLBACK Win32App::WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        instance->CreateControls(hw);
        return 0;
    case WM_COMMAND:
        instance->HandleCommand(wp);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hw, msg, wp, lp);
}

void Win32App::Run(HINSTANCE hInstance) {
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ImageCryptoApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    HWND hw = CreateWindowA("ImageCryptoApp", "Image Cryptography System - AES-128",
        WS_OVERLAPPEDWINDOW, 100, 100, 480, 320,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hw, SW_SHOW);
    UpdateWindow(hw);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

Win32App::~Win32App() {
    if (conn) mysql_close(conn);
}