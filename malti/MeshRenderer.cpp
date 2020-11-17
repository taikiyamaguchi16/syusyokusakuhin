#include "MeshRenderer.h"
#include "CDirectInput.h"

using namespace Egliss::ComponentSystem;

std::map<std::string, CModel*> CMeshRenderer::m_models;
bool CMeshRenderer::m_onceInitFg = true;

void CMeshRenderer::Start()
{
	m_name = "CMeshRenderer";
	//m_model.Init("assets/X-Wing.x.dat", "shader/vs.fx", "shader/ps.fx");
	//一度だけ読み込み実行
	m_meshr_render_cnt++;
	if (m_onceInitFg) {
		//===============================================================================
		//filesystemを使ってassetファイル内のモデル取るとかにするといいかも
		//===============================================================================
		////今だけ
		//m_models["X-Wing"].Init("assets/X-Wing.x.dat", "shader/vs.fx", "shader/ps.fx");
		//m_models["TIE_Fighter"] = new CModel();

		//m_models["TIE_Fighter"]->Init("assets/TIE_Fighter.x.dat", "shader/vs.fx", "shader/ps.fx");
	
		//m_models["skydome"].Init("assets/skydome.x.dat", "shader/vs.fx", "shader/ps.fx");

		m_meshtype = GEOMETRYTYPE::BOX;
		m_box = new CBox();
		m_box->Init(XMFLOAT3(1.f, 1.f, 1.f));

		m_sphere = new CSphere();
		m_meshtype = GEOMETRYTYPE::SPHERE;
		m_sphere->Init(1.f);

		m_onceInitFg = false;
	}
	//m_sphere.Init(m_radius);

	m_modelName = "X-Wing";
}


void CMeshRenderer::LoadModel(const char * filename, const char * vsfile, const char * psfile)
{
	//m_model->Init(filename, vsfile, psfile);
}


void CMeshRenderer::Draw(){
	if (!m_pos.IsExist())
		m_pos = Holder->GetWeakComponent<CTransform>();
	//描画可能状態なら
	if (m_active) {

		if(m_defaultMeshsMode) {
			switch (m_meshtype)
			{
			case GEOMETRYTYPE::BOX:
				m_box->SetDiffuseMaterial(m_color);
				m_box->Draw();
				break;
			case GEOMETRYTYPE::CAPSILE:

				break;

			case GEOMETRYTYPE::SPHERE:
				m_sphere->SetDiffuseMaterial(m_color);
				m_sphere->Draw();
				break;
			default:
				break;
			}
		}
		else {
			m_models[m_modelName]->Draw();
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
		ImGui::ColorPicker4(u8"カラーピッカー", m_color);
	}
}

void CMeshRenderer::SetModel(std::string s_)
{
	m_modelName = s_;
}
