#pragma once
#include <windows.h>
#include <string>
#include <mysql.h>

class Win32App {
private:
    MYSQL* conn;
    HWND hwnd;
    HWND hPasswordLabel;
    HWND hPasswordInput;
    HWND hIPLabel;
    HWND hIPInput;
    HWND hBtnReceive;
    HWND hBtnEncrypt;
    HWND hBtnSend;
    HWND hStatus;
    std::string password;
    std::string receiverIP;

public:
    static Win32App* instance;
    Win32App();
    bool InitMySQL(const std::string& host, const std::string& user,
        const std::string& pass, const std::string& db);
    void LogToMySQL(const std::string& op, const std::string& status);
    void SetStatus(const std::string& msg);
    void CreateControls(HWND hwnd);
    void HandleCommand(WPARAM wParam);
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void Run(HINSTANCE hInstance);
    ~Win32App();
};