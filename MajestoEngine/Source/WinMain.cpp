#pragma once

#include "pch.h"
#include "RenderManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    RenderManager RenderManager(hInstance, nCmdShow);
    if (RenderManager.Initialization())
    {
        return RenderManager.Run();
    }
    return 0;
}

