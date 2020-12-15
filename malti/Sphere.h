#pragma once
#include "object.h"
#include "DirectX11Manager.h"

#define  PI  (3.1412)


class CSphere final
{
	Egliss::ComponentSystem::CTransform* m_pos = nullptr;
	
	unsigned int		m_divX;				// 水平方向の分割数
	unsigned int		m_divY;				// 垂直方向の分割数
	float				m_radius;			// 半径


	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
	};
	VertexBuffer vb;
	IndexBuffer ib;

	//==========================================追加=========================================
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

	Vertex					m_vertex[441];			// 頂点座標

	Face					m_face[800];				// 面インデックスデータ
	unsigned int			m_facenum;			// 面数

	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
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

	// 初期化
	bool Init(float r,						// 半径
		int division_horizontal,			// 水平方向の分割数
		int division_vertical				// 垂直方向の分割数
		);

	// 終了処理
	void Exit();

};