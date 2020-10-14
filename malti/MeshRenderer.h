#pragma once
#include "object.h"
#include "CModel.h"
#include "Sphere.h"
#include <map>
#include "CBox.h"

class CMeshRenderer final:public CComponent
{
	static inline CSphere* m_sphere = nullptr;
	static inline CBox*    m_box = nullptr;
	float m_radius=1;

	CTransform* m_pos = nullptr;
	bool m_active = true;
	//球やボックスなどのシンプルなメッシュの描画する用のフラグ
	bool m_defaultMeshsMode = true;

	GEOMETRYTYPE m_meshtype = GEOMETRYTYPE::BOX;

	static std::map<std::string, CModel*>m_models;
	static bool m_onceInitFg;
	std::string m_modelName;

	float m_color[4] = { 255.0f,255.0f,255.0f,255.0f };
public:
	~CMeshRenderer() { 
		if (m_sphere != nullptr) {
			m_sphere->Exit();
			delete m_sphere;
		}
		if (m_box != nullptr) {
			m_box->Exit();
			delete m_box;
		}
	}
	void Start()override;
	//void Update()override;
	void LoadModel(const char * filename, const char * vsfile, const char * psfile);
	void Draw();
	void ImGuiDraw()override;

	void SetModel(std::string);
	inline void SetName()override {
		m_name = "MeshRender";
	}

	inline CComponent* CreateAdress()override {
		return new CMeshRenderer();
	}

	inline void SphereInit(float r_) {
		m_defaultMeshsMode = true;
		m_meshtype = GEOMETRYTYPE::SPHERE;
	}

	inline void BoxInit(XMFLOAT3 s_) {
		m_defaultMeshsMode = true;
		m_meshtype = GEOMETRYTYPE::BOX;
	}

	float GetRadius() {
		return m_radius;
	}

	void SetColor(float _col[4]) {
		for (int i = 0; i < 4; i++) {
			m_color[i] = _col[i];
		}
	}

	void SetColor(float _r, float _g, float _b,float _a) {
		m_color[0] = _r;
		m_color[1] = _g;
		m_color[2] = _b;
		m_color[3] = _a;
	}

	static void SetOnceFlag(bool _f) {
		m_onceInitFg = _f;
	}

	static void DeleteModel() {
		m_onceInitFg = true;
		m_models["TIE_Fighter"]->Uninit();
		delete m_models["TIE_Fighter"];
		m_models.clear();
	}

};
