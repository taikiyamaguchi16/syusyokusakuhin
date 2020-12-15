#include "Sphere.h"


void CSphere::Init()
{
	Init(1.f, 10, 10);
	vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/DefaultShape.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/DefaultShape.hlsl", "psMain"));

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 3, "assets/Shaders/DefaultShape.hlsl", "vsMain"));

	//コンスタントバッファの作成
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::CreateConstantBuffer(sizeof(Material), &cb2);

	mate.df = XMFLOAT4(1, 1, 1, 1);
}

// 法線ベクトルを計算
// ノーマライズ
void CSphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector);			// XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v);			// 正規化
	XMStoreFloat3(&Normal, v);			// XMVECTOR=>XMFLOAT3
}

// インデックスデータを作成
void CSphere::CreateIndex() {

	// 面数セット
	m_facenum = m_divY * m_divX * 2;

	//m_face = new Face[m_facenum];

//	Face* pface = m_face;
	int pface = 0;
	// インデックス生成
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1)*y + x;			// 左上座標のインデックス

			// 上半分
			m_face[pface].idx[0] = count;
			m_face[pface].idx[1] = count + 1;
			m_face[pface].idx[2] = count + 1 + (m_divX + 1);

			pface++;

			// 下半分
			m_face[pface].idx[0] = count;
			m_face[pface].idx[1] = count + (m_divX + 1) + 1;
			m_face[pface].idx[2] = count + (m_divX + 1);

			pface++;
		}
	}

	ib.Attach(DirectX11Manager::CreateIndexBuffer(m_face->idx, (UINT)sizeof(m_face)));
}

// 頂点データを作成
void CSphere::CreateVertex() {

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	//m_vertex = new Vertex[(m_divX + 1) * (m_divY + 1)];
	//Vertex* pvtx = m_vertex;
	int pvtx = 0;
	XMFLOAT3	Normal;
	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (float)(PI * (float)y) / (float)m_divY;    // 仰角をセット
		float r = m_radius * sinf(elevation);					// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (float)(2.0f * PI * (float)x) / (float)m_divX;	// 方位角をセット

			// 頂点座標セット
			m_vertex[pvtx].Pos.x = r * cosf(azimuth);
			m_vertex[pvtx].Pos.y = m_radius * cosf(elevation);
			m_vertex[pvtx].Pos.z = r * sinf(azimuth);

			// 法線ベクトルセット
			Normalize(m_vertex[pvtx].Pos, Normal);		// 法線を計算
			m_vertex[pvtx].Normal = Normal;				// 法線をセット
			// 次へ
			pvtx++;
		}
	}
	vb.Attach(DirectX11Manager::CreateVertexBuffer(m_vertex, (UINT)sizeof(m_vertex)));
}

// 描画
void CSphere::Draw() {
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


	DirectX11Manager::m_pImContext->DrawIndexed(m_facenum * 3,		// 描画するインデックス数（面数×３）
		0,									// 最初のインデックスバッファの位置
		0);									// 頂点バッファの最初から使う
}



bool CSphere::Init(float r,				// 半径
	int division_horizontal,			// 水平方向の分割数
	int division_vertical				// 垂直方向の分割数
	) {
	// 分割数を保存
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// 半径を保存
	m_radius = r;

	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	return true;
}

void CSphere::Exit() {

	//if (m_vertex) {
	//	delete[] m_vertex;
	//}

	//if (m_face) {
	//	delete[] m_face;
	//}
}