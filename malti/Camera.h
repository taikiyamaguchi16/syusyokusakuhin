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
		float m_elevation = 0.0f;						// 仰角(入力)
		float m_azimuth = 0.0f;						// 方位角(入力)

		XMFLOAT3 m_eye = { 0,0,-30 };				// 視点
		XMFLOAT3 m_lookat = { 0,0,0 };				// 注視点
		XMFLOAT3 m_up = { 0,1,0 };					// 上向きベクトル

		XMFLOAT4X4		m_projection;

		XMFLOAT4X4		m_camera;
		//=====================コンスタントバッファー更新用===============================================
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

		inline void UpdateSphereCamera(const DirectX::XMFLOAT3 & lookat, float elevation, float azimuth)		// 注視点(入力)
		{
			DirectX::XMFLOAT3 eye;		// 視点

			// 仰角、方位角をラジアンに変換
			elevation = XMConvertToRadians(elevation);
			azimuth = XMConvertToRadians(azimuth);

			// 仰角に応じた半径を計算
			float r = m_radius * sinf(elevation);

			// 視点座標を計算(3D極座標系をデカルト座標系へ変換)
			eye.x = r * cosf(azimuth);
			eye.y = m_radius * cosf(elevation);
			eye.z = r * sinf(azimuth);

			// 上向きベクトルを計算(仰角を90度回転させると計算できる)
			DirectX::XMFLOAT3 up;
			float elevationplus90 = elevation + XMConvertToRadians(90.0f);	// 仰角を90度足す

			//上向きベクトルを計算(3D極座標系をデカルト座標系へ変換)
			r = m_radius * sinf(elevationplus90);

			up.x = r * cosf(azimuth);
			up.y = m_radius * cosf(elevationplus90);
			up.z = r * sinf(azimuth);

			//視点ベクトルを注視点分平行移動
			eye.x += lookat.x;
			eye.y += lookat.y;
			eye.z += lookat.z;

			// ビュー変換行列を作成
			SetEye(eye);
			SetLookat(lookat);
			SetUp(up);
			CreateCameraMatrix();
		}

	};
}

