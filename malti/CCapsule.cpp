#include "CCapsule.h"

void CCapsule::CreateIndex()
{
}

void CCapsule::CreateVertex()
{
	//float slices = 10.f;
	//// ������
	//FLOAT invSlices = 1.0f / slices;
	//FLOAT invStacks = 1.0f / slices;

	////--  �J�v�Z�����W�̒P�ʃx�N�g�����쐬����  --//

	//XMFLOAT3 uz = p1 - p2;	// vz : P2����P1�֌����������̃x�N�g��

	//XMFLOAT3 uy;			// vy : vz��v�ƒ��p�ȃx�N�g��
	//D3DXVec3Cross(&uy, &uz, &v);

	//XMFLOAT3 ux;			// vx : vz��vy�ƒ��p�ȃx�N�g��
	//D3DXVec3Cross(&ux, &uz, &uy);
	//// vz�Evy��0�x�N�g���łȂ����vx��0�x�N�g���ł͂Ȃ��̂Ń`�F�b�N�͕s�v
	////if (D3DXVec3LengthSq(&uy)==0.0f) return E_INVALIDARG;

	//// �x�N�g���̑傫�������[���h���W�ł�radius�ɂ���
	//DX11Vec3Normalize(ux, ux);
	//D3DXVec3Scale(&ux, &ux, -radius);
	//DX11Vec3Normalize(uy, uy);
	//D3DXVec3Scale(&uy, &uy, radius);
	//DX11Vec3Normalize(uz, uz);
	//D3DXVec3Scale(&uz, &uz, radius);

	////--  ���_�v�Z  --//

	//// �ϐ���`
	//LPDIRECT3DVERTEXBUFFER9 pVB;
	//HRESULT hr;

	//// ���_�o�b�t�@�̐�(�i�q�_+�[�_)
	//UINT numberOfVB = (1 + slices * stacks_1_2) * 2;

	//// ���_�o�b�t�@�쐬
	//hr = pd3dDevice->CreateVertexBuffer(
	//	sizeof(CUSTOMVERTEX_XYZ_NORMAL)*numberOfVB,
	//	0,
	//	CUSTOMVERTEX_XYZ_NORMAL::GetFVF(),
	//	D3DPOOL_DEFAULT,
	//	&pVB,
	//	NULL);
	//if (FAILED(hr)) return hr;

	//// ���_�o�b�t�@�̃��b�N
	//CUSTOMVERTEX_XYZ_NORMAL *pV;
	//hr = pVB->Lock(0, 0, (void**)&pV, 0);
	//if (FAILED(hr))
	//{
	//	pVB->Release();
	//	return hr;
	//}

	//// P1���̌v�Z
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

	//// P2���̌v�Z
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
