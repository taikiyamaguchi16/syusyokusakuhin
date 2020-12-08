 #pragma once
#include <list>
#include <Vector>
#include "object.h"
#include "CPhysx.h"
#include "DirectX11Manager.h"

class Scene
{
protected:
	std::list<sp<CObject>> m_obj_list;
	sp<CObject>m_mainCamera;
	std::vector<sp<CObject>>m_subCameras;
	wp<CObject> m_activeCamera;
	//wp<CObject> m_active_obj;
	CObject* m_active_obj;
public:
	virtual ~Scene() {}

	virtual bool Init() = 0;
	virtual void UnInit() = 0;
	virtual void MainLoop() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	inline  std::list<sp<CObject>> GetObjList() {
	
		return m_obj_list;
	}
	std::vector<sp<CObject>> GetCameraObjects() {
		return m_subCameras;
	}
	inline void AddObject(CObject* _o) { 
		sp<CObject>_s(_o);
		m_obj_list.emplace_back(_s); 
	}

	inline void SetActiveObj(CObject* obj_) {
		m_active_obj = obj_;
	}
};


class SceneManager 
{
private:
	sp<Scene> m_activeScene;

public:
	ID3D11ShaderResourceView* m_srv = nullptr;	// SRV
	bool m_frameAdvanceFg = false;			//コマ送り
	int m_fpsCount;							//オブジェクト毎にFPSを変えるための変数
	float m_fps = 144.0f;						//全体のFPS
	SceneManager(){}
	
	bool Init(HINSTANCE hinst, HWND hwnd, int width, int height, bool fullscreen);
	void UnInit();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	inline static SceneManager* GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	inline void ChangeScene(sp<Scene> replacementScene_)
	{
		m_activeScene.Clear();
		m_activeScene = replacementScene_;
		m_activeScene->Update();
	}
	inline void Update() {
		m_activeScene->Update();
	}

	inline void Draw() {
		m_activeScene->Draw();
	}
	inline void MainLoop() 	{
		if (!m_frameAdvanceFg) {
			Update();
		}
		Draw();
	}

	inline void AddObject(CObject* _o) { 
		m_activeScene->AddObject(_o); }
};
