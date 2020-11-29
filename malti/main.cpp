//*****************************************************************************
//!	@file	main.cpp
//!	@brief	
//!	@note	GM31ひな形メイン
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
// マクロの定義
//-----------------------------------------------------------------------------
#define		NAME			"win32A"
#define		TITLE			"GM31 DX11 フレームワーク"
#define		FULLSCREEN		0				// フルスクリーンフラグ

//-----------------------------------------------------------------------------
// リンクライブラリの設定
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")			// リンク対象ライブラリにd3d11.libを追加
#pragma comment(lib, "dxgi.lib")			// リンク対象ライブラリにdxgxi.libを追加
#pragma comment(lib, "D3DCompiler.lib")		// リンク対象ライブラリにd3dcompiler.libを追加
#pragma comment(lib, "winmm.lib")			// リンク対象ライブラリにwinmm.libを追加
#pragma comment(lib, "directxtex.lib")		// リンク対象ライブラリにwinmm.libを追加
#pragma comment(lib, "dxguid.lib")			// リンク対象ライブラリにdxguid.libを追加
#pragma comment(lib, "dinput8.lib")			// リンク対象ライブラリにdinput8.libを追加


//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// ウインドウプロシージャ
//LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
int g_nCountFPS = 0;						// ＦＰＳへのカウンタ


//==============================================================================
//!	@fn		WinMain
//!	@brief	エントリポイント
//!	@param	インスタンスハンドル
//!	@param	意味なし
//!	@param	起動時の引数文字列
//!	@param	ウインドウ表示モード
//!	@retval	TRUE　成功終了/FALSE　失敗終了
//!	@note	
//==============================================================================
int APIENTRY WinMain(HINSTANCE 	hInstance, 		// アプリケーションのハンドル
					 HINSTANCE 	hPrevInstance,	// 今のWindowsバージョンでは使われない
					 LPSTR 		lpszArgs, 		// 起動時の引数（文字列）
					 int 		nWinMode)		// ウインドウ表示モード
{
	//HWND			hwnd;						// ウインドウハンドル
	MSG				msg;						// メッセージ構造体
	//WNDCLASSEX		wcex;						// ウインドウクラス構造体
	//int				width = SCREEN_X;			// ウインドウの幅 計算用ワーク
	//int				height = SCREEN_Y;			// ウインドウの高さ 計算用ワーク

	DWORD dwExecLastTime;		// 最後に実行した時間		
	DWORD dwFPSLastTime;		// 最後に計測した時間
	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwFrameCount;			// フレーム数

	//// メモリリークを検知
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//// ウインドウクラス情報のセット
	//wcex.hInstance		= hInstance;			// インスタンス値のセット
	//wcex.lpszClassName	= NAME;					// クラス名
	//wcex.lpfnWndProc	= (WNDPROC)WndProc;		// ウインドウメッセージ関数
	//wcex.style			= 0;					// ウインドウスタイル
	//wcex.cbSize 		= sizeof(WNDCLASSEX);	// 構造体のサイズ
	//wcex.hIcon			= LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);	// ラージアイコン
	//wcex.hIconSm		= LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);		// スモールアイコン
	//wcex.hCursor		= LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// カーソルスタイル
	//wcex.lpszMenuName	= 0; 					// メニューなし
	//wcex.cbClsExtra		= 0;					// エキストラなし
	//wcex.cbWndExtra		= 0;					
	//wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);		// 背景色白

	//if (!RegisterClassEx(&wcex)) return FALSE;	// ウインドウクラスの登録

	//if(FULLSCREEN){
	//	hwnd = CreateWindow(
	//		NAME,							// ウィンドウクラスの名前
	//		TITLE,							// タイトル
	//		WS_VISIBLE | WS_POPUP,			// ウィンドウスタイル
	//		0, 0,							// ウィンドウ位置 縦, 横
	//		SCREEN_X, SCREEN_Y,				// ウィンドウサイズ
	//		NULL,							// 親ウィンドウなし
	//		(HMENU)nullptr,					// メニューなし
	//		hInstance,						// インスタンスハンドル
	//		(LPVOID)nullptr);					// 追加引数なし
	//}else{
	//	//タイトルバーとウインドウ枠の分を含めてウインドウサイズを設定
	//	RECT rect;
	//	SetRect(&rect, 0, 0, SCREEN_X, SCREEN_Y);
	//	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//	int window_width = rect.right - rect.left;
	//	int window_height = rect.bottom - rect.top;
	//	rect.top = 0;
	//	rect.left = 0;

	//	hwnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
	//		NAME,					// ウィンドウクラスの名前
	//		TITLE,					// タイトル
	//		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
	//		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
	//		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
	//		window_width,			// ウィンドウの幅
	//		window_height,			// ウィンドウの高さ
	//		nullptr,				// 親ウィンドウのハンドル
	//		nullptr,				// メニューハンドルまたは子ウィンドウID
	//		hInstance,				// インスタンスハンドル
	//		nullptr);				// ウィンドウ作成データ
	//}

	//if (!hwnd) return FALSE;

	timeBeginPeriod(1);							// タイマの分解能力を１msにする
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	//// ウインドウを表示する
	//ShowWindow(hwnd, nWinMode);
	//UpdateWindow(hwnd);

	//dx11マネージャーの初期化
	if (FAILED(DirectX11Manager::Init(hInstance, nWinMode)))
		return -1;

	//===================================================追加=======================================================================
	
	//==============================================================================================================================

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合"0"を返す
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれて、WM_QUITメッセージが来たらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
		 // ＦＰＳ一定化処理

		 // 現在時間を取得
			dwCurrentTime = timeGetTime();
			// 0.5秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) > 1000) {

				// FPSを計算
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// ＦＰＳ計測時間に現座時間をセット
				dwFPSLastTime = dwCurrentTime;

				// フレーム数をゼロクリア
				dwFrameCount = 0;
			}

			// １６ミリ秒経過したら
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0 / (double)SceneManager::GetInstance()->m_fps)) {
				// 最終実行時間に現在時間をセット
				dwExecLastTime = dwCurrentTime;
				SceneManager::GetInstance()->MainLoop();
				//コマ送り実行中は代入しない
				if (!SceneManager::GetInstance()->m_frameAdvanceFg) {
					SceneManager::GetInstance()->m_fpsCount = dwFrameCount;
				}

				// フレーム数をインクリメント
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);			// タイマの分解能力もとに戻す

	DirectX11Manager::Cleanup();
	return 0;
}

//==============================================================================
//!	@fn		WindowProc
//!	@brief	ウインドウプロシージャ
//!	@param	ウインドウハンドル
//!	@param	メッセージ
//!	@param	Wパラメータ
//!	@param	Lパラメータ
//!	@retval	終了状況
//==============================================================================
//LRESULT WINAPI WndProc(	HWND hwnd, 		// ウィンドウハンドル
//						UINT message,	// メッセージ識別子
//						WPARAM wParam,	// 付帯情報１
//						LPARAM lParam)	// 付帯情報２
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
