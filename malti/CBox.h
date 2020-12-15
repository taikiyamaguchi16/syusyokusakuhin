#pragma once
#include "object.h"
#include "DirectX11Manager.h"
#include "UnityExportModel.h"

class CBox
{

	Egliss::ComponentSystem::CTransform* m_pos = nullptr;
	VertexBuffer vb;
	IndexBuffer ib;

	//==========================================追加=========================================
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

	Vertex					m_vertex[8];			// 頂点座標

	Face					m_face[12];				// 面インデックスデータ

	
	XMFLOAT3 m_size = XMFLOAT3(1, 1, 1);
	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
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


	// 終了処理
	void Exit();
};

