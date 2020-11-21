#pragma once
#include "object.h"
#include "DirectX11Manager.h"
namespace Egliss::ComponentSystem
{
	class CCamera :public CComponent
	{
		wp<CTransform> m_pos;
		wp<CTransform> m_trackingObj;

		bool m_freeCameraFg = true;
		bool m_trackingFg;

		float m_radius;
		float m_elevation = 0.0f;						// �p(����)
		float m_azimuth = 0.0f;						// ���ʊp(����)

		XMFLOAT3 m_eye = { 0,0,-30 };				// ���_
		XMFLOAT3 m_lookat = { 0,0,0 };				// �����_
		XMFLOAT3 m_up = { 0,1,0 };					// ������x�N�g��

		XMFLOAT4X4		m_projection;

		XMFLOAT4X4		m_camera;
		//=====================�R���X�^���g�o�b�t�@�[�X�V�p===============================================
		ConstantBufferMatrix m_constantBuffer;
		ConstantBuffer m_cb;
		//================================================================================================

		float			m_near;
		float			m_aspect;
		float			m_fov;
		float			m_far;

		bool m_mainCameraFg = false;
	public:
		CCamera() {}
		void Start()override;
		void Update()override;
		void Draw()override;
		void ImGuiDraw()override;

		inline void SetMainCameraFg(bool fg_) {
			m_mainCameraFg = fg_;
			m_freeCameraFg = false;
			m_trackingFg = true;
		}

		/*
			CCamera(const CCamera&) = delete;
			CCamera& operator=(const CCamera&) = delete;
			CCamera(CCamera&&) = delete;
			CCamera& operator=(CCamera&&) = delete;

			static CCamera* GetInstance() {
				static CCamera instance;
				return &instance;
			}*/

		inline void Init(float nearclip, float farclip, float fov,
			float width, float height,
			XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up) {

			SetProjection(nearclip, farclip, fov, width / 2.0f, height);
			SetCamera(eye, lookat, up);
		}

		inline void SetNear(float nearclip) {
			m_near = nearclip;
		}

		inline void SetFar(float farclip) {
			m_far = farclip;
		}

		inline void SetFov(float fov) {
			m_fov = fov;
		}

		inline void SetAspect(float width, float height) {
			m_aspect = width / height;
		}

		inline void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
			SetNear(nearclip);
			SetFar(farclip);
			SetFov(fov);
			SetAspect(width, height);
			CreateProjectionMatrix();
		}

		inline void SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up) {

			SetEye(eye);
			SetLookat(lookat);
			SetUp(up);
			CreateCameraMatrix();
		}

		inline void SetEye(const XMFLOAT3& eye) {
			m_eye = eye;
		}

		inline void SetLookat(const XMFLOAT3& lookat) {
			m_lookat = lookat;
		}

		inline void SetUp(const XMFLOAT3& up) {
			m_up = up;
		}

		inline void SetTrackingObj(wp<CTransform> target_) {
			m_trackingObj = target_;
		}

		inline void CreateCameraMatrix() {
			ALIGN16 XMVECTOR Eye = XMVectorSet(m_eye.x, m_eye.y, m_eye.z, 0.0f);
			ALIGN16 XMVECTOR At = XMVectorSet(m_lookat.x, m_lookat.y, m_lookat.z, 0.0f);
			ALIGN16 XMVECTOR Up = XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f);

			ALIGN16 XMMATRIX camera;
			camera = XMMatrixLookAtLH(Eye, At, Up);

			XMStoreFloat4x4(&m_camera, camera);
		}

		void CreateProjectionMatrix() {

			ALIGN16 XMMATRIX projection;

			projection = XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_near, m_far);

			XMStoreFloat4x4(&m_projection, projection);

		}

		inline const XMFLOAT4X4& GetCameraMatrix() {
			return m_camera;
		}

		inline const XMFLOAT4X4& GetProjectionMatrix() {
			return m_projection;
		}

		inline float GetFov() const {
			return m_fov;
		}

		inline const XMFLOAT3& GetEye() const {
			return m_eye;
		}

		inline const XMFLOAT3& GetLookat() const {
			return m_lookat;
		}

		inline const XMFLOAT3& GetUp() const {
			return m_up;
		}

		inline void UpdateSphereCamera(const DirectX::XMFLOAT3 & lookat, float elevation, float azimuth)		// �����_(����)
		{
			DirectX::XMFLOAT3 eye;		// ���_

			// �p�A���ʊp�����W�A���ɕϊ�
			elevation = XMConvertToRadians(elevation);
			azimuth = XMConvertToRadians(azimuth);

			// �p�ɉ��������a���v�Z
			float r = m_radius * sinf(elevation);

			// ���_���W���v�Z(3D�ɍ��W�n���f�J���g���W�n�֕ϊ�)
			eye.x = r * cosf(azimuth);
			eye.y = m_radius * cosf(elevation);
			eye.z = r * sinf(azimuth);

			// ������x�N�g�����v�Z(�p��90�x��]������ƌv�Z�ł���)
			DirectX::XMFLOAT3 up;
			float elevationplus90 = elevation + XMConvertToRadians(90.0f);	// �p��90�x����

			//������x�N�g�����v�Z(3D�ɍ��W�n���f�J���g���W�n�֕ϊ�)
			r = m_radius * sinf(elevationplus90);

			up.x = r * cosf(azimuth);
			up.y = m_radius * cosf(elevationplus90);
			up.z = r * sinf(azimuth);

			//���_�x�N�g���𒍎��_�����s�ړ�
			eye.x += lookat.x;
			eye.y += lookat.y;
			eye.z += lookat.z;

			// �r���[�ϊ��s����쐬
			SetEye(eye);
			SetLookat(lookat);
			SetUp(up);
			CreateCameraMatrix();
		}

	};
}

