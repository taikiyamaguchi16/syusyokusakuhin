#include "UnityExportSkinnedModel.h"

UnityExportSkinnedModel::UnityExportSkinnedModel()
{
	vs.Attach(DirectX11Manager::CreateVertexShader("Assets/Shaders/UnityExportSkinnedModel.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("Assets/Shaders/UnityExportSkinnedModel.hlsl", "psMain"));

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"		,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD"	,	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "BONEINDEX"	,	0,	DXGI_FORMAT_R32G32B32A32_UINT,	0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "BONEWEIGHT"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	48,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 5, "Assets/Shaders/UnityExportSkinnedModel.hlsl", "vsMain"));

	DirectX11Manager::CreateConstantBuffer(sizeof(XMMATRIX) * 200, &boneMtxCb);

	//コンスタントバッファの作成
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	constantBuffer2.proj = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
			SCREEN_X / SCREEN_X, 0.5f, 4096.0f * 8.0f));
}

void UnityExportSkinnedModel::LoadAscii(string filename)
{
	uemData.LoadAscii(filename);

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

void UnityExportSkinnedModel::Draw(XMFLOAT4X4 _mat)
{
	DirectX11Manager::SetVertexShader(vs.Get());
	DirectX11Manager::SetPixelShader(ps.Get());

	DirectX11Manager::SetInputLayout(il.Get());

	constantBuffer2.world = XMMatrixTranspose(XMLoadFloat4x4(&_mat));
	DirectX11Manager::UpdateConstantBuffer(cb.Get(), constantBuffer2);
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
