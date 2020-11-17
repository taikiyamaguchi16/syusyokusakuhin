#pragma once
#include <windows.h>
#include <stdio.h>
#include <Crtdbg.h>
#include <DirectXMath.h>
#include "dx11mathutil.h"
#include <list>
#include "DX11Settransform.h"

#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include  "imGui/ImGuizmo.h" 

#include "dixsmartptr.h"
#include<memory>
#include "PxPhysicsAPI.h"

#include "Reflection/StaticType.hpp"
#include "Reflection/DynamicType.hpp"

#define		SCREEN_X		1920
#define		SCREEN_Y		1080
#define		FULLSCREEN      0
using namespace Dix;

enum class GEOMETRYTYPE {
	BOX = 0,
	SPHERE,
	CAPSILE,
};


// 頂点フォーマット
struct Vertex {
	XMFLOAT3 pos;					// 座標
	XMFLOAT3 normal;				// 法線ベクトル
	XMFLOAT2 tex;					// テクスチャ座標
};

class CObject;
namespace Egliss::ComponentSystem
{
	class CComponent abstract
	{
	protected:

	public:
		std::string m_name = "No name";
		CComponent() {}
		virtual ~CComponent() {}
		CObject *Holder;
		virtual void Start() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void Draw() {}
		virtual void OnCollisionEnter(CObject* col) {}
		virtual void OnCollisionExit(CObject* col) {}
		virtual void OnCollisionStay(CObject* col) {}
		virtual void OnTriggerEnter(CObject* col) {}
		virtual void OnTriggerExit(CObject* col) {}
		virtual void OnTriggerStay(CObject* col) {}

		virtual void ImGuiDraw() {}
		virtual void SetName() {}
		virtual CComponent* CreateAdress() { return nullptr; }
		template<class T>
		inline T* As()
		{
			return static_cast<T*>(this);
		}
	};

	class CRigidbody;

	class CTransform final : public CComponent
	{
		wp<CRigidbody>m_rb;
		XMFLOAT3 m_scale;

		physx::PxTransform* m_trans = new physx::PxTransform();

	public:
		~CTransform() { delete m_trans; }
		XMFLOAT3 m_angle;
		XMFLOAT4X4 m_mat;

		CTransform* m_parent = nullptr;
		std::list<CTransform*>m_child_list;
		inline void Start()override {
			m_angle = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_scale = XMFLOAT3(1.f, 1.f, 1.f);
			DX11MtxTranslation(m_trans->p, m_mat);
			m_name = "Transform";
			m_trans->p = physx::PxVec3(0.0f, 10.0f, 0.0f);
			m_trans->q = physx::PxQuat(physx::PxIdentity);
		}
		//===============================================================================================================
		//アクセサ
		//===============================================================================================================
		inline void SetPos(const XMFLOAT3& p_) { SetPos(physx::PxVec3(p_.x, p_.y, p_.z)); }
		inline void SetPos(const physx::PxVec3& p_) { m_trans->p = p_; }
		inline XMFLOAT3 GetDirectPos() { return XMFLOAT3(m_trans->p.x, m_trans->p.y, m_trans->p.z); }
		inline const physx::PxVec3& GetPhysXPos()const { return m_trans->p; }
		inline void SetAngle(const XMFLOAT3& p_) { SetAngle(physx::PxVec3(p_.x, p_.y, p_.z)); }
		inline void SetAngle(const physx::PxVec3& p_) { m_trans->q.x = p_.x, m_trans->q.y = p_.y, m_trans->q.z = p_.z; }
		inline const XMFLOAT3& GetDirectAngle()const { return XMFLOAT3(m_trans->q.x, m_trans->q.y, m_trans->q.z); }

		inline const physx::PxVec3& GetPhysXAngle()const { return physx::PxVec3(m_trans->q.x, m_trans->q.y, m_trans->q.z); }
		inline physx::PxTransform& GetTrans() { return  *m_trans; }
		inline physx::PxQuat& GetQuat() { return m_trans->q; }
		inline void SetScale(const XMFLOAT3& _s) { m_scale = _s; }
		inline const physx::PxVec3 GetScale()const { return physx::PxVec3(m_scale.x, m_scale.y, m_scale.z); }
		inline const XMFLOAT3& GetDirectScale()const { return m_scale; }
		inline const XMFLOAT3& GetDirectPos()const { return XMFLOAT3(m_trans->p.x, m_trans->p.y, m_trans->p.z); }
		//================================================================================================================
		void Update()override;
		void LateUpdate()override;
		void MoveForward(XMFLOAT3 vec_);
		void Draw()override;
		inline void SetRigidbody(wp<CRigidbody> rb_) {
			m_rb = rb_;
		}

		void Rotation(XMFLOAT4X4& rotateMat_);
		void Rotation(XMFLOAT3 _angle);
		void ImGuiDraw()override;

		void SetParent(CTransform* parent_) {
			m_parent = parent_;
			parent_->SetChild(this);
		}

		void SetChild(CTransform* child_) {
			m_child_list.emplace_back(child_);
			//child_->SetParent(this)
		};

		void SetQuat(XMFLOAT4X4& rotateMat_);
	};

	class CRigidbody final :public CComponent
	{
		wp<CTransform> m_transform;
		physx::PxRigidDynamic*		   m_rigidDynamic = nullptr;
		physx::PxRigidStatic*          m_rigidStatic = nullptr;
		physx::PxRigidActor*		   m_actor = nullptr;
		physx::PxMaterial*			   m_material = nullptr;

		GEOMETRYTYPE m_geometryType = GEOMETRYTYPE::BOX;

		float m_mass = 1.0f;

		bool m_trigger = false;
		bool m_simulation = false;
		//FilterGroup::Enum m_layer = FilterGroup::eDEFAULT;
	public:
		~CRigidbody();
		bool m_usegravity;
		void Start()override;
		void InitDynamic();
		void InitStatic();

		void Update()override;
		void LateUpdate()override;
		void ImGuiDraw()override;
		//==============================================アクセサ===================================================================
		inline void SetName()override {
			m_name = "Rigidbody";
		}
		inline CComponent* CreateAdress()override {
			return new CRigidbody();
		}
		inline physx::PxRigidActor* GetActor() {
			return  m_actor;
		}
		inline void SetMass(float _mass) { m_mass = _mass; }
		inline void SetRigid(physx::PxRigidDynamic* _r) { m_rigidDynamic = _r; }
		inline void SetActor(physx::PxRigidActor* _a) { m_actor = _a; }
		inline void SetGeometryType(GEOMETRYTYPE _type) { m_geometryType = _type; }
		inline physx::PxRigidDynamic* GetRigidDynamic() { return m_rigidDynamic; }
		inline physx::PxRigidStatic* GetRigidStatic() { return m_rigidStatic; }

		inline physx::PxMaterial* GetMaterial()const { return m_material; }
		//============================================アクセサ=======================================================================

		void OnCollisionEnter(CObject* col)override;
		void OnCollisionStay(CObject* col)override;
	};
}


class CObject final
{
	std::string m_tag;
	std::string m_name;
	//オブジェクト毎のFPS
	int m_myFps = 60;
	bool m_cameraObj = false;		//カメラの場合true;
	bool m_life = true;
	//現在のレイヤー
	const char* m_currentItem;
	unsigned int  m_currentLayer;
public:
	std::list<sp<Egliss::ComponentSystem::CComponent>> m_ComponentList;
	CObject() {}
	virtual ~CObject() {
		for (auto itr = m_ComponentList.begin(); itr != m_ComponentList.end(); itr++) {
			(*itr).Clear();
		}
		m_ComponentList.clear();

	}

	void Update();
	void LateUpdate();
	void Draw();
	inline int& GetFps() {
		return m_myFps;
	}

	//オブジェクトが持っているコンポーネントを取得
	template<class T>
	T* GetComponent()
	{
		/*for (auto itr = m_ComponentList.begin(); itr != m_ComponentList.end(); itr++) {
			T* buff = dynamic_cast<T*>(itr->get());
			if (buff != nullptr)
				return buff;
		}*/
		for (auto com : m_ComponentList) {
			T* buff = dynamic_cast<T*>(com.GetPtr());
			if (buff != nullptr)
				return buff;
		}
		return nullptr;
	}

	template<class T>
	wp<T> GetWeakComponent()
	{
		/*for (auto itr = m_ComponentList.begin(); itr != m_ComponentList.end(); itr++) {
			T* buff = dynamic_cast<T*>(itr->get());
			if (buff != nullptr)
				return buff;
		}*/
		for (auto com : m_ComponentList) {
			wp<T>_wp1(com);
			if (_wp1.IsExist() != NULL)
				return _wp1;
		}
		return NULL;
	}
	//オブジェクトが持っているコンポーネントを追加
	template<class T>
	T* AddComponent()
	{
		//std::shared_ptr<T> buff = std::make_shared<T>();
		T* buff = new T();
		buff->Holder = this;
		//m_ComponentList.emplace_back(std::make_shared<T>(buff));
		sp<Egliss::ComponentSystem::CComponent>work_sp;
		work_sp.SetPtr(buff);
		m_ComponentList.emplace_back(work_sp);
		buff->Start();
		return buff;
	}

	template<class T>
	inline T* AddComponent(T* com) {
		com->Holder = this;
		m_ComponentList.emplace_back(com);
		com->Start();
		return com;
	}
	void SetName(std::string s_) {
		m_name = s_;
	}

	inline void _InternalAddComponent(Egliss::ComponentSystem::CComponent* component, int typeId)
	{
		sp<Egliss::ComponentSystem::CComponent>work_sp;
		work_sp.SetPtr(component);
		this->m_ComponentList.emplace_back(work_sp);
		work_sp->Holder = this;
		work_sp->Start();
	}
	//文字列からコンポーネントの生成
	inline Egliss::ComponentSystem::CComponent* AddComponentByName(const std::string& typeName)
	{
		const auto description = Egliss::Reflection::DynamicTypeManager::FindByTypeName(typeName);
		if (description == nullptr)
			return nullptr;
		if (description->isAbstract)
			return nullptr;

		const auto component = static_cast<Egliss::ComponentSystem::CComponent*>(description->constructor());
		this->_InternalAddComponent(component, description->Id());
		return component;
	}
	//文字列からコンポーネントの生成をし生成したアドレスを返す
	template<class T>
	inline T* AddComponentByNameAs(const std::string& typeName)
	{
		return this->AddComponentByName(typeName)->As<T>();
	}

	//template<class T, class U>
	//inline T* _InternalGetComponentFrom(const U& container)
	//{
	//	// 検索する型のIDを拾う
	//	const int inputTypeId = Reflection::StaticTypeDescription<T>::Id;
	//	// コンテナに存在するコンポーネントを列挙
	//	for (const auto component : container)
	//	{
	//		const auto typeID = component->TypeId();
	//		// 型情報を取得
	//		const auto& description = Reflection::DynamicTypeManager::IndexOf(typeID);
	//		// 基底クラスまでの型ID一覧を辿って対象のIDが存在するか
	//		if (description.HasTypeRelation(inputTypeId))
	//			return component->As<T>();
	//	}
	//	return nullptr;
	//}

	inline void SetMyFps(int fps_) {
		m_myFps = fps_;
	}

	void SetTag(std::string s_) {
		m_tag = s_;
	}

	std::string GetName() {
		return m_name;
	}

	std::string GetTag() {
		return m_tag;
	}

	inline bool GetLife() {
		return m_life;
	}
	inline void SetLife(bool _f) { m_life = _f; }

	inline void OnCollisionEnter(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnCollisionEnter(col);
		}
	}
	inline void OnCollisionExit(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnCollisionExit(col);
		}
	}
	inline void OnCollisionStay(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnCollisionStay(col);
		}
	}
	inline void OnTriggerEnter(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnTriggerEnter(col);
		}
	}
	inline void OnTriggerExit(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnTriggerExit(col);
		}
	}
	inline void OnTriggerStay(CObject* col) {
		for (auto&& item : m_ComponentList) {
			item->OnTriggerStay(col);
		}
	}

	inline const char* GetCurrentItem()
	{
		return m_currentItem;
	}

	inline void SetCurrentItem(const char* _work)
	{
		m_currentItem = _work;
	}

	inline unsigned int GetCurrentLayer()
	{
		return m_currentLayer;
	}

	inline void SetCurrentLayer(unsigned int _work)
	{
		m_currentLayer = _work;
	}
};