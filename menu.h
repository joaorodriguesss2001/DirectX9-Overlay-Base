#pragma once
#include <d3d9.h>
#include <d3dx9.h>

// --- INCLUDES DO IMGUI ---
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

// ---------------------------------------------------------
// 1. PREPARAÇÃO DO "PORTEIRO" (INPUT)
// ---------------------------------------------------------

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc = NULL;

extern bool dwWallhack;
bool ShowMenu = true;

// O NOSSO NOVO PORTEIRO (WndProc)
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------
// DEFINIÇÕES E HOOKS
// ---------------------------------------------------------
typedef HRESULT(WINAPI* tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);
typedef HRESULT(WINAPI* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* tReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters); // <--- DEFINIÇÃO DO RESET

extern tDrawIndexedPrimitive Original_DrawIndexedPrimitive;
extern tEndScene Original_EndScene;
extern tReset Original_Reset; // <--- EXTERN DO RESET

extern BYTE DrawIndexedPrimitive_byte[5];
extern BYTE EndScene_byte[5];
extern BYTE Reset_byte[5];    // <--- BYTES DO RESET

// ---------------------------------------------------------
// FUNÇÃO 1: MY_RESET (Anti-Crash Alt+Tab)
// ---------------------------------------------------------
HRESULT WINAPI My_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();

    UnHookFunction((void*)Original_Reset, Reset_byte, 5);
    HRESULT ret = Original_Reset(pDevice, pPresentationParameters);
    HookFunction((void*)Original_Reset, (void*)My_Reset, 5, Reset_byte);

    if (ret == D3D_OK) {
        ImGui_ImplDX9_CreateDeviceObjects();
    }
    return ret;
}

// ---------------------------------------------------------
// FUNÇÃO 2: O MENU (My_EndScene)
// ---------------------------------------------------------
HRESULT WINAPI My_EndScene(LPDIRECT3DDEVICE9 pDevice) {
    if (!pDevice) return Original_EndScene(pDevice);

    static bool init = false;
    if (!init) {
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);
        HWND GameWindow = params.hFocusWindow;

        if (GameWindow != NULL) {
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            ImGui_ImplWin32_Init(GameWindow);
            ImGui_ImplDX9_Init(pDevice);
            oWndProc = (WNDPROC)SetWindowLongPtr(GameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
            init = true;
        }
    }

    if (init) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (GetAsyncKeyState(VK_HOME) & 1) {
            ShowMenu = !ShowMenu;
        }

        if (ShowMenu) {
            ImGui::GetIO().MouseDrawCursor = true;
            ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

            ImGui::Begin("Detect PB - V4", &ShowMenu); // <--- Mudei o nome aqui já!
            ImGui::Text("Status: Injetado");
            ImGui::Separator();
            ImGui::Checkbox("Ativar Wallhack", &dwWallhack);
            ImGui::TextDisabled("Aperte HOME para esconder");
            ImGui::End();
        }
        else {
            ImGui::GetIO().MouseDrawCursor = false;
        }

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    UnHookFunction((void*)Original_EndScene, EndScene_byte, 5);
    HRESULT ret = Original_EndScene(pDevice);
    HookFunction((void*)Original_EndScene, (void*)My_EndScene, 5, EndScene_byte);

    return ret;
}

// ---------------------------------------------------------
// FUNÇÃO 3: WALLHACK
// ---------------------------------------------------------
HRESULT WINAPI My_DrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) {
    LPDIRECT3DVERTEXBUFFER9 Stream_Data;
    UINT Offset = 0;
    UINT Stride = 0;

    if (pDevice->GetStreamSource(0, &Stream_Data, &Offset, &Stride) == D3D_OK) {
        Stream_Data->Release();
    }

    if (dwWallhack && Stride == 44) {
        pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    UnHookFunction((void*)Original_DrawIndexedPrimitive, DrawIndexedPrimitive_byte, 5);
    HRESULT ret = Original_DrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
    HookFunction((void*)Original_DrawIndexedPrimitive, (void*)My_DrawIndexedPrimitive, 5, DrawIndexedPrimitive_byte);

    return ret;
}