#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "windows.h"
#include "direct3D.h"

// ゲームアプリケーション本体クラス
class Application
{
public:
    Application();
    ~Application();

    // アプリケーションの初期化
    bool Initialize();

    // ゲームループの実行
    void Run();

    // アプリケーションの終了処理
    void Shutdown();

private:
    // ゲームロジックの更新（deltaTime: 前フレームからの経過時間[秒]）
    void Update(float deltaTime);

    // 描画処理
    void Render();

    GameWindow* m_window;
    Direct3D*   m_direct3D;

    // 可変フレームレート用タイマー
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_lastTime;
};
