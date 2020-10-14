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
//	ウィンドウサイズを設定します。
//	ImGuiCond_Once により、初回のみ設定されます。
//	ImGuiCond_Always で、常に設定することもできます。
//	*/
//
//	ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
//
//	bool show_gui = true;
//	bool checkbox;
//	if (ImGui::Begin("MyImGui TitleBar Text", &show_gui))
//	{
//		ImGui::Text(u8"今日は天気が良いです");
//
//		//区切り線
//		ImGui::Separator();
//
//		ImGui::Text(u8"このように");
//		ImGui::SameLine();
//		ImGui::Text(u8"同じ行にコンテンツを追加することもできます");
//
//		ImGui::Separator();
//
//		ImGui::Checkbox(u8"チェックボックス", &checkbox);
//
//		ImGui::Separator();
//
//		ImGui::ColorPicker4(u8"カラーピッカー", color_picker);
//
//		//フレームレートを表示
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//		ImGui::End();
//	}
//
//	ImGui::Render();
//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//}
