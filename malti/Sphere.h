#pragma once
#include "object.h"
#include "DirectX11Manager.h"

#define  PI  (3.1412)


class CSphere final
{
	Egliss::ComponentSystem::CTransform* m_pos = nullptr;
	
	unsigned int		m_divX;				// ���������̕�����
	unsigned int		m_divY;				// ���������̕�����
	float				m_radius;			// ���a


	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
	};
	VertexBuffer vb;
	IndexBuffer ib;

	//==========================================�ǉ�=========================================
	InputLayout il;
	VertexShader vs;
	PixelShader ps;

	//=======================================================================================

	struct  Material
	{
		XMFLOAT4 df;
	};


	struct Face {
		unsigned int idx[3];
	};

	Vertex					m_vertex[441];			// ���_���W

	Face					m_face[800];				// �ʃC���f�b�N�X�f�[�^
	unsigned int			m_facenum;			// �ʐ�

	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();

public:

	ConstantBuffer cb;
	ConstantBuffer cb2;
	Material mate;
	CSphere() { Init(); }
	~CSphere() { }
	void Init();
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
	void Draw();

	// ������
	bool Init(float r,						// ���a
		int division_horizontal,			// ���������̕�����
		int division_vertical				// ���������̕�����
		);

	// �I������
	void Exit();

};