#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"Shader.h"
#include	"memory.h"
#include	"CLight.h"
#include	"DX11Settransform.h"
#include     "Camera.h"

#include "ImGuiControl.h"

CDirectXGraphics	g_DXGrobj;			// ＤＩＲＥＣＴＸ　ＧＲＡＰＨＩＣＳ　オブジェクト		
										// 定数バッファ定義
CLight				g_directionallight;

void DX11LightInit(DirectX::XMFLOAT4 lightpos) {
	//一旦XMFLOAT3(0, 0, 0)を入れている
	g_directionallight.Init(XMFLOAT3(0, 0, 0), lightpos);
	g_directionallight.SetAmbient(XMFLOAT4(1.0f,0.0f,0.0f,0.0f));				// 環境光
	g_directionallight.Update();
}

void DX11LightUninit() {
	g_directionallight.Uninit();
}

void DX11LightUpdate(DirectX::XMFLOAT4 lpos) {
	//一旦XMFLOAT3(0, 0, 0)を入れている
	g_directionallight.SetEyePos(XMFLOAT3(0, 0, 0));
	g_directionallight.SetLightPos(lpos);
	g_directionallight.Update();
}

CDirectXGraphics* GetDX11Obj() {
	return &g_DXGrobj;
}

ID3D11Device* GetDX11Device() {
	return g_DXGrobj.GetDXDevice();
}

ID3D11DeviceContext*	GetDX11DeviceContext() {
	return g_DXGrobj.GetImmediateContext();
}

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen) {
	bool sts;

	sts = g_DXGrobj.Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}


	sts = DX11SetTransform::GetInstance()->Init();
	if (!sts) {
		MessageBox(NULL, "SetTransform error", "Error", MB_OK);
		return false;
	}

	return true;
}

void DX11Uninit() {

	DX11SetTransform::GetInstance()->Uninit();

	// 解放処理
	g_DXGrobj.Exit();

}

void DX11BeforeRender(float ClearColor[]) {
	// ターゲットバッファクリア
	g_DXGrobj.GetImmediateContext()->ClearRenderTargetView(g_DXGrobj.GetRenderTargetView(), ClearColor);
	// Zバッファクリア
	
	g_DXGrobj.GetImmediateContext()->ClearDepthStencilView(g_DXGrobj.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ImGuiControl::GetInstance()->BeforeRender();
}

void DX11AfterRender() {
	ImGuiControl::GetInstance()->AfterRebder();
	// ｓｗａｐ
	g_DXGrobj.GetSwapChain()->Present(0, 0);
}

int DX11GetScreenWidth() {
	return g_DXGrobj.GetWidth();
}

int DX11GetScreenHeight() {
	return g_DXGrobj.GetHeight();
}

void TurnOffZbuffer() {
	g_DXGrobj.TurnOffZbuffer();
}

void TurnOnZbuffer() {
	g_DXGrobj.TurnOnZBuffer();
}

void TurnOnAlphablend() {
	g_DXGrobj.TurnOnAlphaBlending();
}

void TurnOffAlphablend() {
	g_DXGrobj.TurnOffAlphaBlending();
}

void TurnWire() {
	g_DXGrobj.TurnWire();
}

void TurnSolid() {
	g_DXGrobj.TurnSolid();
}
