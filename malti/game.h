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
public:
	//頻繁に削除が行われるため
	std::list<Egliss::ComponentSystem::CComponent*> m_Components;
	CGame() { Init(); }
	~CGame() { UnInit(); }

	bool Init()override;
	void UnInit()override;
	void Update()override;
	void MainLoop()override;
	void Draw()override;

	
	inline void CreateObject(std::string str_) {
		CObject* obj = new CObject;
		obj->AddComponent<Egliss::ComponentSystem::CTransform>();
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
