#pragma once

#include "pch.h"
#include "RenderManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    RenderManager RenderManager(hInstance, nCmdShow);
    if (RenderManager.Initialization())
    {
        RenderManager.Run();
    }
    return 0;
}

