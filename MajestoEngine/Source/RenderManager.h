#pragma once

#include "DirectXHeaders.h"

#include <string>
#include <wrl.h>
#include <exception>
#include <cassert>

#include "GameTimer.h"
#include "UploadBuffer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

struct Vertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
};

//struct ObjectConstants
//{
//    DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
//};

class RenderManager
{
public:
    RenderManager() = delete;
    RenderManager(const RenderManager& rhs) = delete;
    RenderManager& operator=(const RenderManager& rhs) = delete;

    RenderManager(HINSTANCE hInstance, int nCmdShow);
    ~RenderManager(); // TODO - make it virtual if there is a child

    bool Initialization();
    int Run();

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
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateRTV();
    void CreateDepthStencilBufferAndView();

    ID3D12Resource* CurrentBackBuffer() const; // TODO add exeption here
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    float AspectRatio() const;
    void CalculateFrameStats();

    // Next Step methods


    void BuildConstantBufferDescriptorHeaps();     // TODO: Move to CreateDescriptorHeaps method
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildTriangleGeometry();
    void BuildPSO();

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
    Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target);

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
    bool mAppPaused = false;  // is the application paused?
    bool mMinimized = false;  // is the application minimized?
    bool mMaximized = false;  // is the application maximized?
    bool mResizing = false;   // are the resize bars being dragged?
    bool mFullscreenState = false;// fullscreen enabled
    int mClientWidth = 800;
    int mClientHeight = 600;

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

    // Next Step properties
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    //std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

    Microsoft::WRL::ComPtr<ID3DBlob> mVSByteCode = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> mPSByteCode = nullptr;
    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> mVertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
};