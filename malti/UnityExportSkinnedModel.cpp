#include "UnityExportSkinnedModel.h"
#include <filesystem>

namespace fs = std::filesystem;

UnityExportSkinnedModel::UnityExportSkinnedModel()
{
	vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/UnityExportSkinnedModel.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/UnityExportSkinnedModel.hlsl", "psMain"));

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"		,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD"	,	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "BONEINDEX"	,	0,	DXGI_FORMAT_R32G32B32A32_UINT,	0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "BONEWEIGHT"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	48,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 5, "assets/Shaders/UnityExportSkinnedModel.hlsl", "vsMain"));

	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	/*DirectX11Manager::m_constantBuffer.proj = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
			SCREEN_X / SCREEN_X, 0.5f, 4096.0f * 8.0f));*/

	DirectX11Manager::CreateConstantBuffer(sizeof(XMMATRIX) * 200, &boneMtxCb);

}

UnityExportSkinnedModel::~UnityExportSkinnedModel()
{
	/*cb->Release();
	boneMtxCb->Release();
	il->Release();
	vs->Release();
	ps->Release();

	for (auto itr : models)
	{
		itr.ib->Release();
		itr.vb->Release();
	}
	for (auto itr : materials)
	{
		itr.albedoTexture->Release();
	}*/
}

void UnityExportSkinnedModel::LoadBinary(string filename)
{
	uemData.LoadBinary(filename);

	//VertexBuffer IndexBuffer作成
	for (auto& mesh : uemData.meshs)
	{
		ModelData tmpData;
		tmpData.vb.Attach(DirectX11Manager::CreateVertexBuffer(mesh.vertexDatas.data(), (UINT)mesh.vertexDatas.size()));
		tmpData.ib.Attach(DirectX11Manager::CreateIndexBuffer(mesh.indexs.data(), (UINT)mesh.indexs.size()));
		models.push_back(tmpData);
	}

	//TextureLoad
	for (auto& material : uemData.materials)
	{
		Material tmpMaterial;
		tmpMaterial.albedoTexture.Attach(DirectX11Manager::CreateTextureFromFile(material.GetTexture("_MainTex")));
		materials.push_back(tmpMaterial);
	}
}

void UnityExportSkinnedModel::LoadAnimation(string _pathname)
{
	for (auto & q : fs::directory_iterator(_pathname))
	{
		fs::path s_ps = q.path();
		//拡張子のぬきだし
		std::string extensionName = s_ps.extension().string();
		std::string comExtension = ".usab";
		//拡張子がusbだったら（スキンメッシュモデルの場合）
		if (extensionName == comExtension)
		{
			//モデルの格納されているフォルダの名前で登録
			std::string loadName = _pathname + "/" + s_ps.filename().string();
			m_animation[s_ps.stem().string()] = new uem::SkinnedAnimation();
			m_animation[s_ps.stem().string()]->LoadBinary(loadName, (*this).uemData.root.get());
			m_AnimationNames.emplace_back(s_ps.stem().string());
			m_currentAnimationName = s_ps.stem().string();
		}
	}
}

void UnityExportSkinnedModel::ImGuiDraw()
{
	auto work_str = ImGuiControl::GetInstance()->SelectDropDown(m_AnimationNames, m_currentAnimationName,"animations");
	if (work_str != "null")
	{
		m_currentAnimationName = work_str;
	}
	ImGui::SliderFloat("AnimTime", &m_loopSplit, 0.0f, 200.f);
	
	DrawChild(uemData.root.get());
	
}

void UnityExportSkinnedModel::DrawChild(uem::Transform* _trans)
{
	if (_trans->child.size() > 0)
	{
		for (auto itr = _trans->child.begin(); itr != _trans->child.end(); itr++)
		{
			if (ImGui::TreeNode((*itr)->name.c_str()))
			{
				auto mat = _trans->LocalToWorldMatrix();
				ImGui::Text("x %.3f  y%.3f  z%.3f", mat.r[3].m128_f32[0], mat.r[3].m128_f32[1], mat.r[3].m128_f32[2]);
				DrawChild(*itr);
				ImGui::TreePop();
			}
		}

	}
}

void UnityExportSkinnedModel::Draw()
{
	m_animTime += m_animation[m_currentAnimationName]->GetMaxAnimationTime() / m_loopSplit;
	if (m_animTime > m_animation[m_currentAnimationName]->GetMaxAnimationTime())
		m_animTime = 0.f;
	

	m_animation[m_currentAnimationName]->SetTransform(m_animTime);
	DirectX11Manager::SetVertexShader(vs.Get());
	DirectX11Manager::SetPixelShader(ps.Get());

	DirectX11Manager::SetInputLayout(il.Get());

	//DirectX11Manager::m_constantBuffer.world = XMMatrixTranspose(XMLoadFloat4x4(&_mat));
	DirectX11Manager::UpdateConstantBuffer(cb.Get(), DirectX11Manager::m_constantBuffer);
	ID3D11Buffer* tmpCb2[] = { cb.Get() };
	DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, tmpCb2);

	for(int j=0;j<uemData.meshs.size();j++){
		auto& model = uemData.meshs[j];
		//ボーン行列を作る
		for (int i = 0; i < model.bones.size(); i++)
		{
			auto mat = model.bones[i].second->LocalToWorldMatrix();
			boneMtx[i] = XMMatrixTranspose(model.bones[i].first * mat);
		}

		DirectX11Manager::UpdateConstantBuffer(boneMtxCb.Get(), boneMtx);
		ID3D11Buffer* tmpCb[] = { boneMtxCb.Get() };
		DirectX11Manager::m_pImContext->VSSetConstantBuffers(1, 1, tmpCb);

		DirectX11Manager::SetVertexBuffer(models[j].vb.Get(), sizeof(VertexData));
		DirectX11Manager::SetIndexBuffer(models[j].ib.Get());
		if (materials[model.materialNo].albedoTexture.Get() != nullptr)
			DirectX11Manager::SetTexture2D(0, materials[model.materialNo].albedoTexture.Get());

		//DrawCall
		DirectX11Manager::DrawIndexed(static_cast<UINT>(model.indexs.size()));
	}
}
