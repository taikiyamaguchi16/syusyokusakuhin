#include "UnityExportModel.h"

UnityExportModel::UnityExportModel()
{
	vs.Attach(DirectX11Manager::CreateVertexShader("assets/Shaders/UnityExportModel.hlsl", "vsMain"));
	ps.Attach(DirectX11Manager::CreatePixelShader("assets/Shaders/UnityExportModel.hlsl", "psMain"));

	//InputLayoutの作成
	D3D11_INPUT_ELEMENT_DESC elem[] = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "NORMAL"	,	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "COLOR"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	32,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
	};
	il.Attach(DirectX11Manager::CreateInputLayout(elem, 4, "assets/Shaders/UnityExportModel.hlsl", "vsMain"));

	//コンスタントバッファの作成
	DirectX11Manager::CreateConstantBuffer(sizeof(ConstantBufferMatrix), &cb);
	DirectX11Manager::m_constantBuffer.proj = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
			SCREEN_X / SCREEN_X, 0.5f, 4096.0f * 8.0f));
}

UnityExportModel::~UnityExportModel()
{
	cb.Reset();
}


void UnityExportModel::LoadBinary(string filename)
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

void UnityExportModel::Draw()
{

	DirectX11Manager::SetVertexShader(vs.Get());
	DirectX11Manager::SetPixelShader(ps.Get());
	DirectX11Manager::SetInputLayout(il.Get());	

	//DirectX11Manager::m_constantBuffer.world = XMMatrixTranspose(XMLoadFloat4x4(&_mat));

	DirectX11Manager::UpdateConstantBuffer(cb.Get(), DirectX11Manager::m_constantBuffer);
	ID3D11Buffer* tmpCb[] = { cb.Get() };
	DirectX11Manager::m_pImContext->VSSetConstantBuffers(0, 1, tmpCb);
	
	for (int i = 0; i < uemData.meshs.size(); i++) {
		
		auto& model = uemData.meshs[i];
		DirectX11Manager::SetVertexBuffer(models[i].vb.Get(), sizeof(VertexData));
		DirectX11Manager::SetIndexBuffer(models[i].ib.Get());
		DirectX11Manager::SetTexture2D(0, materials[model.materialNo].albedoTexture.Get());

		//DrawCall
		DirectX11Manager::DrawIndexed(static_cast<UINT>(model.indexs.size()));
	}

}
