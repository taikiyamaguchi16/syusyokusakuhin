//#include "MyGui.h"
//#include "DX11util.h"
//
//
//void CMyImGui::Draw()
//{
//	ImGui_ImplDX11_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//
//	/*
//	�E�B���h�E�T�C�Y��ݒ肵�܂��B
//	ImGuiCond_Once �ɂ��A����̂ݐݒ肳��܂��B
//	ImGuiCond_Always �ŁA��ɐݒ肷�邱�Ƃ��ł��܂��B
//	*/
//
//	ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
//
//	bool show_gui = true;
//	bool checkbox;
//	if (ImGui::Begin("MyImGui TitleBar Text", &show_gui))
//	{
//		ImGui::Text(u8"�����͓V�C���ǂ��ł�");
//
//		//��؂��
//		ImGui::Separator();
//
//		ImGui::Text(u8"���̂悤��");
//		ImGui::SameLine();
//		ImGui::Text(u8"�����s�ɃR���e���c��ǉ����邱�Ƃ��ł��܂�");
//
//		ImGui::Separator();
//
//		ImGui::Checkbox(u8"�`�F�b�N�{�b�N�X", &checkbox);
//
//		ImGui::Separator();
//
//		ImGui::ColorPicker4(u8"�J���[�s�b�J�[", color_picker);
//
//		//�t���[�����[�g��\��
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//		ImGui::End();
//	}
//
//	ImGui::Render();
//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//}
