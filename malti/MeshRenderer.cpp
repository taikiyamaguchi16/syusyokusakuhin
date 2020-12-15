#include "MeshRenderer.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace Egliss::ComponentSystem;
std::map<std::string, UnityExportModel*>CMeshRenderer::m_models;

bool CMeshRenderer::m_onceInitFg = true;

void CMeshRenderer::Start()
{
	m_name = "CMeshRenderer";
	//一度だけ読み込み実行
	m_meshr_render_cnt++;
	if (m_onceInitFg) {
	
		std::string path = "assets/Models";
		for (auto & p : fs::directory_iterator(path))
		{
			fs::path ps = p.path();
			//==========モデルフォルダの更に中の階層の検索してモデル読み込み============================
			//モデルファイル内のフォルダのパスの生成
			std::string serchPath = path + "/" + ps.filename().string();
			for (auto & q : fs::directory_iterator(serchPath))
			{
				fs::path s_ps = q.path();
				//拡張子のぬきだし
				std::string extensionName = s_ps.extension().string();
				std::string comExtension = ".umb";
				//拡張子がusbだったら（モデルの場合）
				if (extensionName == comExtension)
				{
					std::string fileName = s_ps.stem().string();
					//モデルの格納されているフォルダの名前で登録
					std::string loadName = serchPath + "/" + s_ps.filename().string();
					m_models[fileName] = new UnityExportModel();
					m_models[fileName]->LoadBinary(loadName);
					if (ps.filename().string()!= "DefaultShapes")
					{
						m_modelNames.emplace_back(fileName);
						//m_currentModelName = fileName;
					}
				}
			}
			//========================================================================================================
		}

		m_meshtype = GEOMETRYTYPE::BOX;
		m_box = new CBox();
		//m_box->Init(XMFLOAT3(1.f, 1.f, 1.f));

		m_sphere = new CSphere();
		m_meshtype = GEOMETRYTYPE::SPHERE;
		m_sphere->Init();

		m_onceInitFg = false;
	}

	m_currentModelName = m_modelNames[1];
}


void CMeshRenderer::Draw(){
	
	//描画可能状態なら
	if (m_active) {

		if(m_defaultMeshsMode) {
			switch (m_meshtype)
			{
			case GEOMETRYTYPE::BOX:
				//m_box->SetDiffuseMaterial(m_color);
				m_box->Draw();
				//m_models["cube"]->Draw();
				break;
			case GEOMETRYTYPE::CAPSILE:

				break;

			case GEOMETRYTYPE::SPHERE:
				//m_sphere->SetDiffuseMaterial(m_color);
				m_sphere->Draw();
				//m_models["sphere"]->Draw();
				break;
			default:
				break;
			}
		}
		else {
			m_models[m_currentModelName]->Draw();
		}
	}
}

void CMeshRenderer::ImGuiDraw()
{
	ImGui::Checkbox(u8"描画", &m_active);

	ImGui::Checkbox(u8"描画切り替え", &m_defaultMeshsMode);
	if (m_defaultMeshsMode) {
		int keeptype=(int)m_meshtype;
		ImGui::RadioButton(u8"Cube", &keeptype, (int)GEOMETRYTYPE::BOX);
		ImGui::RadioButton(u8"カプセル", &keeptype, (int)GEOMETRYTYPE::CAPSILE);
		ImGui::RadioButton(u8"球", &keeptype, (int)GEOMETRYTYPE::SPHERE);
		m_meshtype = (GEOMETRYTYPE)keeptype;
		//カラーの決定
		//ImGui::ColorPicker4(u8"カラーピッカー", m_color);
	}
	else
	{
		auto work_str = ImGuiControl::GetInstance()->SelectDropDown(m_modelNames, m_currentModelName,"models");
		if (work_str != "null")
		{
			m_currentModelName = work_str;
		}
	}
}

