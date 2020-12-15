#pragma once
#include "object.h"
#include "DirectX11Manager.h"
#include "UnityExportModel.h"

class CBox
{

	Egliss::ComponentSystem::CTransform* m_pos = nullptr;
	VertexBuffer vb;
	IndexBuffer ib;

	//==========================================�ǉ�=========================================
	InputLayout il;
	VertexShader vs;
	PixelShader ps;

	//=======================================================================================

	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
	};

	struct  Material
	{
		XMFLOAT4 df;
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

	ConstantBuffer cb;
	ConstantBuffer cb2;
	Material mate;
	CBox() {
		Init();
	}
	~CBox() { Exit(); }
	void Init();
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);

	void Draw();


	// �I������
	void Exit();
};

