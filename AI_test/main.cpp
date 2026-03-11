#include "application.h"

// ---------------------------------------------------------------------------
//  WinMain – アプリケーションのエントリーポイント
// ---------------------------------------------------------------------------
int WINAPI WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPSTR     lpCmdLine,
    _In_     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    Application* app = new Application();

    if (app->Initialize())
    {
        app->Run();
    }

    app->Shutdown();
    delete app;
    app = nullptr;

    return 0;
}
