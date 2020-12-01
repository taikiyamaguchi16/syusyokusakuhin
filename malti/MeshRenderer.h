#pragma once
#include "object.h"
#include "Sphere.h"
#include <map>
#include "CBox.h"

#include "UnityExportModel.h"
#include "UniExportModel.hpp"

namespace Egliss::ComponentSystem
{
	class CMeshRenderer final :public CComponent
	{
		//================クラス内で完結しているのでスマートポインタ化しない=====================
		static inline CSphere* m_sphere = nullptr;
		static inline CBox*    m_box = nullptr;
		static inline int m_meshr_render_cnt = 0;
		//=======================================================================================
		float m_radius = 1;

		wp<CTransform> m_pos;
		bool m_active = true;
		//球やボックスなどのシンプルなメッシュの描画する用のフラグ
		bool m_defaultMeshsMode = true;

		GEOMETRYTYPE m_meshtype = GEOMETRYTYPE::BOX;

		static std::map<std::string, UnityExportModel*>m_models;
		static inline std::vector<std::string>m_modelNames;
		std::string m_currentModelName;

		static bool m_onceInitFg;
		//===========================実験======================================
		
		//=====================================================================
		float m_color[4] = { 255.0f,255.0f,255.0f,255.0f };
	public:
		~CMeshRenderer() {
			m_meshr_render_cnt--;
			if (m_meshr_render_cnt == 0) {
				m_onceInitFg = true;
				if (m_sphere != nullptr) {
					m_sphere->Exit();
					delete m_sphere;
				}
				if (m_box != nullptr) {
					m_box->Exit();
					delete m_box;
				}	
				CMeshRenderer::DeleteModel();
			}
		}
		void Start()override;
		void Update()override;

		void Draw();
		void ImGuiDraw()override;

		inline void SetName()override {
			m_name = "MeshRender";
		}

		inline CComponent* CreateAdress()override {
			return new CMeshRenderer();
		}

		inline void SphereInit() {
			m_defaultMeshsMode = true;
			m_meshtype = GEOMETRYTYPE::SPHERE;
		}

		inline void BoxInit() {
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

		void SetColor(float _r, float _g, float _b, float _a) {
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

			for (auto itr : m_models)
			{
				delete	(itr.second);
			}
			m_models.clear();
		}

		static void DrawModel(std::string _str)
		{
			m_models[_str]->Draw();
		}

	};
}
