#include "direct3D.h"

// ---------------------------------------------------------------------------
//  コンストラクタ / デストラクタ
// ---------------------------------------------------------------------------
Direct3D::Direct3D()
    : m_device(nullptr)
    , m_deviceContext(nullptr)
    , m_swapChain(nullptr)
    , m_renderTargetView(nullptr)
    , m_depthStencilView(nullptr)
    , m_depthStencilBuffer(nullptr)
    , m_depthStencilState(nullptr)
    , m_rasterizerState(nullptr)
{
}

Direct3D::~Direct3D()
{
}

// ---------------------------------------------------------------------------
//  Initialize – DirectX 11 の初期化
// ---------------------------------------------------------------------------
bool Direct3D::Initialize(HWND hwnd, int width, int height)
{
    HRESULT hr;

    // -----------------------------------------------------------------------
    //  スワップチェーンの設定
    // -----------------------------------------------------------------------
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount                        = 1;
    scDesc.BufferDesc.Width                   = static_cast<UINT>(width);
    scDesc.BufferDesc.Height                  = static_cast<UINT>(height);
    scDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.RefreshRate.Numerator   = 0;   // 可変フレームレートのため 0/1 を指定
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.SampleDesc.Count                   = 1;
    scDesc.SampleDesc.Quality                 = 0;
    scDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow                       = hwnd;
    scDesc.Windowed                           = TRUE;
    scDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags                              = 0;

    // -----------------------------------------------------------------------
    //  デバイス・スワップチェーンの生成
    // -----------------------------------------------------------------------
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &scDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_deviceContext
    );

    if (FAILED(hr))
    {
        return false;
    }

    // -----------------------------------------------------------------------
    //  レンダーターゲットビューの生成
    // -----------------------------------------------------------------------
    ID3D11Texture2D* backBuffer = nullptr;
    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (FAILED(hr))
    {
        return false;
    }

    hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
    backBuffer->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // -----------------------------------------------------------------------
    //  深度ステンシルバッファの生成
    // -----------------------------------------------------------------------
    D3D11_TEXTURE2D_DESC dsDesc = {};
    dsDesc.Width              = static_cast<UINT>(width);
    dsDesc.Height             = static_cast<UINT>(height);
    dsDesc.MipLevels          = 1;
    dsDesc.ArraySize          = 1;
    dsDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count   = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Usage              = D3D11_USAGE_DEFAULT;
    dsDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

    hr = m_device->CreateTexture2D(&dsDesc, nullptr, &m_depthStencilBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // -----------------------------------------------------------------------
    //  深度ステンシルステートの生成
    // -----------------------------------------------------------------------
    D3D11_DEPTH_STENCIL_DESC dssDesc = {};
    dssDesc.DepthEnable    = TRUE;
    dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dssDesc.DepthFunc      = D3D11_COMPARISON_LESS;
    dssDesc.StencilEnable  = FALSE;

    hr = m_device->CreateDepthStencilState(&dssDesc, &m_depthStencilState);
    if (FAILED(hr))
    {
        return false;
    }

    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

    // -----------------------------------------------------------------------
    //  深度ステンシルビューの生成
    // -----------------------------------------------------------------------
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format        = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &dsvDesc, &m_depthStencilView);
    if (FAILED(hr))
    {
        return false;
    }

    // レンダーターゲットと深度ステンシルビューをパイプラインにバインド
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // -----------------------------------------------------------------------
    //  ラスタライザーステートの設定
    // -----------------------------------------------------------------------
    D3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.FillMode              = D3D11_FILL_SOLID;
    rsDesc.CullMode              = D3D11_CULL_BACK;
    rsDesc.FrontCounterClockwise = FALSE;
    rsDesc.DepthClipEnable       = TRUE;

    hr = m_device->CreateRasterizerState(&rsDesc, &m_rasterizerState);
    if (FAILED(hr))
    {
        return false;
    }

    m_deviceContext->RSSetState(m_rasterizerState);

    // -----------------------------------------------------------------------
    //  ビューポートの設定
    // -----------------------------------------------------------------------
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = static_cast<float>(width);
    viewport.Height   = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);

    return true;
}

// ---------------------------------------------------------------------------
//  Shutdown – DirectX 11 リソースの解放
// ---------------------------------------------------------------------------
void Direct3D::Shutdown()
{
    // スワップチェーンをウィンドードモードに戻してから解放
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(FALSE, nullptr);
    }

    if (m_rasterizerState)   { m_rasterizerState->Release();   m_rasterizerState   = nullptr; }
    if (m_depthStencilView)  { m_depthStencilView->Release();  m_depthStencilView  = nullptr; }
    if (m_depthStencilState) { m_depthStencilState->Release(); m_depthStencilState = nullptr; }
    if (m_depthStencilBuffer) { m_depthStencilBuffer->Release(); m_depthStencilBuffer = nullptr; }
    if (m_renderTargetView)  { m_renderTargetView->Release();  m_renderTargetView  = nullptr; }
    if (m_swapChain)         { m_swapChain->Release();         m_swapChain         = nullptr; }
    if (m_deviceContext)     { m_deviceContext->Release();     m_deviceContext     = nullptr; }
    if (m_device)            { m_device->Release();            m_device            = nullptr; }
}

// ---------------------------------------------------------------------------
//  BeginScene – バックバッファと深度バッファのクリア
// ---------------------------------------------------------------------------
void Direct3D::BeginScene(float r, float g, float b, float a)
{
    float color[4] = { r, g, b, a };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    m_deviceContext->ClearDepthStencilView(m_depthStencilView,
                                           D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                           1.0f, 0);
}

// ---------------------------------------------------------------------------
//  EndScene – スワップチェーンのプレゼント（VSync なし = 可変フレームレート）
// ---------------------------------------------------------------------------
void Direct3D::EndScene()
{
    // SyncInterval = 0: VSync 無効 → 可変フレームレート
    m_swapChain->Present(0, 0);
}
