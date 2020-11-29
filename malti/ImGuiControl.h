#pragma once
#include <iostream>

#include <vector>
#include <list>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include  "imGui/ImGuizmo.h" 
#include <sstream>
#include <DirectXMath.h>

#include "object.h"
#include "dixsmartptr.h"

using namespace Dix;

class CObject;
class CGame;
class CComponent;

class ImGuiControl
{
	bool m_CreateMode = false;
	bool m_AddComponentMode = false;
	std::list<std::string> m_Logs;
	ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::LOCAL;
public:
	//trueならデバッグ用のGUI表示
	bool m_debugMode = false;
	ImGuiControl(const ImGuiControl&) = delete;
	ImGuiControl& operator=(const ImGuiControl&) = delete;
	ImGuiControl(ImGuiControl&&) = delete;
	ImGuiControl& operator=(ImGuiControl&&) = delete;

	static ImGuiControl* GetInstance() {
		static ImGuiControl instance;
		return &instance;
	}

	float color_picker[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	bool show_gui = false;
	bool checkbox;
	float m_x=0.0f;

	ImGuiControl();
	~ImGuiControl();
	

	template<typename T>
	inline void HierarchyDraw(T* scene_) {
		//=======================================ヒエラルキーの描画=========================================================
		//std::list<wp<CObject>>obj_s = scene_->GetObjList();
		std::list<wp<CObject>>obj_s;
		for (auto item : scene_->GetObjList()) {
			wp<CObject>_ob(item);
			obj_s.emplace_back(_ob);
		}

		//std::vector<CObject*>camera_s = scene_->GetCameraObjects();
		std::vector<wp<CObject>>camera_s;
		for (auto item : scene_->GetCameraObjects()) {
			wp<CObject>_ob(item);
			camera_s.emplace_back(_ob);
		}
		ImGui::SetNextWindowPos(ImVec2(0, 150), ImGuiCond_None);
		ImGui::SetNextWindowSize(ImVec2(280, 600), ImGuiCond_None);

		ImGui::Begin("Hierarchy");

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 600), ImGuiWindowFlags_NoTitleBar);
		

		if (ImGui::Button(u8"新規作成")) {
			m_CreateMode = true;
			//items.push_back(0.0f);
		}
		std::ostringstream oss;


		//現状のオブジェクトの表示
		unsigned int label = 0;
		for (auto com : obj_s)
		{
			//同名が認識されないのでラベルを付与
			label++;
			oss << "##"<<label;
			std::string str = (*com).GetName() + oss.str();
			const char* TitleName = str.c_str();
			if (ImGui::Button(TitleName)) {
				//クリックされたオブジェクトのアドレスを代入
				scene_->SetActiveObj(com);	//一時しのぎ変える！！！！！！！！！！！！！！！！！！！
			}
		}
		for (auto com : camera_s) {
			//同名が認識されないのでラベルを付与
			label++;
			oss << "##" << label;
			std::string str = (*com).GetName() + oss.str();
			const char* TitleName = str.c_str();
			if (ImGui::Button(TitleName)) {
				//クリックされたオブジェクトのアドレスを代入
				scene_->SetActiveObj(com);
			}
		}
		ImGui::EndChild();

		ImGui::End();

		//オブジェクトの新規作成=======================================================
		if (m_CreateMode) {
			ImGui::SetNextWindowPos(ImVec2(300, 0), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
			ImGui::Begin(u8"作成画面");

			static char str0[128];
			ImGui::InputText(u8"オブジェクトの名前", str0, IM_ARRAYSIZE(str0));

			if (ImGui::Button(u8"作成終了")) {
				std::string s = str0;
				scene_->CreateObject(s);
				DebugLog(u8"作成完了");
				m_CreateMode = false;
			}

			ImGui::End();
		}
		//==============================================================================


		//==============================================================================================================================


		//=================================================================================FPS関係==========================================
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_None);
		ImGui::SetNextWindowSize(ImVec2(320, 150), ImGuiCond_None);
		ImGui::Begin(u8"FPS表示");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::SliderFloat(u8"FPS表示", &(SceneManager::GetInstance()->m_fps), 1.0f, 144.0f);

		ImGui::Checkbox(u8"コマ送りボタン", &(SceneManager::GetInstance()->m_frameAdvanceFg));

		if (SceneManager::GetInstance()->m_frameAdvanceFg) {
			if (ImGui::Button(u8"コマ送り")) {
				SceneManager::GetInstance()->Update();
				SceneManager::GetInstance()->m_fpsCount++;
			}
		}

		ImGui::End();
		//======================================================================================================================================
	}

	void InspectorDraw(wp<CObject> obj_, std::list<Egliss::ComponentSystem::CComponent*> coms_);

	void ConsoleDraw();

	void DebugLog(std::string s_) {
		m_Logs.push_back(s_);
	}

	void Draw3DGuizm(DirectX::XMFLOAT4X4 _view, DirectX::XMFLOAT4X4 _projection,DirectX::XMFLOAT4X4& _mat,DirectX::XMFLOAT3& _angle);
	void Select3DGuizm();
	std::string SelectDropDown(const char* _str[],int _size);
	std::string SelectDropDown(std::vector<std::string>_str, std::string _current_str, std::string _title);
	void DropDown(const char* _str[], int _size, wp<CObject> _obj);

	void BeforeRender();
	void AfterRebder();
};

