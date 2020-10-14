#include	<windows.h>
#include	"CModel.h"
#include	"Shader.h"

#include	"DX11util.h"
#include	"DX11Settransform.h"

bool CModel::Init(const char* filename, const char* vsfile, const char* psfile) {

	bool sts;

	// ��s�@�̃��f���f�[�^��ǂݍ���
	sts = m_datfile.Load(filename, GetDX11Device(), GetDX11DeviceContext());
	if (!sts) {
		char str[128];
		sprintf_s(str, 128,"%s load ERROR!!", filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}

	// ��s�@�p�̒��_�f�[�^�̒�`
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

	return true;
}

void CModel::Uninit() {
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
}

void CModel::Update() {
}

void CModel::Draw() {
	// ���_�t�H�[�}�b�g���Z�b�g
	GetDX11DeviceContext()->IASetInputLayout(m_pVertexLayout);

	// ���_�V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->VSSetShader(m_pVertexShader, nullptr, 0);

	// �s�N�Z���V�F�[�_�[���Z�b�g
	GetDX11DeviceContext()->PSSetShader(m_pPixelShader, nullptr, 0);

	// ���f���`��
	m_datfile.Draw(GetDX11DeviceContext());
}