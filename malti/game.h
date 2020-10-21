//*****************************************************************************
//!	@file	game.h
//!	@brief	ゲーム処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once
#include "Scene.h"
#include <iostream>
#include <vector>


class CGame final:public Scene
{
	
	//ID3D11Buffer*	m_vertexbuffer = nullptr;	// 頂点バッファ
	//ID3D11Buffer*	m_idxbuffer = nullptr;		// インデックスバッファ	
	//ID3D11Buffer*	m_cbuffer = nullptr;		// 定数バッファ	

	//ID3D11VertexShader*	m_vs = nullptr;			// 頂点シェーダー
	//ID3D11PixelShader* m_ps = nullptr;			// ピクセルシェーダー
	//ID3D11InputLayout* m_layout = nullptr;		// 頂点レイアウト

	//ID3D11ShaderResourceView* m_srv = nullptr;	// SRV
	//ID3D11Resource* m_texres = nullptr;			// texture resource


public:
	//頻繁に削除が行われるため
	std::list<CComponent*> m_Components;
	CGame() { Init(); }
	~CGame() { UnInit(); }

	bool Init()override;
	void UnInit()override;
	void Update()override;
	void MainLoop()override;
	void Draw()override;

	
	inline void CreateObject(std::string str_) {
		CObject* obj = new CObject;
		obj->AddComponent<CTransform>();
		obj->SetName(str_);
		m_obj_list.emplace_back(obj);
	}

	template<typename T> 
	T * createInstance() { return new T; }


	void AddObjects();
};


//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//******************************************************************************
//	End of file.
//******************************************************************************
