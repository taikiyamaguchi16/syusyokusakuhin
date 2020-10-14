#pragma once
#include "object.h"

#define  PI  (3.1412)

class CSphere final
{
	CTransform* m_pos = nullptr;
	ID3D11Buffer*	m_cbuffer = nullptr;		// 定数バッファ	
	ID3D11Buffer*		m_pVertexBuffer;	// 頂点バッファ
	ID3D11Buffer*		m_pIndexBuffer;		// インデックスバッファ
	ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダー
	ID3D11PixelShader*  m_pPixelShader;		// ピクセルシェーダー
	ID3D11InputLayout*  m_pVertexLayout;	// 頂点レイアウト
	unsigned int		m_divX;				// 水平方向の分割数
	unsigned int		m_divY;				// 垂直方向の分割数
	float				m_radius;			// 半径

	
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
	};

	// 定数バッファ定義（マテリアル単位）
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// 環境光の反射率
		XMFLOAT4	DiffuseMaterial;		// ディフューズ光の反射率
		XMFLOAT4	SpecularMaterial;		// スペキュラ光の反射率
	};

	// マテリアル
	ConstantBufferMaterial m_material = {
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// 環境光マテリアル
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// 拡散反射マテリアル
		XMFLOAT4(0.0f,1.0f,0.0f,1.0f),			// 鏡面反射マテリアル
	};

	struct Face {
		unsigned int idx[3];
	};

	Vertex*					m_vertex;			// 頂点座標

	Face*					m_face;				// 面インデックスデータ
	unsigned int			m_facenum;			// 面数

	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
	void CreateVertex();

public:
	~CSphere() { }
	void Init(float r_);
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
	void Draw();

	// 初期化
	bool Init(float r,						// 半径
		int division_horizontal,			// 水平方向の分割数
		int division_vertical,				// 垂直方向の分割数
		ID3D11Device* device);

	// 終了処理
	void Exit();

	inline void SetDiffuseMaterial(float col_[4]){
		m_material.DiffuseMaterial = XMFLOAT4(col_[0], col_[1], col_[2], col_[3]);
	}
};

