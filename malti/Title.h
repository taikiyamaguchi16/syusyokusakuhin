#pragma once
#include "Scene.h"

class Title :public Scene
{
	CObject* m_active_obj = nullptr;
public:
	Title() { Init(); }
	~Title() { UnInit(); }

	bool Init()override;
	void UnInit()override;
	void MainLoop()override;
	void Update()override;
	void Draw()override;

	template<typename T>
	inline void SetActiveObj(T* obj_) {
		m_active_obj = obj_;
	}

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
