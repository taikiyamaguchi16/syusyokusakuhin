#include	<windows.h>
#include	"CObject2D.h"
#include	"Shader.h"

#include	"DX11util.h"
#include	"Camera.h"
#include	"DX11Settransform.h"

bool CObject2D::Init(const char* filename, const char* vsfile, const char* psfile) {
	c_fg = false;
	alfa = 1.0f;
	bool sts;

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(GetDX11Device(),
		vsfile,
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
		GetDX11Device(),		// �f�o�C�X�I�u�W�F�N�g
		psfile,
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}
	m_geight = 60.0f;
	m_width = 100.0f;
	// ���_���W
	Vertex	v[4] = {
		XMFLOAT3(-m_width/2.0f,  m_geight/2.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f),
		XMFLOAT3(m_width / 2.0f,  m_geight / 2.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f),
		XMFLOAT3(-m_width / 2.0f, -m_geight / 2.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f),
		XMFLOAT3(m_width / 2.0f, -m_geight / 2.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)
	};

	// ���_�o�b�t�@����
	CreateVertexBuffer(GetDX11Device(), sizeof(Vertex), 4, v, &m_pVertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	unsigned int idx[4] = { 0, 1, 2, 3 };

	// �C���f�b�N�X�o�b�t�@����
	CreateIndexBuffer(GetDX11Device(), 4, idx, &m_pIndexBuffer);

	// �萔�o�b�t�@����
	CreateConstantBufferWrite(GetDX11Device(), sizeof(ConstantBufferMaterial), &m_pCBuffer);

	// SRV����
	CreateSRVfromFile(filename, GetDX11Device(), GetDX11DeviceContext(), &m_texRes, &m_texSRV);

	

	// �萔�o�b�t�@��������
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetDX11DeviceContext()->Map(m_pCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&material), sizeof(ConstantBufferMaterial));
		GetDX11DeviceContext()->Unmap(m_pCBuffer, 0);
	}

	return true;
}

void CObject2D::Uninit() {

	if (m_pVertexBuffer != nullptr) {
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pIndexBuffer != nullptr) {
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pCBuffer != nullptr) {
		m_pCBuffer->Release();
		m_pCBuffer = nullptr;
	}

	// ���_�V�F�[�_�[���
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	// �s�N�Z���V�F�[�_�[���
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// ���_���C�A�E�g���
	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}

	if (m_texSRV != nullptr) {
		m_texSRV->Release();
		m_texSRV = nullptr;
	}

	if (m_texRes != nullptr) {
		m_texRes->Release();
		m_texRes = nullptr;
	}
}

void CObject2D::Update() {
}

void CObject2D::Draw() {

	XMFLOAT4X4 positionMatrix, rotMtx;

	DX11MtxTranslation(m_pos, positionMatrix);

	DX11MtxInverse(rotMtx, m_cameraMtx);
	rotMtx._41 = 0;
	rotMtx._42 = 0;
	rotMtx._43 = 0;

	m_matrix = positionMatrix;
	

	DX11MtxMultiply(m_matrix, rotMtx, positionMatrix);

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_matrix);

	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetDX11DeviceContext()->Map(m_pCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&material), sizeof(ConstantBufferMaterial));
		GetDX11DeviceContext()->Unmap(m_pCBuffer, 0);
	}
	// ���_�V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);

	// �s�N�Z���V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);

	// �g�|���W�[���Z�b�g
	GetDX11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@���Z�b�g
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	GetDX11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@���Z�b�g
	GetDX11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// PS��SRV���Z�b�g
	GetDX11DeviceContext()->PSSetShaderResources(0, 1, &m_texSRV);

	// PS�ɒ萔�o�b�t�@���Z�b�g
	GetDX11DeviceContext()->PSSetConstantBuffers(3, 1, &m_pCBuffer);

	// GPU�R�}���h�𔭍s
	GetDX11DeviceContext()->DrawIndexed(4, 0, 0);
}

bool CObject2D::CountUp()
{
	if (c_fg) {
		alfa -=0.1f;
		material.DiffuseMaterial = { 1.0f, 1.0f, 1.0f, alfa };
		if (alfa < 0) {
			return true;
		}
	}
	return false;
}

//****************************************************************************************************
//	End of file.
//****************************************************************************************************