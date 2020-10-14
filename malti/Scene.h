 #pragma once
#include <list>
#include <Vector>
#include "object.h"
#include "CPhysx.h"

class Scene
{
protected:
	std::list<CObject*> m_obj_list;
	CObject* m_mainCamera;
	std::vector<CObject*>m_subCameras;
	CObject* m_activeCamera;
public:
	virtual ~Scene() {}

	virtual bool Init() = 0;
	virtual void UnInit() = 0;
	virtual void MainLoop() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	inline  std::list<CObject*> GetObjList() {
		return m_obj_list;
	}
	std::vector<CObject*> GetCameraObjects() {
		return m_subCameras;
	}
	inline void AddObject(CObject* _o) { m_obj_list.emplace_back(_o); }
};


class SceneManager 
{
private:
	Scene* m_activeScene;

	ID3D11Buffer*	m_vertexbuffer = nullptr;	// 頂点バッファ
	ID3D11Buffer*	m_idxbuffer = nullptr;		// インデックスバッファ	
	ID3D11Buffer*	m_cbuffer = nullptr;		// 定数バッファ	

	ID3D11VertexShader*	m_vs = nullptr;			// 頂点シェーダー
	ID3D11PixelShader* m_ps = nullptr;			// ピクセルシェーダー
	ID3D11InputLayout* m_layout = nullptr;		// 頂点レイアウト

	
	ID3D11Resource* m_texres = nullptr;			// texture resource

	// 定数バッファ定義（マテリアル単位）
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// 環境光の反射率
		XMFLOAT4	DiffuseMaterial;		// ディフューズ光の反射率
		XMFLOAT4	SpecularMaterial;		// スペキュラ光の反射率
	};
public:
	ID3D11ShaderResourceView* m_srv = nullptr;	// SRV
	bool m_frameAdvanceFg = false;			//コマ送り
	int m_fpsCount;							//オブジェクト毎にFPSを変えるための変数
	float m_fps = 60.0;						//全体のFPS
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

	inline void ChangeScene(Scene* replacementScene_)
	{
		delete m_activeScene;
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

	inline void AddObject(CObject* _o) { m_activeScene->AddObject(_o); }
};
