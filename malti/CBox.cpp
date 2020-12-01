#include "CBox.h"
#include "Scene.h"
#include "UnityExportModel.h"

void CBox::Init(XMFLOAT3 s_)
{
	m_size = s_;
	//Init(GetDX11Device());

	//vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/UnityExportModel.hlsl", "vsMain"));
	//ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/UnityExportModel.hlsl", "psMain"));

	////InputLayoutの作成
	//D3D11_INPUT_ELEMENT_DESC elem[] = {
	//	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//};
	//il.Attach(DirectX11Manager::CreateInputLayout(elem, 4, "assets/Shaders/UnityExportModel.hlsl", "vsMain"));

	Init(DirectX11Manager::m_pDevice.Get());

	//コンスタントバッファの作成
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::m_constantBuffer.proj = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
			SCREEN_X / SCREEN_X, 0.5f, 4096.0f * 8.0f));
	//m_cube.LoadBinary("assets/Models/Cube.bin");
}

// 法線ベクトルを計算
// ノーマライズ
void CBox::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector);			// XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v);			// 正規化
	XMStoreFloat3(&Normal, v);			// XMVECTOR=>XMFLOAT3
}

// インデックスデータを作成
void CBox::CreateIndex() {

	Face fa[12] = {
		3,1,0,
		2,1,3,
		0,5,4,
		1,5,0,
		3,4,7,
		0,4,3,
		1,6,5,
		2,6,1,
		2,7,6,
		3,7,2,
		6,4,5,
		7,4,6,
	};

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 3; j++) {
			m_face[i].idx[j] = fa[i].idx[j];
		}
	}

}

// 頂点データを作成
void CBox::CreateVertex() {
	float w2 = 0.5f*m_size.x;
	float h2 = 0.5f*m_size.y;
	float d2 = 0.5f*m_size.z;

	Vertex vertices[8] =
	{
		{ XMFLOAT3(-w2,  h2, -d2),  XMFLOAT3(-1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(w2,  h2, -d2),   XMFLOAT3(1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(w2,  h2,  d2),    XMFLOAT3(1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-w2,  h2,  d2),   XMFLOAT3(-1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-w2, -h2, -d2), XMFLOAT3(-1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(w2, -h2, -d2),  XMFLOAT3(1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(w2, -h2,  d2),   XMFLOAT3(1.0f, -1.0f, 1.0f) },
		{ XMFLOAT3(-w2, -h2,  d2),  XMFLOAT3(-1.0f, -1.0f, 1.0f) }
	};
	m_vertex[0] = { XMFLOAT3(-w2,  h2, -d2), XMFLOAT3(-1.0f, 1.0f, -1.0f) };
	m_vertex[1] = { XMFLOAT3(w2,  h2, -d2),   XMFLOAT3(1.0f, 1.0f, -1.0f) };
	m_vertex[2] = { XMFLOAT3(w2,  h2,  d2),    XMFLOAT3(1.0f, 1.0f, 1.0f) };
	m_vertex[3] = { XMFLOAT3(-w2,  h2,  d2),   XMFLOAT3(-1.0f, 1.0f, 1.0f) };
	m_vertex[4] = { XMFLOAT3(-w2, -h2, -d2), XMFLOAT3(-1.0f, -1.0f, -1.0f) };
	m_vertex[5] = { XMFLOAT3(w2, -h2, -d2),  XMFLOAT3(1.0f, -1.0f, -1.0f) };
	m_vertex[6] = { XMFLOAT3(w2, -h2,  d2),   XMFLOAT3(1.0f, -1.0f, 1.0f) };
	m_vertex[7] = { XMFLOAT3(-w2, -h2,  d2),  XMFLOAT3(-1.0f, -1.0f, 1.0f) };
	
}

// 描画
void CBox::Draw() {

	//// 定数バッファ書き換え
	//D3D11_MAPPED_SUBRESOURCE pData;
	//HRESULT hr = DirectX11Manager::m_pImContext->Map(m_cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	//if (SUCCEEDED(hr)) {
	//	memcpy_s(pData.pData, pData.RowPitch, (void*)(&m_material), sizeof(ConstantBufferMaterial));
	//	DirectX11Manager::m_pImContext->Unmap(m_cbuffer, 0);
	//}

	//UnityExportModel::constantBuffer.world = XMMatrixTranspose(XMLoadFloat4x4(&_mat));


	//DirectX11Manager::UpdateConstantBuffer(cb.Get(), UnityExportModel::constantBuffer);
	//ID3D11Buffer* tmpCb[] = { cb.Get() };
	//DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, tmpCb);
	m_cube.Draw();

	//ID3D11DeviceContext* device = DirectX11Manager::m_pImContext.Get();
	//// 頂点バッファをセットする
	//unsigned int stride = sizeof(Vertex);
	//unsigned  offset = 0;
	//device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
	//device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// トポロジーをセット（旧プリミティブタイプ）
	//device->IASetInputLayout(m_pVertexLayout);					// 頂点レイアウトセット

	//device->VSSetShader(m_pVertexShader, nullptr, 0);			// 頂点シェーダーをセット
	//device->PSSetShader(m_pPixelShader, nullptr, 0);			// ピクセルシェーダーをセット


	////device->PSSetConstantBuffers(3, 1, &m_cbuffer);
	//device->PSSetConstantBuffers(3, 1, tmpCb);
	//device->DrawIndexed(36,		// 描画するインデックス数（面数×３）
	//	0,									// 最初のインデックスバッファの位置
	//	0);									// 頂点バッファの最初から使う
}


bool CBox::Init(ID3D11Device* device) {
	//bool sts;
	//
	//// 球のインデックスデータを作成
	//CreateIndex();

	//// 球の頂点データを作成
	//CreateVertex();

	//// 頂点バッファ作成
	//sts = CreateVertexBufferWrite(
	//	device,
	//	sizeof(Vertex),						// １頂点当たりバイト数
	//	8,			// 頂点数
	//	m_vertex,							// 頂点データ格納メモリ先頭アドレス
	//	&m_pVertexBuffer);					// 頂点バッファ
	//if (!sts) {
	//	MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
	//	return false;
	//}

	//// インデックスバッファ作成
	//sts = CreateIndexBuffer(
	//	device,
	//	36,	// インデックス数
	//	m_face,					// インデックスデータ先頭アドレス
	//	&m_pIndexBuffer);		// インデックスバッファ
	//if (!sts) {
	//	MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
	//	return false;
	//}


	//// 定数バッファ生成
	//sts = CreateConstantBufferWrite(DirectX11Manager::m_pDevice.Get(), sizeof(ConstantBufferMaterial), &m_cbuffer);
	//if (!sts) {
	//	MessageBox(nullptr, "CreateConstantBufferWrite error", "error", MB_OK);
	//	return false;
	//}


	//// 頂点データの定義
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//unsigned int numElements = ARRAYSIZE(layout);

	//// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	//sts = CreateVertexShader(device,
	//	"shader/3dspherevs.fx",
	//	"main",
	//	"vs_5_0",
	//	layout,
	//	numElements,
	//	&m_pVertexShader,
	//	&m_pVertexLayout);
	//if (!sts) {
	//	MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
	//	return false;
	//}

	//// ピクセルシェーダーを生成
	//sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
	//	device,							// デバイスオブジェクト
	//	"shader/3dsphereps.fx",
	//	"main",
	//	"ps_5_0",
	//	&m_pPixelShader);
	//if (!sts) {
	//	MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
	//	return false;
	//}



	return true;
}



void CBox::Exit() {

	/*if (m_vertex) {
		delete[] m_vertex;
	}*/


	// ピクセルシェーダー解放
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// 頂点シェーダー解放
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}

	if (m_pIndexBuffer) {
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer) {
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
}