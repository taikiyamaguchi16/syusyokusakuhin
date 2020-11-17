#pragma once
#include	<DirectXMath.h>
#include	"memory.h"
#include	"Shader.h"
#include	"DX11util.h"
#include     "object.h"

namespace Egliss::ComponentSystem
{
	class CLight :public CComponent {
		ALIGN16 struct ConstantBufferLight {
			DirectX::XMFLOAT4 LightDirection;
			DirectX::XMFLOAT4 EyePos;
			DirectX::XMFLOAT4 Ambient;
		};

		enum class LightType {
			DIRECTIONAL,
			POINT,
			SPOT
		};

		CTransform* m_pos = nullptr;
		LightType			m_type;
		DirectX::XMFLOAT3	m_eyepos;
		DirectX::XMFLOAT4	m_lightpos;		// w=0�̎��͕��� w=1�̎��͈ʒu
		ID3D11Buffer*       m_pConstantBufferLight = nullptr;
		DirectX::XMFLOAT4	m_ambient;

		float color_[4] = { 255.0f,255.0f ,255.0f,255.0f };
	public:
		bool Init(DirectX::XMFLOAT3 eyepos, DirectX::XMFLOAT4 lightpos) {
			m_lightpos = lightpos;
			m_eyepos = eyepos;
			m_type = LightType::DIRECTIONAL;

			// �R���X�^���g�o�b�t�@�쐬
			bool sts = CreateConstantBuffer(
				GetDX11Device(),				// �f�o�C�X
				sizeof(ConstantBufferLight),		// �T�C�Y
				&m_pConstantBufferLight);			// �R���X�^���g�o�b�t�@�S
			if (!sts) {
				MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
				return false;
			}

			Update();

			return true;
		}
		inline void Start()override {
			if (m_pos == nullptr)
				m_pos = Holder->GetComponent<CTransform>();

			Init(m_pos->GetDirectPos(), XMFLOAT4(0, 0, 0, 0));
			m_name = "Light";
		}

		inline void Update()override {
			ConstantBufferLight		cb;

			cb.EyePos.w = 1.0;
			cb.EyePos.x = m_eyepos.x;
			cb.EyePos.y = m_eyepos.y;
			cb.EyePos.z = m_eyepos.z;

			cb.LightDirection.x = m_lightpos.x;
			cb.LightDirection.y = m_lightpos.y;
			cb.LightDirection.z = m_lightpos.z;
			cb.LightDirection.w = m_lightpos.w;

			cb.Ambient = m_ambient;

			GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferLight,
				0,
				nullptr,
				&cb,
				0, 0);

			// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
			GetDX11DeviceContext()->VSSetConstantBuffers(4, 1, &m_pConstantBufferLight);
			// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
			GetDX11DeviceContext()->PSSetConstantBuffers(4, 1, &m_pConstantBufferLight);

		}

		inline void SetName()override {
			m_name = "Light";
		}

		inline CComponent* CreateAdress()override {
			return new CLight();
		}

		void Uninit() {
			if (m_pConstantBufferLight) {
				m_pConstantBufferLight->Release();
				m_pConstantBufferLight = nullptr;
			}
		}

		void SetEyePos(DirectX::XMFLOAT3 eyepos) {
			m_eyepos = eyepos;
		}

		void SetLightPos(DirectX::XMFLOAT4 lightpos) {
			m_lightpos = lightpos;
		}

		void SetAmbient(DirectX::XMFLOAT4 amb) {
			m_ambient = amb;
		}

		inline void ImGuiDraw()override {
			ImGui::ColorPicker4(u8"�J���[�s�b�J�[", color_);
			m_ambient = XMFLOAT4(color_[0], color_[1], color_[2], color_[3]);
		}
	};
}