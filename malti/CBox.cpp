#include "CBox.h"
#include "Scene.h"
#include "UnityExportModel.h"

void CBox::Init(XMFLOAT3 s_)
{
	m_size = s_;
	//Init(GetDX11Device());

	//vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/UnityExportModel.hlsl", "vsMain"));
	//ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/UnityExportModel.hlsl", "psMain"));

	////InputLayout�̍쐬
	//D3D11_INPUT_ELEMENT_DESC elem[] = {
	//	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//	{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//};
	//il.Attach(DirectX11Manager::CreateInputLayout(elem, 4, "assets/Shaders/UnityExportModel.hlsl", "vsMain"));

	Init(DirectX11Manager::m_pDevice.Get());

	//�R���X�^���g�o�b�t�@�̍쐬
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::m_constantBuffer.proj = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
			SCREEN_X / SCREEN_X, 0.5f, 4096.0f * 8.0f));
	//m_cube.LoadBinary("assets/Models/Cube.bin");
}

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void CBox::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector);			// XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v);			// ���K��
	XMStoreFloat3(&Normal, v);			// XMVECTOR=>XMFLOAT3
}

// �C���f�b�N�X�f�[�^���쐬
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

// ���_�f�[�^���쐬
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

// �`��
void CBox::Draw() {

	//// �萔�o�b�t�@��������
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
	//// ���_�o�b�t�@���Z�b�g����
	//unsigned int stride = sizeof(Vertex);
	//unsigned  offset = 0;
	//device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	//device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	//device->IASetInputLayout(m_pVertexLayout);					// ���_���C�A�E�g�Z�b�g

	//device->VSSetShader(m_pVertexShader, nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	//device->PSSetShader(m_pPixelShader, nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g


	////device->PSSetConstantBuffers(3, 1, &m_cbuffer);
	//device->PSSetConstantBuffers(3, 1, tmpCb);
	//device->DrawIndexed(36,		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
	//	0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
	//	0);									// ���_�o�b�t�@�̍ŏ�����g��
}


bool CBox::Init(ID3D11Device* device) {
	//bool sts;
	//
	//// ���̃C���f�b�N�X�f�[�^���쐬
	//CreateIndex();

	//// ���̒��_�f�[�^���쐬
	//CreateVertex();

	//// ���_�o�b�t�@�쐬
	//sts = CreateVertexBufferWrite(
	//	device,
	//	sizeof(Vertex),						// �P���_������o�C�g��
	//	8,			// ���_��
	//	m_vertex,							// ���_�f�[�^�i�[�������擪�A�h���X
	//	&m_pVertexBuffer);					// ���_�o�b�t�@
	//if (!sts) {
	//	MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
	//	return false;
	//}

	//// �C���f�b�N�X�o�b�t�@�쐬
	//sts = CreateIndexBuffer(
	//	device,
	//	36,	// �C���f�b�N�X��
	//	m_face,					// �C���f�b�N�X�f�[�^�擪�A�h���X
	//	&m_pIndexBuffer);		// �C���f�b�N�X�o�b�t�@
	//if (!sts) {
	//	MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
	//	return false;
	//}


	//// �萔�o�b�t�@����
	//sts = CreateConstantBufferWrite(DirectX11Manager::m_pDevice.Get(), sizeof(ConstantBufferMaterial), &m_cbuffer);
	//if (!sts) {
	//	MessageBox(nullptr, "CreateConstantBufferWrite error", "error", MB_OK);
	//	return false;
	//}


	//// ���_�f�[�^�̒�`
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//unsigned int numElements = ARRAYSIZE(layout);

	//// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
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

	//// �s�N�Z���V�F�[�_�[�𐶐�
	//sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
	//	device,							// �f�o�C�X�I�u�W�F�N�g
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


	// �s�N�Z���V�F�[�_�[���
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// ���_�V�F�[�_�[���
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