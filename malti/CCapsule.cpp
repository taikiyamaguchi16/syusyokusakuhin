#include "CCapsule.h"

void CCapsule::CreateIndex()
{
}

void CCapsule::CreateVertex()
{
	//float slices = 10.f;
	//// 分割数
	//FLOAT invSlices = 1.0f / slices;
	//FLOAT invStacks = 1.0f / slices;

	////--  カプセル座標の単位ベクトルを作成する  --//

	//XMFLOAT3 uz = p1 - p2;	// vz : P2からP1へ向かう向きのベクトル

	//XMFLOAT3 uy;			// vy : vzとvと直角なベクトル
	//D3DXVec3Cross(&uy, &uz, &v);

	//XMFLOAT3 ux;			// vx : vzとvyと直角なベクトル
	//D3DXVec3Cross(&ux, &uz, &uy);
	//// vz・vyが0ベクトルでなければvxも0ベクトルではないのでチェックは不要
	////if (D3DXVec3LengthSq(&uy)==0.0f) return E_INVALIDARG;

	//// ベクトルの大きさをワールド座標でのradiusにする
	//DX11Vec3Normalize(ux, ux);
	//D3DXVec3Scale(&ux, &ux, -radius);
	//DX11Vec3Normalize(uy, uy);
	//D3DXVec3Scale(&uy, &uy, radius);
	//DX11Vec3Normalize(uz, uz);
	//D3DXVec3Scale(&uz, &uz, radius);

	////--  頂点計算  --//

	//// 変数定義
	//LPDIRECT3DVERTEXBUFFER9 pVB;
	//HRESULT hr;

	//// 頂点バッファの数(格子点+端点)
	//UINT numberOfVB = (1 + slices * stacks_1_2) * 2;

	//// 頂点バッファ作成
	//hr = pd3dDevice->CreateVertexBuffer(
	//	sizeof(CUSTOMVERTEX_XYZ_NORMAL)*numberOfVB,
	//	0,
	//	CUSTOMVERTEX_XYZ_NORMAL::GetFVF(),
	//	D3DPOOL_DEFAULT,
	//	&pVB,
	//	NULL);
	//if (FAILED(hr)) return hr;

	//// 頂点バッファのロック
	//CUSTOMVERTEX_XYZ_NORMAL *pV;
	//hr = pVB->Lock(0, 0, (void**)&pV, 0);
	//if (FAILED(hr))
	//{
	//	pVB->Release();
	//	return hr;
	//}

	//// P1側の計算
	//SetVertex(pV, p1, uz);
	//for (UINT i = 0; i < stacks_1_2; i++)
	//{
	//	FLOAT t = (D3DX_PI*0.5f)*invStacks*(i + 1);
	//	FLOAT r = sinf(t);
	//	XMFLOAT3 vx = ux * r;
	//	XMFLOAT3 vy = uy * r;
	//	XMFLOAT3 vz = uz * cosf(t);
	//	for (UINT j = 0; j < slices; j++)
	//	{
	//		FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
	//		SetVertex(pV, p1, vx*cosf(u) + vy * sinf(u) + vz);
	//	}
	//}

	//// P2側の計算
	//for (UINT i = 0; i < stacks_1_2; i++)
	//{
	//	FLOAT t = (D3DX_PI*0.5f)*invStacks*i;
	//	FLOAT r = cosf(t);
	//	XMFLOAT3 vx = ux * r;
	//	XMFLOAT3 vy = uy * r;
	//	XMFLOAT3 vz = uz * -sinf(t);
	//	for (UINT j = 0; j < slices; j++)
	//	{
	//		FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
	//		SetVertex(pV, p2, vx*cosf(u) + vy * sinf(u) + vz);
	//	}
	//}
	//SetVertex(pV, p2, -uz);
}
