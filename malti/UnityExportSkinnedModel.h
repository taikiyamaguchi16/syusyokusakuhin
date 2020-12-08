#pragma once
#include "DirectX11Manager.h"
#include <map>

class UnityExportSkinnedModel
{
	InputLayout il;
	VertexShader vs;
	PixelShader ps;

	ConstantBuffer boneMtxCb;
	XMMATRIX boneMtx[200];

	ConstantBuffer cb;

	std::string m_currentAnimationName;
	std::map<string,uem::SkinnedAnimation*> m_animation;
	std::vector<std::string>m_AnimationNames;
	float m_animTime = 0.f;
	float m_loopSplit = 60.f;
public:
	
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
		XMFLOAT2 uv = XMFLOAT2(0, 0);
		XMUINT4 boneIndex = XMUINT4(0, 0, 0, 0);
		XMFLOAT4 boneWeight = XMFLOAT4(0, 0, 0, 0);
	};

	struct Material
	{
		ShaderTexture albedoTexture;
	};

	struct ModelData
	{
		VertexBuffer vb;
		IndexBuffer ib;
	};

	uem::SkinnedModel<VertexData> uemData;

	vector<ModelData> models;
	vector<Material> materials;

	UnityExportSkinnedModel();
	~UnityExportSkinnedModel();

	void LoadBinary(string filename);

	void LoadAnimation(string _pathname);

	inline float GetMaxAnimTime()
	{
		return m_animation[m_currentAnimationName]->GetMaxAnimationTime();
	}

	inline void SetAnimTime(float _time)
	{
		m_animTime = _time;
	}

	void ImGuiDraw();
	void Draw();

	void AddObject();

};