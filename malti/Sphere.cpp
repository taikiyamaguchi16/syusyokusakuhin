#include "Sphere.h"


void CSphere::Init(float r_)
{
	Init(r_, 20, 20, DirectX11Manager::m_pDevice.Get());
}

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void CSphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {

	DirectX::XMVECTOR v;
	v = XMLoadFloat3(&vector);			// XMFLOAT3=>XMVECTOR
	v = XMVector3Normalize(v);			// ���K��
	XMStoreFloat3(&Normal, v);			// XMVECTOR=>XMFLOAT3
}

// �C���f�b�N�X�f�[�^���쐬
void CSphere::CreateIndex() {

	// �ʐ��Z�b�g
	m_facenum = m_divY * m_divX * 2;

	m_face = new Face[m_facenum];

	Face* pface = m_face;

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1)*y + x;			// ������W�̃C���f�b�N�X

			// �㔼��
			pface->idx[0] = count;
			pface->idx[1] = count + 1;
			pface->idx[2] = count + 1 + (m_divX + 1);

			pface++;

			// ������
			pface->idx[0] = count;
			pface->idx[1] = count + (m_divX + 1) + 1;
			pface->idx[2] = count + (m_divX + 1);

			pface++;
		}
	}
}

// ���_�f�[�^���쐬
void CSphere::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	m_vertex = new Vertex[(m_divX + 1) * (m_divY + 1)];
	Vertex* pvtx = m_vertex;
	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (PI * (float)y) / (float)m_divY;    // �p���Z�b�g
		float r = m_radius * sinf(elevation);					// �p�ɉ��������a���v�Z

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (2.0f * PI * (float)x) / (float)m_divX;	// ���ʊp���Z�b�g

			// ���_���W�Z�b�g
			pvtx->Pos.x = r * cosf(azimuth);
			pvtx->Pos.y = m_radius * cosf(elevation);
			pvtx->Pos.z = r * sinf(azimuth);

			// �@���x�N�g���Z�b�g
			Normalize(pvtx->Pos, Normal);		// �@�����v�Z
			pvtx->Normal = Normal;				// �@�����Z�b�g
			// ����
			pvtx++;
		}
	}

}

// �`��
void CSphere::Draw() {

	// �萔�o�b�t�@��������
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = DirectX11Manager::m_pImContext->Map(m_cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&m_material), sizeof(ConstantBufferMaterial));
		DirectX11Manager::m_pImContext->Unmap(m_cbuffer, 0);
	}

	ID3D11DeviceContext* device = DirectX11Manager::m_pImContext.Get();
	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	device->IASetInputLayout(m_pVertexLayout);					// ���_���C�A�E�g�Z�b�g

	device->VSSetShader(m_pVertexShader, nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	device->PSSetShader(m_pPixelShader, nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g


	device->PSSetConstantBuffers(3, 1, &m_cbuffer);

	device->DrawIndexed(m_facenum * 3,		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}



bool CSphere::Init(float r,				// ���a
	int division_horizontal,			// ���������̕�����
	int division_vertical,				// ���������̕�����
	ID3D11Device* device) {
	bool sts;
	// ��������ۑ�
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// ���a��ۑ�
	m_radius = r;

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// �P���_������o�C�g��
		(m_divY + 1)*(m_divX + 1),			// ���_��
		m_vertex,							// ���_�f�[�^�i�[�������擪�A�h���X
		&m_pVertexBuffer);					// ���_�o�b�t�@
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
		device,
		(m_divY)*(m_divX) * 2 * 3,	// �C���f�b�N�X��
		m_face,					// �C���f�b�N�X�f�[�^�擪�A�h���X
		&m_pIndexBuffer);		// �C���f�b�N�X�o�b�t�@
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// �萔�o�b�t�@����
	sts = CreateConstantBufferWrite(DirectX11Manager::m_pDevice.Get(), sizeof(ConstantBufferMaterial), &m_cbuffer);
	if (!sts) {
		MessageBox(nullptr, "CreateConstantBufferWrite error", "error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(device,
		"shader/3dspherevs.fx",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		device,							// �f�o�C�X�I�u�W�F�N�g
		"shader/3dsphereps.fx",
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	return true;
}

void CSphere::Exit() {

	if (m_vertex) {
		delete[] m_vertex;
	}

	if (m_face) {
		delete[] m_face;
	}

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

	if (m_cbuffer) {
		m_cbuffer->Release();
		m_cbuffer = nullptr;
	}
}