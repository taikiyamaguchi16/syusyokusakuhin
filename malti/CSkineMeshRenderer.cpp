#include "CSkineMeshRenderer.h"
#include <filesystem>

using namespace Egliss::ComponentSystem;

namespace fs = std::filesystem;

std::map<std::string, UnityExportSkinnedModel*>CSkineMeshRenderer::m_skinnedModels;
bool CSkineMeshRenderer::m_onceInitFg = true;

void CSkineMeshRenderer::Start()
{
	m_mesh_cnt++;
	m_name = "CSkineMeshRenderer";
	//一度だけ読み込み実行
	if (m_onceInitFg) {

		std::string path = "assets/SkineMeshModels";
		for (auto & p : fs::directory_iterator(path))
		{
			fs::path ps = p.path();
			//==========スキンメッシュモデルフォルダの更に中の階層の検索してモデル読み込み============================
			//スキンメッシュモデルフォルダ内のフォルダのパスの生成
			std::string serchPath = path + "/" + ps.filename().string();
			for (auto & q : fs::directory_iterator(serchPath))
			{
				fs::path s_ps = q.path();
				//拡張子のぬきだし
				std::string extensionName = s_ps.extension().string();
				std::string comExtension = ".usb";
				//拡張子がusbだったら（スキンメッシュモデルの場合）
				if (extensionName == comExtension)
				{
					//モデルの格納されているフォルダの名前で登録
					std::string loadName = serchPath + "/";//+ s_ps.filename().string();
					loadName += s_ps.filename().string();
					m_skinnedModels[ps.filename().string()] = new UnityExportSkinnedModel();
					m_skinnedModels[ps.filename().string()]->LoadBinary(loadName);
					m_skinnedModels[ps.filename().string()]->LoadAnimation(serchPath);

					m_skinnedModelNames.emplace_back(ps.filename().string());
					m_currentModelName = ps.filename().string();
				}
			}
			//========================================================================================================
			
		}

		m_skinnedModels[m_currentModelName]->AddObject();
		m_onceInitFg = false;
	}

}



void CSkineMeshRenderer::Draw() {
	//描画可能状態なら
	if (m_active) {
		m_skinnedModels[m_currentModelName]->Draw();
	}
}

void CSkineMeshRenderer::ImGuiDraw()
{
	ImGui::Checkbox(u8"描画", &m_active);
	//スキンメッシュモデルの切り替え用
	auto work_str = ImGuiControl::GetInstance()->SelectDropDown(m_skinnedModelNames, m_currentModelName,"models");
	if (work_str != "null")
	{
		m_currentModelName = work_str;
	}
	//各スキンメッシュモデルのアニメーション情報呼び出し
	m_skinnedModels[m_currentModelName]->ImGuiDraw();

}

