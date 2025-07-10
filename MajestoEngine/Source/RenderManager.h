#pragma once

#include "../../include/directx/d3dx12.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <windows.h>
#include <Windowsx.h>
#include <string>
#include <wrl.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <DirectXColors.h>
#include <exception>
#include <cassert>

// #include <shellapi.h>

#include "GameTimer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


class RenderManager
{
public:
    RenderManager() = delete;
    RenderManager(HINSTANCE hInstance, int nCmdShow);
    RenderManager(const RenderManager& rhs) = delete;
    RenderManager& operator=(const RenderManager& rhs) = delete;
    ~RenderManager(); // TODO - make it virtual if there is a child

    bool Initialization();
    void Run();

    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);

    static RenderManager* GetRenderManager();
    LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    bool InitializationWindow();
    bool InitializationDX12();
    void FlushCommandQueue();
    void CreateDescriptorHeaps();
    void OnResize();
    void Update(const GameTimer& mTimer);
    void Draw(const GameTimer& mTimer);

    void OnMouseDown(WPARAM btnState, int x, int y) {}
    void OnMouseUp(WPARAM btnState, int x, int y) {}
    void OnMouseMove(WPARAM btnState, int x, int y) {}

private:
    void CreateDevice();
    void CreateFenceAndDescriptorSize();
    void Check4XMSAAQualitySupport();
    void CreateCommonQueueAndCommandList();
    void CreateSwapChain();
    void CreateRenderTargetView();
    void CreateDepthStencilBufferAndView();
    void SetViewport();
    void SetScissorRectangle();

    ID3D12Resource* CurrentBackBuffer() const; // TODO add exeption here
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    float AspectRatio() const;
    void CalculateFrameStats();

    // Logs
    //
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

    // UTils function 
    // TODO: Make it in future - template or static or static template
    //
    void ThrowIfFailed(HRESULT hr);
    void ReleaseCOM(IUnknown* ComObj); // Try to change it to Template.

private:

    // Propierties
    // 
    inline static RenderManager* mRenderManager = nullptr;

    // Win
    HINSTANCE WinInstance = nullptr; // application instance handle
    int WinCmdShow = 0;
    HWND mHwnd = nullptr;  // main window handle
    std::wstring mMainWndCaption = L"d3d App";
    D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    bool mFullscreenState = false;// fullscreen enabled
    int mClientWidth = 800;
    int mClientHeight = 600;

    bool mAppPaused = false;
    bool mResizing = false;

    // Timer
    GameTimer mTimer;

    // DX
    Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;

    UINT mRtvDescriptorSize = 0;
    UINT mDsvDescriptorSize = 0;
    UINT mCbvSrvUavDescriptorSize = 0;

    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    bool m4xMsaaState = false;  // 4X MSAA enabled
    UINT m4xMsaaQuality = 0; // quality level of 4X MSAA
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels; // TODO - check this

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    Microsoft::WRL::ComPtr < IDXGISwapChain> mSwapChain;
    static const int SwapChainBufferCount = 2;
    int mCurrBackBuffer = 0;

    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;
};