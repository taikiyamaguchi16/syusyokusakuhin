#pragma once
#include "object.h"

#define  PI  (3.1412)

class CSphere final
{
	CTransform* m_pos = nullptr;
	ID3D11Buffer*	m_cbuffer = nullptr;		// �萔�o�b�t�@	
	ID3D11Buffer*		m_pVertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer*		m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ID3D11VertexShader* m_pVertexShader;	// ���_�V�F�[�_�[
	ID3D11PixelShader*  m_pPixelShader;		// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*  m_pVertexLayout;	// ���_���C�A�E�g
	unsigned int		m_divX;				// ���������̕�����
	unsigned int		m_divY;				// ���������̕�����
	float				m_radius;			// ���a

	
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

	Vertex*					m_vertex;			// ���_���W

	Face*					m_face;				// �ʃC���f�b�N�X�f�[�^
	unsigned int			m_facenum;			// �ʐ�

	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();

public:
	~CSphere() { }
	void Init(float r_);
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
	void Draw();

	// ������
	bool Init(float r,						// ���a
		int division_horizontal,			// ���������̕�����
		int division_vertical,				// ���������̕�����
		ID3D11Device* device);

	// �I������
	void Exit();

	inline void SetDiffuseMaterial(float col_[4]){
		m_material.DiffuseMaterial = XMFLOAT4(col_[0], col_[1], col_[2], col_[3]);
	}
};

