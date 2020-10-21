#include "ImGuiControl.h"

#include "object.h"
#include <typeinfo.h>
#include "Scene.h"

ImGuiControl::ImGuiControl()
{
	/*
	ImGui::Checkbox(u8"チェックボックス", &checkbox);

	ImGui::Separator();

	ImGui::ColorPicker4(u8"カラーピッカー", color_picker);*/
}


ImGuiControl::~ImGuiControl()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
}


void ImGuiControl::InspectorDraw(wp<CObject> obj_, std::list<CComponent*>& coms_)
{
	//オブジェクトの名前を書くよう
	std::string str = obj_->GetName();
	const char* TitleName = str.c_str();

	/*
	ウィンドウサイズを設定します。
	ImGuiCond_Once により、初回のみ設定されます。
	ImGuiCond_Always で、常に設定することもできます。
	*/
	ImGui::SetNextWindowPos(ImVec2(SCREEN_X-300, 0), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(300, 750), ImGuiCond_None);

	
	ImGui::Begin(TitleName, &show_gui);
	const char* work[] = { "aa","vv","adadsa","asd" };
	DropDown(work, IM_ARRAYSIZE(work));
	//スクロール可能に
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 750), ImGuiWindowFlags_NoTitleBar);
	//区切り線
	ImGui::Separator();
	ImGui::SliderInt(u8"FPS", &obj_->GetFps(), 1, 60);
	ImGui::Separator();
	ImGui::Text(u8"コンポーネントリスト");

	ImGui::Separator();

	//コンポーネント全表示
	for (const auto& com : obj_->m_ComponentList) {
		//コンポーネントの名前を書くように
		TitleName = com->m_name.c_str();

		ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode(TitleName)) {
			com->ImGuiDraw();
			ImGui::TreePop();
		}
	}
	if (ImGui::Button(u8"コンポーネントの追加")) {
		m_AddComponentMode = true;
	}

	//スクロールの終了処理
	ImGui::EndChild();

	ImGui::End();


	if (m_AddComponentMode) {
		ImGui::SetNextWindowPos(ImVec2(300, 0), ImGuiCond_None);
		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_None);
		ImGui::Begin(u8"追加できるコンポーネント");
		//スクロール可能に
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 500), ImGuiWindowFlags_NoTitleBar);


		for (auto itr = coms_.begin(); itr != coms_.end();) {
			TitleName = (*itr)->m_name.c_str();
			if (ImGui::Button(TitleName)) {
				obj_->AddComponent(*itr);

				//同じアドレスを持ったコンポーネントがついてしまうのでアドレス入れなおしている
				auto com_adres = (*itr)->CreateAdress();
				com_adres->SetName();
				coms_.emplace_back(com_adres);
				itr = coms_.erase(itr);
				m_AddComponentMode = false;
			}
			itr++;
		}

		//スクロールの終了処理
		ImGui::EndChild();

		ImGui::End();
	}

}



void ImGuiControl::ConsoleDraw()
{
	ImGui::SetNextWindowPos(ImVec2(0, SCREEN_Y-300), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(SCREEN_X, 300), ImGuiCond_None);

	ImGui::Begin(u8"コンソール", &show_gui);
	//スクロール可能に
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(SCREEN_X, 300), ImGuiWindowFlags_NoTitleBar);

	for (auto item : m_Logs) {
		ImGui::Text(item.c_str());
	}

	//スクロールの終了処理
	ImGui::EndChild();
	ImGui::End();
}


void ImGuiControl::Draw3DGuizm(DirectX::XMFLOAT4X4 _view, DirectX::XMFLOAT4X4 _projection,DirectX::XMFLOAT4X4& _mat,DirectX::XMFLOAT3& _angle)
{
	ImGuizmo::Enable(true);
	float view[16];
	view[0] = _view._11; view[1] = _view._12; view[2] = _view._13; view[3] = _view._14;
	view[4] = _view._21; view[5] = _view._22; view[6] = _view._23; view[7] = _view._24;
	view[8] = _view._31; view[9] = _view._32; view[10] = _view._33; view[11] = _view._34;
	view[12] = _view._41; view[13] = _view._42; view[14] = _view._43; view[15] = _view._44;
	float projection[16];
	projection[0] = _projection._11; projection[1] = _projection._12; projection[2] = _projection._13; projection[3] = _projection._14;
	projection[4] = _projection._21; projection[5] = _projection._22; projection[6] = _projection._23; projection[7] = _projection._24;
	projection[8] = _projection._31; projection[9] = _projection._32; projection[10] = _projection._33; projection[11] = _projection._34;
	projection[12] = _projection._41; projection[13] = _projection._42; projection[14] = _projection._43; projection[15] = _projection._44;

	float mat[16];
	mat[0] = _mat._11; mat[1] = _mat._12; mat[2] = _mat._13; mat[3] = _mat._14;
	mat[4] = _mat._21; mat[5] = _mat._22; mat[6] = _mat._23; mat[7] = _mat._24;
	mat[8] = _mat._31; mat[9] = _mat._32; mat[10] = _mat._33; mat[11] = _mat._34;
	mat[12]= _mat._41; mat[13] = _mat._42;mat[14] = _mat._43; mat[15] = _mat._44;

	ImGuiIO& io = ImGui::GetIO();

	ImGuizmo::SetID(0);
	//描画範囲の設定
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	//矢印などの描画
	ImGuizmo::Manipulate(view, projection, mCurrentGizmoOperation, mCurrentGizmoMode, mat);
	//右上に角度連動したCube描画
	ImGuizmo::ViewManipulate(view, 1.f, ImVec2(io.DisplaySize.x - 300.f, 0), ImVec2(300.f, 300.f), 0x10101010);

	_mat._11 = mat[0]; _mat._12 = mat[1]; _mat._13 = mat[2]; _mat._14 = mat[3];
	_mat._21 = mat[4]; _mat._22 = mat[5]; _mat._23 = mat[6]; _mat._24 = mat[7];
	_mat._31 = mat[8]; _mat._32 = mat[9]; _mat._33 = mat[10]; _mat._34 = mat[11];
	_mat._41 = mat[12]; _mat._42 = mat[13]; _mat._43 = mat[14]; _mat._44 = mat[15];

	float matrixRotation[3], matrixTranslation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(mat, matrixTranslation, matrixRotation, matrixScale);
	_angle.x = matrixRotation[0];
	_angle.y = matrixRotation[1];
	_angle.z = matrixRotation[2];
}

void ImGuiControl::Select3DGuizm()
{
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
		mCurrentGizmoMode = ImGuizmo::LOCAL;
	if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
		mCurrentGizmoMode = ImGuizmo::WORLD;
}

void ImGuiControl::DropDown(const char* _str[],int _size)
{
	const char** items = _str;

	static const char* current_item = NULL;
	ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();
	ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < _size; n++)
		{
			bool is_selected = (current_item == *(items + n));
			if (ImGui::Selectable(*(items+n), is_selected))
				current_item = *(items + n);
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Left))
	{
	}
	ImGui::SameLine(0, spacing);
	if (ImGui::ArrowButton("##r", ImGuiDir_Right))
	{
	}
	ImGui::SameLine(0, style.ItemInnerSpacing.x);
	ImGui::Text("Custom Combo");
}

void ImGuiControl::BeforeRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
}

void ImGuiControl::AfterRebder()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

