#include "MeshRenderer.h"
#include <filesystem>

namespace fs = std::filesystem;

using namespace Egliss::ComponentSystem;
std::map<std::string, UnityExportModel*>CMeshRenderer::m_models;

bool CMeshRenderer::m_onceInitFg = true;

void CMeshRenderer::Start()
{
	m_name = "CMeshRenderer";
	//��x�����ǂݍ��ݎ��s
	m_meshr_render_cnt++;
	if (m_onceInitFg) {
	
		std::string path = "assets/Models";
		for (auto & p : fs::directory_iterator(path))
		{
			fs::path ps = p.path();
			//==========���f���t�H���_�̍X�ɒ��̊K�w�̌������ă��f���ǂݍ���============================
			//���f���t�@�C�����̃t�H���_�̃p�X�̐���
			std::string serchPath = path + "/" + ps.filename().string();
			for (auto & q : fs::directory_iterator(serchPath))
			{
				fs::path s_ps = q.path();
				//�g���q�̂ʂ�����
				std::string extensionName = s_ps.extension().string();
				std::string comExtension = ".umb";
				//�g���q��usb��������i���f���̏ꍇ�j
				if (extensionName == comExtension)
				{
					std::string fileName = s_ps.stem().string();
					//���f���̊i�[����Ă���t�H���_�̖��O�œo�^
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
	
	//�`��\��ԂȂ�
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
	ImGui::Checkbox(u8"�`��", &m_active);

	ImGui::Checkbox(u8"�`��؂�ւ�", &m_defaultMeshsMode);
	if (m_defaultMeshsMode) {
		int keeptype=(int)m_meshtype;
		ImGui::RadioButton(u8"Cube", &keeptype, (int)GEOMETRYTYPE::BOX);
		ImGui::RadioButton(u8"�J�v�Z��", &keeptype, (int)GEOMETRYTYPE::CAPSILE);
		ImGui::RadioButton(u8"��", &keeptype, (int)GEOMETRYTYPE::SPHERE);
		m_meshtype = (GEOMETRYTYPE)keeptype;
		//�J���[�̌���
		//ImGui::ColorPicker4(u8"�J���[�s�b�J�[", m_color);
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

