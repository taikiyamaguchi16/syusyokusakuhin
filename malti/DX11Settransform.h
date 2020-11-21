#pragma once
#include	<DirectXMath.h>
#include	"Shader.h"
#include	"Memory.h"

class DX11SetTransform {
	DX11SetTransform() {}
public:
	DX11SetTransform(const DX11SetTransform&) = delete;
	DX11SetTransform& operator=(const DX11SetTransform&) = delete;
	DX11SetTransform(DX11SetTransform&&) = delete;
	DX11SetTransform& operator=(DX11SetTransform&&) = delete;

	static DX11SetTransform* GetInstance() {
		static DX11SetTransform instance;
		return &instance;
	}
	// 定数バッファ定義(ワールド変換行列)
	ALIGN16 struct ConstantBufferWorld
	{
		XMMATRIX World;				// ワールド変換行列
	};
	// 定数バッファ定義(ビュー変換行列)
	ALIGN16 struct ConstantBufferView
	{
		XMMATRIX View;				// ビュー変換行列
	};
	// 定数バッファ定義(毎フレーム設定)
	ALIGN16 struct ConstantBufferProjection
	{
		XMMATRIX Projection;		// プロジェクション変換行列
	};

	enum class TYPE {
		WORLD,
		VIEW,
		PROJECTION
	};

	void Uninit() {
		if (m_pConstantBufferWorld) {
			m_pConstantBufferWorld->Release();
			m_pConstantBufferWorld = nullptr;
		}
		if (m_pConstantBufferView) {
			m_pConstantBufferView->Release();
			m_pConstantBufferView = nullptr;
		}
		if (m_pConstantBufferProjection) {
			m_pConstantBufferProjection->Release();
			m_pConstantBufferProjection = nullptr;
		}
	}

	bool Init();

	void SetTransform(TYPE type, DirectX::XMFLOAT4X4& matrix);
private:
	ID3D11Buffer * m_pConstantBufferWorld = nullptr;		// コンスタントバッファ2
	ID3D11Buffer * m_pConstantBufferView = nullptr;			// コンスタントバッファ2
	ID3D11Buffer * m_pConstantBufferProjection = nullptr;	// コンスタントバッファ2
	ConstantBufferWorld m_CBWorld;				// b0 
	ConstantBufferView m_CBView;				// b1
	ConstantBufferProjection m_CBProjection;	// b2
};