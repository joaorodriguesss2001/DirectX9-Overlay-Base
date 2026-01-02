#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "Hook.h"
#include "Draw.h"
#include "Menu.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <random>
#include "windows.h"
#include "JunkCode.h"


using namespace std;

BYTE DrawIndexedPrimitive_byte[5]; 
bool dwWallhack = false;            
tDrawIndexedPrimitive Original_DrawIndexedPrimitive = NULL;
tReset Original_Reset = NULL;
BYTE Reset_byte[5];

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int BLACK = 0;
int BLUE = 1;
int GREEN = 2;
int CYAN = 3;
int RED = 4;
int MAGENTA = 5;
int BROWN = 6;
int LIGHTGRAY = 7;
int DARKGRAY = 8;
int LIGHTBLUE = 9;
int LIGHTGREEN = 10;
int LIGHTCYAN = 11;
int LIGHTRED = 12;
int LIGHTMAGENTA = 13;
int YELLOW = 14;
int WHITE = 15;

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

void CreatDevice(DWORD* dVTable) {
	LPDIRECT3D9 mD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mD3D) {
		printf("[DEBUG] Erro: Direct3DCreate9 falhou.\n");
		return;
	}

	D3DPRESENT_PARAMETERS pPresentParams;
	ZeroMemory(&pPresentParams, sizeof(pPresentParams));
	pPresentParams.Windowed = TRUE;
	pPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pPresentParams.BackBufferFormat = D3DFMT_UNKNOWN;

	// Tenta achar a janela do jogo especificamente
	HWND TargetWindow = FindWindowA("PointBlank", NULL);
	if (!TargetWindow) {
		// Se não achar pelo nome da classe, tenta a janela ativa
		TargetWindow = GetForegroundWindow();
		printf("[AVISO] Janela 'PointBlank' nao achada, usando janela ativa.\n");
	}

	LPDIRECT3DDEVICE9 pDevice = NULL;
	if (FAILED(mD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pPresentParams, &pDevice))) {
		printf("[DEBUG] Erro: CreateDevice falhou! (Janela invalida?)\n");
		mD3D->Release();
		return;
	}

	DWORD* vTable = (DWORD*)pDevice;
	vTable = (DWORD*)vTable[0];

	for (int i = 0; i < 120; i++) {
		dVTable[i] = vTable[i];
	}

	pDevice->Release();
	mD3D->Release();

	printf("[DEBUG] CreatDevice finalizado com sucesso.\n");
}

void mainmenu() {
	

	// 1. Cria a tabela (O Balde) com tamanho suficiente
	DWORD vTable[120] = { 0 };

	// 2. Preenche a tabela usando nossa função segura
	CreatDevice(vTable);

	Original_EndScene = (_EndScene)vTable[42];

	// Wallhack (DIP) está no índice 82
	Original_DrawIndexedPrimitive = (_DrawIndexedPrimitive)vTable[82];

	// NOVO: Reset (Correção de Alt+Tab) está no índice 16
	Original_Reset = (tReset)vTable[16];

	
	if (Original_EndScene && Original_DrawIndexedPrimitive && Original_Reset) {

		// Aplica o Hook do Menu
		HookFunction((void*)Original_EndScene, (void*)My_EndScene, 5, EndScene_byte);

		// Aplica o Hook do Wallhack
		HookFunction((void*)Original_DrawIndexedPrimitive, (void*)My_DrawIndexedPrimitive, 5, DrawIndexedPrimitive_byte);

		// NOVO: Aplica o Hook do Reset (Anti-Crash)
		HookFunction((void*)Original_Reset, (void*)My_Reset, 5, Reset_byte);
	}
}


DWORD WINAPI mThread(PVOID lpParam) {
	
	while (!GetModuleHandle("d3d9.dll")) {
		Sleep(100);
	}

	// Inicia o nosso setup
	mainmenu();

	return 0;
}

void OpenConsole(std::string Title)
{
	/*AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	//Console Title Hashing
	string str = "0123456789ABCD-EFG+HIJKLMNO-PQR+STUVWXY-Zab+cdefghijklmnop-qrstuvwxyz";
	string nwstr;
	int ps;
	while (nwstr.size() != 25) {
		ps = ((rand() % (str.size() - 1)));
		nwstr += str.substr(ps, 1);
	}
	std::string stemp = std::string(nwstr.begin(), nwstr.end());
	LPCTSTR sww = const_cast<char*>(stemp.c_str());
	SetConsoleTitle(sww);*/
	//
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		///All Processes Run
		ExecutarLixo();
		Sleep(1000);
		CreateThread(0, 0, &mThread, 0, 0, 0);
	}
	return TRUE;
}

