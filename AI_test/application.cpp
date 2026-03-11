#include "application.h"

// ---------------------------------------------------------------------------
//  コンストラクタ / デストラクタ
// ---------------------------------------------------------------------------
Application::Application()
    : m_window(nullptr)
    , m_direct3D(nullptr)
{
    m_frequency.QuadPart = 0;
    m_lastTime.QuadPart  = 0;
}

Application::~Application()
{
}

// ---------------------------------------------------------------------------
//  Initialize – ウィンドウと DirectX 11 の初期化
// ---------------------------------------------------------------------------
bool Application::Initialize()
{
    // ウィンドウの初期化
    m_window = new GameWindow();
    if (!m_window->Initialize(L"DirectX 11 Game", 1280, 720))
    {
        return false;
    }

    // DirectX 11 の初期化
    m_direct3D = new Direct3D();
    if (!m_direct3D->Initialize(m_window->GetHwnd(),
                                m_window->GetWidth(),
                                m_window->GetHeight()))
    {
        return false;
    }

    // 可変フレームレート用タイマーの初期化
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_lastTime);

    return true;
}

// ---------------------------------------------------------------------------
//  Run – メインゲームループ（可変フレームレート）
// ---------------------------------------------------------------------------
void Application::Run()
{
    while (m_window->ProcessMessages())
    {
        // 前フレームからの経過時間（デルタタイム）を計算
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        float deltaTime = static_cast<float>(
            currentTime.QuadPart - m_lastTime.QuadPart) /
            static_cast<float>(m_frequency.QuadPart);

        m_lastTime = currentTime;

        // ゲームロジックの更新
        Update(deltaTime);

        // 描画
        Render();
    }
}

// ---------------------------------------------------------------------------
//  Shutdown – リソースの解放
// ---------------------------------------------------------------------------
void Application::Shutdown()
{
    if (m_direct3D)
    {
        m_direct3D->Shutdown();
        delete m_direct3D;
        m_direct3D = nullptr;
    }

    if (m_window)
    {
        m_window->Shutdown();
        delete m_window;
        m_window = nullptr;
    }
}

// ---------------------------------------------------------------------------
//  Update – ゲームロジックの更新
//  deltaTime: 前フレームからの経過時間（秒）
//  ここにゲームオブジェクトの移動・当たり判定などを追加してください
// ---------------------------------------------------------------------------
void Application::Update(float deltaTime)
{
    // TODO: ゲームロジックをここに追加
    (void)deltaTime; // 未使用警告の抑制
}

// ---------------------------------------------------------------------------
//  Render – 描画処理
//  ここにスプライト・3Dモデルなどの描画コードを追加してください
// ---------------------------------------------------------------------------
void Application::Render()
{
    // バックバッファのクリア（コーンフラワーブルー）
    m_direct3D->BeginScene(0.392f, 0.584f, 0.929f, 1.0f);

    // TODO: 描画コードをここに追加

    // フレームの終了（スワップチェーンのプレゼント）
    m_direct3D->EndScene();
}
