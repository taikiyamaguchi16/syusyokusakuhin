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

	ID3D11Buffer*	m_vertexbuffer = nullptr;	// ���_�o�b�t�@
	ID3D11Buffer*	m_idxbuffer = nullptr;		// �C���f�b�N�X�o�b�t�@	
	ID3D11Buffer*	m_cbuffer = nullptr;		// �萔�o�b�t�@	

	ID3D11VertexShader*	m_vs = nullptr;			// ���_�V�F�[�_�[
	ID3D11PixelShader* m_ps = nullptr;			// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* m_layout = nullptr;		// ���_���C�A�E�g

	
	ID3D11Resource* m_texres = nullptr;			// texture resource

	// �萔�o�b�t�@��`�i�}�e���A���P�ʁj
	struct ConstantBufferMaterial {
		XMFLOAT4	AmbientMaterial;		// �����̔��˗�
		XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
		XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
	};
public:
	ID3D11ShaderResourceView* m_srv = nullptr;	// SRV
	bool m_frameAdvanceFg = false;			//�R�}����
	int m_fpsCount;							//�I�u�W�F�N�g����FPS��ς��邽�߂̕ϐ�
	float m_fps = 60.0;						//�S�̂�FPS
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
