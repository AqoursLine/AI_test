#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

// ウィンドウ管理クラス
class GameWindow
{
public:
    GameWindow();
    ~GameWindow();

    // ウィンドウの初期化
    bool Initialize(const std::wstring& title, int width, int height);

    // ウィンドウの終了処理
    void Shutdown();

    // メッセージ処理（falseを返したらアプリ終了）
    bool ProcessMessages();

    // ゲッター
    HWND    GetHwnd()   const { return m_hwnd; }
    int     GetWidth()  const { return m_width; }
    int     GetHeight() const { return m_height; }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND         m_hwnd;
    HINSTANCE    m_hInstance;
    std::wstring m_className;
    int          m_width;
    int          m_height;
};
