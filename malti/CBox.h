#pragma once
#include "object.h"
class CBox
{
	CTransform* m_pos = nullptr;
	ID3D11Buffer*	m_cbuffer = nullptr;		// �萔�o�b�t�@	
	ID3D11Buffer*		m_pVertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer*		m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ID3D11VertexShader* m_pVertexShader;	// ���_�V�F�[�_�[
	ID3D11PixelShader*  m_pPixelShader;		// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*  m_pVertexLayout;	// ���_���C�A�E�g


	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
	};

	// �萔�o�b�t�@��`�i�}�e���A���P�ʁj
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// �����̔��˗�
		XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
		XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
	};

	// �}�e���A��
	ConstantBufferMaterial m_material = {
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// �����}�e���A��
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// �g�U���˃}�e���A��
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// ���ʔ��˃}�e���A��
	};

	struct Face {
		unsigned int idx[3];
	};

	Vertex					m_vertex[8];			// ���_���W

	Face					m_face[12];				// �ʃC���f�b�N�X�f�[�^

	XMFLOAT3 m_size = XMFLOAT3(1, 1, 1);
	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();

public:
	~CBox() {  }
	void Init(XMFLOAT3 s_);
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);

	void Draw();

	inline void SetDiffuseMaterial(float col_[4]) {
		m_material.DiffuseMaterial = XMFLOAT4(col_[0], col_[1], col_[2], col_[3]);
	}

	// ������
	bool Init(ID3D11Device* device);

	// �I������
	void Exit();
};

