#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// DirectX 11 管理クラス
class Direct3D
{
public:
    Direct3D();
    ~Direct3D();

    // DirectX 11 の初期化
    bool Initialize(HWND hwnd, int width, int height);

    // DirectX 11 の終了処理
    void Shutdown();

    // フレーム開始（バックバッファのクリア）
    void BeginScene(float r, float g, float b, float a);

    // フレーム終了（スワップチェーンのプレゼント）
    void EndScene();

    // ゲッター
    ID3D11Device*        GetDevice()        const { return m_device; }
    ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }

private:
    ID3D11Device*            m_device;
    ID3D11DeviceContext*     m_deviceContext;
    IDXGISwapChain*          m_swapChain;
    ID3D11RenderTargetView*  m_renderTargetView;
    ID3D11DepthStencilView*  m_depthStencilView;
    ID3D11Texture2D*         m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11RasterizerState*   m_rasterizerState;
};
