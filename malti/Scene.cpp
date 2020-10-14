#include "Scene.h"

#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "DX11util.h"
#include "CModel.h"
#include "game.h"
#include "Title.h"
#include "CDirectInput.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "shader.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "Camera.h"

bool SceneManager::Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen)
{
	CPhysx::Init();
	bool		sts;

	// DX11初期処理
	sts = DX11Init(hwnd, width, height, fullscreen);
	if (!sts) {
		MessageBox(hwnd, "DX11 init error", "error", MB_OK);
		return false;
	}

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(hinst, hwnd, width, height);

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(GetDX11Device(),
		"shader/vs.fx",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_vs,
		&m_layout);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(	// ピクセルシェーダーオブジェクトを生成
		GetDX11Device(),		// デバイス
		"shader/ps.fx",
		"main",
		"ps_5_0",
		&m_ps);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}


	
	// 定数バッファ生成
	sts = CreateConstantBufferWrite(GetDX11Device(), sizeof(ConstantBufferMaterial), &m_cbuffer);
	if (!sts) {
		MessageBox(nullptr, "CreateConstantBufferWrite error", "error", MB_OK);
		return false;
	}

	sts = CreateSRVfromFile("assets/space.jpg", GetDX11Device(), GetDX11DeviceContext(), &m_texres, &m_srv);
	if (!sts) {
		MessageBox(nullptr, "CreateSRVfromFile error", "error", MB_OK);
		return false;
	}

	// マテリアル
	ConstantBufferMaterial material = {
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),			// 環境光マテリアル
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),			// 拡散反射マテリアル
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),			// 鏡面反射マテリアル
	};

	// 定数バッファ書き換え
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetDX11DeviceContext()->Map(m_cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&material), sizeof(ConstantBufferMaterial));
		GetDX11DeviceContext()->Unmap(m_cbuffer, 0);
	}


	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// 平行光源の方向をセット

	// アルファブレンド有効にする
	TurnOnAlphablend();


	m_activeScene = new CGame();
	return	true;
}

void SceneManager::UnInit()
{
	if (m_vertexbuffer != nullptr) {
		m_vertexbuffer->Release();
		m_vertexbuffer = nullptr;
	}

	if (m_idxbuffer != nullptr) {
		m_idxbuffer->Release();
		m_idxbuffer = nullptr;
	}

	if (m_cbuffer != nullptr) {
		m_cbuffer->Release();
		m_cbuffer = nullptr;
	}

	if (m_vs != nullptr) {
		m_vs->Release();
		m_vs = nullptr;
	}

	if (m_ps != nullptr) {
		m_ps->Release();
		m_ps = nullptr;
	}

	if (m_layout != nullptr) {
		m_layout->Release();
		m_layout = nullptr;
	}

	if (m_srv != nullptr) {
		m_srv->Release();
		m_srv = nullptr;
	}

	if (m_texres != nullptr) {
		m_texres->Release();
		m_texres = nullptr;
	}

	DX11Uninit();
}
