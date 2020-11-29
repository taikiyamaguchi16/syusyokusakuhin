//*****************************************************************************
//!	@file	main.cpp
//!	@brief	
//!	@note	GM31�ЂȌ`���C��
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <thread>
#include <crtdbg.h>
#include "game.h"
#include "object.h"
#include "Scene.h"

#include <iostream>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include "CDirectxGraphics.h"

#include "DirectX11Manager.h"
//-----------------------------------------------------------------------------
// �}�N���̒�`
//-----------------------------------------------------------------------------
#define		NAME			"win32A"
#define		TITLE			"GM31 DX11 �t���[�����[�N"
#define		FULLSCREEN		0				// �t���X�N���[���t���O

//-----------------------------------------------------------------------------
// �����N���C�u�����̐ݒ�
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")			// �����N�Ώۃ��C�u������d3d11.lib��ǉ�
#pragma comment(lib, "dxgi.lib")			// �����N�Ώۃ��C�u������dxgxi.lib��ǉ�
#pragma comment(lib, "D3DCompiler.lib")		// �����N�Ώۃ��C�u������d3dcompiler.lib��ǉ�
#pragma comment(lib, "winmm.lib")			// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment(lib, "directxtex.lib")		// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment(lib, "dxguid.lib")			// �����N�Ώۃ��C�u������dxguid.lib��ǉ�
#pragma comment(lib, "dinput8.lib")			// �����N�Ώۃ��C�u������dinput8.lib��ǉ�


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// �E�C���h�E�v���V�[�W��
//LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
int g_nCountFPS = 0;						// �e�o�r�ւ̃J�E���^


//==============================================================================
//!	@fn		WinMain
//!	@brief	�G���g���|�C���g
//!	@param	�C���X�^���X�n���h��
//!	@param	�Ӗ��Ȃ�
//!	@param	�N�����̈���������
//!	@param	�E�C���h�E�\�����[�h
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================
int APIENTRY WinMain(HINSTANCE 	hInstance, 		// �A�v���P�[�V�����̃n���h��
					 HINSTANCE 	hPrevInstance,	// ����Windows�o�[�W�����ł͎g���Ȃ�
					 LPSTR 		lpszArgs, 		// �N�����̈����i������j
					 int 		nWinMode)		// �E�C���h�E�\�����[�h
{
	//HWND			hwnd;						// �E�C���h�E�n���h��
	MSG				msg;						// ���b�Z�[�W�\����
	//WNDCLASSEX		wcex;						// �E�C���h�E�N���X�\����
	//int				width = SCREEN_X;			// �E�C���h�E�̕� �v�Z�p���[�N
	//int				height = SCREEN_Y;			// �E�C���h�E�̍��� �v�Z�p���[�N

	DWORD dwExecLastTime;		// �Ō�Ɏ��s��������		
	DWORD dwFPSLastTime;		// �Ō�Ɍv����������
	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwFrameCount;			// �t���[����

	//// ���������[�N�����m
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//// �E�C���h�E�N���X���̃Z�b�g
	//wcex.hInstance		= hInstance;			// �C���X�^���X�l�̃Z�b�g
	//wcex.lpszClassName	= NAME;					// �N���X��
	//wcex.lpfnWndProc	= (WNDPROC)WndProc;		// �E�C���h�E���b�Z�[�W�֐�
	//wcex.style			= 0;					// �E�C���h�E�X�^�C��
	//wcex.cbSize 		= sizeof(WNDCLASSEX);	// �\���̂̃T�C�Y
	//wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ���[�W�A�C�R��
	//wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// �X���[���A�C�R��
	//wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// �J�[�\���X�^�C��
	//wcex.lpszMenuName	= 0; 					// ���j���[�Ȃ�
	//wcex.cbClsExtra		= 0;					// �G�L�X�g���Ȃ�
	//wcex.cbWndExtra		= 0;					
	//wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);		// �w�i�F��

	//if (!RegisterClassEx(&wcex)) return FALSE;	// �E�C���h�E�N���X�̓o�^

	//if(FULLSCREEN){
	//	hwnd = CreateWindow(
	//		NAME,							// �E�B���h�E�N���X�̖��O
	//		TITLE,							// �^�C�g��
	//		WS_VISIBLE | WS_POPUP,			// �E�B���h�E�X�^�C��
	//		0, 0,							// �E�B���h�E�ʒu �c, ��
	//		SCREEN_X, SCREEN_Y,				// �E�B���h�E�T�C�Y
	//		NULL,							// �e�E�B���h�E�Ȃ�
	//		(HMENU)nullptr,					// ���j���[�Ȃ�
	//		hInstance,						// �C���X�^���X�n���h��
	//		(LPVOID)nullptr);					// �ǉ������Ȃ�
	//}else{
	//	//�^�C�g���o�[�ƃE�C���h�E�g�̕����܂߂ăE�C���h�E�T�C�Y��ݒ�
	//	RECT rect;
	//	SetRect(&rect, 0, 0, SCREEN_X, SCREEN_Y);
	//	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//	int window_width = rect.right - rect.left;
	//	int window_height = rect.bottom - rect.top;
	//	rect.top = 0;
	//	rect.left = 0;

	//	hwnd = CreateWindowEx(0,	// �g���E�B���h�E�X�^�C��
	//		NAME,					// �E�B���h�E�N���X�̖��O
	//		TITLE,					// �^�C�g��
	//		WS_OVERLAPPEDWINDOW,	// �E�B���h�E�X�^�C��
	//		CW_USEDEFAULT,			// �E�B���h�E�̍���w���W
	//		CW_USEDEFAULT,			// �E�B���h�E�̍���x���W 
	//		window_width,			// �E�B���h�E�̕�
	//		window_height,			// �E�B���h�E�̍���
	//		nullptr,				// �e�E�B���h�E�̃n���h��
	//		nullptr,				// ���j���[�n���h���܂��͎q�E�B���h�EID
	//		hInstance,				// �C���X�^���X�n���h��
	//		nullptr);				// �E�B���h�E�쐬�f�[�^
	//}

	//if (!hwnd) return FALSE;

	timeBeginPeriod(1);							// �^�C�}�̕���\�͂��Pms�ɂ���
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//// �E�C���h�E��\������
	//ShowWindow(hwnd, nWinMode);
	//UpdateWindow(hwnd);

	//dx11�}�l�[�W���[�̏�����
	if (FAILED(DirectX11Manager::Init(hInstance, nWinMode)))
		return -1;

	//===================================================�ǉ�=======================================================================
	
	//==============================================================================================================================

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂�āAWM_QUIT���b�Z�[�W�������烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
		 // �e�o�r��艻����

		 // ���ݎ��Ԃ��擾
			dwCurrentTime = timeGetTime();
			// 0.5�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) > 1000) {

				// FPS���v�Z
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// �e�o�r�v�����ԂɌ������Ԃ��Z�b�g
				dwFPSLastTime = dwCurrentTime;

				// �t���[�������[���N���A
				dwFrameCount = 0;
			}

			// �P�U�~���b�o�߂�����
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0 / (double)SceneManager::GetInstance()->m_fps)) {
				// �ŏI���s���ԂɌ��ݎ��Ԃ��Z�b�g
				dwExecLastTime = dwCurrentTime;
				SceneManager::GetInstance()->MainLoop();
				//�R�}������s���͑�����Ȃ�
				if (!SceneManager::GetInstance()->m_frameAdvanceFg) {
					SceneManager::GetInstance()->m_fpsCount = dwFrameCount;
				}

				// �t���[�������C���N�������g
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);			// �^�C�}�̕���\�͂��Ƃɖ߂�

	DirectX11Manager::Cleanup();
	return 0;
}

//==============================================================================
//!	@fn		WindowProc
//!	@brief	�E�C���h�E�v���V�[�W��
//!	@param	�E�C���h�E�n���h��
//!	@param	���b�Z�[�W
//!	@param	W�p�����[�^
//!	@param	L�p�����[�^
//!	@retval	�I����
//==============================================================================
//LRESULT WINAPI WndProc(	HWND hwnd, 		// �E�B���h�E�n���h��
//						UINT message,	// ���b�Z�[�W���ʎq
//						WPARAM wParam,	// �t�я��P
//						LPARAM lParam)	// �t�я��Q
//{
//
//	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
//		return true;
//
//	switch( message ){
//	case WM_KEYDOWN:
//		switch(wParam){
//		case VK_ESCAPE:
//			DestroyWindow(hwnd);
//			break;
//		}
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hwnd, message, wParam, lParam);
//	}
//	return 0;
//}

//******************************************************************************
//	End of file.
//******************************************************************************
