#pragma once
#include "object.h"
#include <map>
#include "UnityExportSkinnedModel.h"
namespace Egliss::ComponentSystem
{
	class CSkineMeshRenderer :public CComponent
	{
		static inline int m_mesh_cnt = 0;
		wp<CTransform> m_pos;
		bool m_active = true;

		static std::map<std::string, UnityExportSkinnedModel*>m_skinnedModels;
		static inline std::vector<std::string>m_skinnedModelNames;
		std::string m_currentModelName;
		static bool m_onceInitFg;
		//===========================ŽÀŒ±======================================
		uem::SkinnedAnimation animation;
		uem::SkinnedAnimation animation2;
		float animtime = 0.f;
		float animtime2 = 0.f;
		//=====================================================================
	public:
	
		~CSkineMeshRenderer()
		{
			m_mesh_cnt--;
			if (m_mesh_cnt == 0) {
				m_onceInitFg = true;	
				CSkineMeshRenderer::DeleteModel();
			}
		}
		void Start()override;

		void Draw();
		void ImGuiDraw()override;

		inline void SetName()override {
			m_name = "SkineMeshRenderer";
		}
		inline void DeleteModel()
		{
			for (auto itr : m_skinnedModels)
			{
				delete	(itr.second);
			}
			m_skinnedModels.clear();
		}
	};

}
