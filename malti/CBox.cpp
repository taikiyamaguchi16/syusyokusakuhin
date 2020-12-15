#include "CBox.h"
#include "Scene.h"
#include "UnityExportModel.h"

void CBox::Init()
{
	m_size = XMFLOAT3(1, 1, 1);

	vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/DefaultShape.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/DefaultShape.hlsl", "psMain"));

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 3, "assets/Shaders/DefaultShape.hlsl", "vsMain"));

	CreateVertex();
	CreateIndex();
	//コンスタントバッファの作成
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::CreateConstantBuffer(sizeof(Material), &cb2);

	mate.df = XMFLOAT4(1, 1, 1, 1);
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
	ib.Attach(DirectX11Manager::CreateIndexBuffer(m_face->idx, (UINT)sizeof(m_face)));
}

// 頂点データを作成
void CBox::CreateVertex() {
	float w2 = 0.5f*m_size.x;
	float h2 = 0.5f*m_size.y;
	float d2 = 0.5f*m_size.z;


	m_vertex[0] = { XMFLOAT3(-w2,  h2, -d2), XMFLOAT3(-1.0f, 1.0f, -1.0f) };
	m_vertex[1] = { XMFLOAT3(w2,  h2, -d2),   XMFLOAT3(1.0f, 1.0f, -1.0f)};
	m_vertex[2] = { XMFLOAT3(w2,  h2,  d2),    XMFLOAT3(1.0f, 1.0f, 1.0f)};
	m_vertex[3] = { XMFLOAT3(-w2,  h2,  d2),   XMFLOAT3(-1.0f, 1.0f, 1.0f)};
	m_vertex[4] = { XMFLOAT3(-w2, -h2, -d2), XMFLOAT3(-1.0f, -1.0f, -1.0f)};
	m_vertex[5] = { XMFLOAT3(w2, -h2, -d2),  XMFLOAT3(1.0f, -1.0f, -1.0f) };
	m_vertex[6] = { XMFLOAT3(w2, -h2,  d2),   XMFLOAT3(1.0f, -1.0f, 1.0f)};
	m_vertex[7] = { XMFLOAT3(-w2, -h2,  d2),  XMFLOAT3(-1.0f, -1.0f, 1.0f)};

	
	vb.Attach(DirectX11Manager::CreateVertexBuffer(m_vertex, (UINT)sizeof(m_vertex)));
	
}

// 描画
void CBox::Draw() {
	DirectX11Manager::SetVertexShader(vs.Get());
	DirectX11Manager::SetPixelShader(ps.Get());
	DirectX11Manager::SetInputLayout(il.Get());

	DirectX11Manager::UpdateConstantBuffer(cb.Get(), DirectX11Manager::m_constantBuffer);
	ID3D11Buffer* tmpCb[] = { cb.Get() };
	DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, tmpCb);

	DirectX11Manager::SetVertexBuffer(vb.Get(), sizeof(Vertex));
	DirectX11Manager::SetIndexBuffer(ib.Get());

	
	DirectX11Manager::UpdateConstantBuffer(cb2.Get(), mate);
	ID3D11Buffer* tmpCb2[] = { cb2.Get() };
	DirectX11Manager::m_pImContext->PSSetConstantBuffers(2, 1, tmpCb2);


	DirectX11Manager::m_pImContext->DrawIndexed(36,		// 描画するインデックス数（面数×３）
		0,									// 最初のインデックスバッファの位置
		0);									// 頂点バッファの最初から使う
}



void CBox::Exit() {

	il.Reset();
	vs.Reset();
}