#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "UniExportModel.hpp"
//WindowsDirectX
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl/client.h>
#include "MyInput8.h"
#include "ImGuiControl.h"


#include <string>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;

//書きやすいようにtypedef色々
typedef ComPtr<ID3D11Buffer> ConstantBuffer, VertexBuffer, IndexBuffer, StructuredBuffer;
typedef ComPtr<ID3D11InputLayout> InputLayout;
typedef ComPtr<ID3D11VertexShader> VertexShader;
typedef ComPtr<ID3D11GeometryShader> GeometryShader;
typedef ComPtr<ID3D11PixelShader> PixelShader;
typedef ComPtr<ID3D11ComputeShader> ComputeShader;
typedef ComPtr<ID3D11Texture2D> Texture2D;
typedef ComPtr<ID3D11ShaderResourceView> ShaderTexture;
typedef ComPtr<ID3D11UnorderedAccessView> ComputeOutputView;

struct ConstantBufferMatrix
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};
//============================================================================
//スタティックなクラスにする
//============================================================================
class DirectX11Manager
{
	static inline HWND hWnd = NULL;
public:
	//コンスタントバッファー
	static inline ConstantBufferMatrix m_constantBuffer;
	
	//DX11
	static inline ComPtr<ID3D11Device>			m_pDevice = nullptr;
	static inline ComPtr<ID3D11DeviceContext>		m_pImContext = nullptr;
	static inline ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;
	static inline Texture2D						m_pRTTex = nullptr;
	static inline ComPtr<ID3D11RenderTargetView>	m_pRTView = nullptr;
	static inline D3D11_VIEWPORT					m_Viewport = { 0,0,0,0,0,0 };
	static inline ComPtr<ID3D11RasterizerState>	m_pRasterizerState = nullptr;
	static inline ComPtr<ID3D11RasterizerState>		m_RasterStateWire=nullptr;		// ラスターステータス(ワイヤー)

	//Zバッファ
	static inline ComPtr<ID3D11Texture2D>			m_pTexture2DDepth = nullptr;
	static inline ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;	// Ｚバッファ、ステンシルステート
	static inline ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;

	//サンプラー
	static inline ComPtr<ID3D11SamplerState>		m_pSampler = nullptr; //通常テクスチャ(0-1サンプリング)

	//Input
	static inline CInput input;

	static HRESULT Init(HINSTANCE hInstance, int cCmdShow);
	static void Cleanup();

	//Shader
	static ID3D11VertexShader* CreateVertexShader(const string& filename, const string& entrypath = "", bool erorr = true);
	static ID3D11PixelShader* CreatePixelShader(const string& filename, const string& entrypath = "", bool erorr = true);
	static ID3D11GeometryShader* CreateGeometryShader(const string& filename, const string& entrypath = "", bool erorr = true);
	static ID3D11ComputeShader* CreateComputeShader(const string& filename, const string& entrypath = "");

	//inputlayout作成
	static ID3D11InputLayout* CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT elem_num, const string& filename, const string& entrypath = "");

	//コンスタントバッファを作成
	static bool CreateConstantBuffer(unsigned int bytesize, ID3D11Buffer** CBuffer);
	template<class x>
	static void UpdateConstantBuffer(ID3D11Buffer* buffer, const x& cb)
	{
		m_pImContext->UpdateSubresource(buffer, 0, NULL, &cb, 0, 0);
	}

	//bufferCreate
	template<class x>
	static ID3D11Buffer* CreateVertexBuffer(x* VertexData, UINT VertexNum)
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));
		hBufferDesc.ByteWidth = sizeof(x) * VertexNum;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		//hBufferDesc.MiscFlags = 0;
		//hBufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA hSubResourceData;
		ZeroMemory(&hSubResourceData, sizeof(hSubResourceData));
		hSubResourceData.pSysMem = VertexData;
		//hSubResourceData.SysMemPitch = 0;
		//hSubResourceData.SysMemSlicePitch = 0;

		ID3D11Buffer* hpBuffer;
		if (FAILED(m_pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))) {
			return nullptr;
		}
		return hpBuffer;
	}
	static ID3D11Buffer* CreateIndexBuffer(UINT* Index, UINT IndexNum)
	{
		//インデックスバッファ作成
		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));
		hBufferDesc.ByteWidth = sizeof(UINT) * IndexNum;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		//hBufferDesc.MiscFlags = 0;
		//hBufferDesc.StructureByteStride = sizeof(UINT);

		D3D11_SUBRESOURCE_DATA hSubResourceData;
		ZeroMemory(&hSubResourceData, sizeof(hSubResourceData));
		hSubResourceData.pSysMem = Index;
		//hSubResourceData.SysMemPitch = 0;
		//hSubResourceData.SysMemSlicePitch = 0;

		ID3D11Buffer* hpBuffer;
		if (FAILED(m_pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))) {
			return nullptr;
		}
		return hpBuffer;
	}
	static ID3D11Texture2D* CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format, UINT BindFlag, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT)
	{
		ID3D11Texture2D* texture = nullptr;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = 1;
		desc.Format = Format;
		desc.Usage = Usage;
		desc.BindFlags = BindFlag;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		HRESULT hr = m_pDevice->CreateTexture2D(&desc, nullptr, &texture);
		if (FAILED(hr))
			return nullptr;
		return texture;
	}

	static ID3D11ShaderResourceView* CreateTextureFromFile(const wchar_t* filename);
	static ID3D11ShaderResourceView* CreateTextureFromFile(const std::string filename);
	static ID3D11ShaderResourceView* CreateTextureFromFile(const char* filename);

	//PipelineSetting
	static void SetInputLayout(ID3D11InputLayout* VertexLayout);
	static void SetVertexShader(ID3D11VertexShader* vs);
	static void SetPixelShader(ID3D11PixelShader* ps);

	static void SetVertexBuffer(ID3D11Buffer* VertexBuffer, UINT VertexSize);
	static void SetIndexBuffer(ID3D11Buffer* IndexBuffer);

	static void SetTexture2D(UINT RegisterNo, ID3D11ShaderResourceView* Texture);

	static void DrawBegin();
	static void DrawEnd();
	static void Draw(UINT VertexNum);
	static void DrawIndexed(UINT VertexNum);

	static void TurnSolid();
	static void TurnWire();
};

