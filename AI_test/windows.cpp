#include "windows.h"

// ---------------------------------------------------------------------------
//  コンストラクタ / デストラクタ
// ---------------------------------------------------------------------------
GameWindow::GameWindow()
    : m_hwnd(nullptr)
    , m_hInstance(nullptr)
    , m_className(L"DirectX11GameWindow")
    , m_width(0)
    , m_height(0)
{
}

GameWindow::~GameWindow()
{
}

// ---------------------------------------------------------------------------
//  Initialize – ウィンドウクラスの登録とウィンドウの生成
// ---------------------------------------------------------------------------
bool GameWindow::Initialize(const std::wstring& title, int width, int height)
{
    m_width    = width;
    m_height   = height;
    m_hInstance = GetModuleHandle(nullptr);

    // ウィンドウクラスの登録
    WNDCLASSEX wc = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = m_hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszClassName = m_className.c_str();

    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    // クライアント領域が指定サイズになるようにウィンドウ矩形を調整
    RECT rc = { 0, 0, m_width, m_height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // ウィンドウの生成
    m_hwnd = CreateWindowEx(
        0,
        m_className.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr
    );

    if (!m_hwnd)
    {
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    return true;
}

// ---------------------------------------------------------------------------
//  Shutdown – ウィンドウの破棄とクラスの登録解除
// ---------------------------------------------------------------------------
void GameWindow::Shutdown()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    if (m_hInstance)
    {
        UnregisterClass(m_className.c_str(), m_hInstance);
    }
}

// ---------------------------------------------------------------------------
//  ProcessMessages – Windows メッセージポンプ
//  戻り値: false → WM_QUIT を受信したのでアプリを終了する
// ---------------------------------------------------------------------------
bool GameWindow::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

// ---------------------------------------------------------------------------
//  WindowProc – ウィンドウプロシージャ
// ---------------------------------------------------------------------------
LRESULT CALLBACK GameWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        // ESC キーでアプリ終了
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
