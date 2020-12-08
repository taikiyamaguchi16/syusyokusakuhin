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
	//��x�����ǂݍ��ݎ��s
	if (m_onceInitFg) {

		std::string path = "assets/SkineMeshModels";
		for (auto & p : fs::directory_iterator(path))
		{
			fs::path ps = p.path();
			//==========�X�L�����b�V�����f���t�H���_�̍X�ɒ��̊K�w�̌������ă��f���ǂݍ���============================
			//�X�L�����b�V�����f���t�H���_���̃t�H���_�̃p�X�̐���
			std::string serchPath = path + "/" + ps.filename().string();
			for (auto & q : fs::directory_iterator(serchPath))
			{
				fs::path s_ps = q.path();
				//�g���q�̂ʂ�����
				std::string extensionName = s_ps.extension().string();
				std::string comExtension = ".usb";
				//�g���q��usb��������i�X�L�����b�V�����f���̏ꍇ�j
				if (extensionName == comExtension)
				{
					//���f���̊i�[����Ă���t�H���_�̖��O�œo�^
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
	//�`��\��ԂȂ�
	if (m_active) {
		m_skinnedModels[m_currentModelName]->Draw();
	}
}

void CSkineMeshRenderer::ImGuiDraw()
{
	ImGui::Checkbox(u8"�`��", &m_active);
	//�X�L�����b�V�����f���̐؂�ւ��p
	auto work_str = ImGuiControl::GetInstance()->SelectDropDown(m_skinnedModelNames, m_currentModelName,"models");
	if (work_str != "null")
	{
		m_currentModelName = work_str;
	}
	//�e�X�L�����b�V�����f���̃A�j���[�V�������Ăяo��
	m_skinnedModels[m_currentModelName]->ImGuiDraw();

}

