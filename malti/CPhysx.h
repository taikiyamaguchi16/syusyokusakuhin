#pragma once
#include "PxPhysicsAPI.h"
#include <vector>

struct FilterGroup
{
	enum Layer
	{
		eDEFAULT	= (1 << 0),
		ePLAYER		= (1 << 1),
		eENEMY		= (1 << 2),
		eFLOOR		= (1 << 3),
		eWALL		= (1 << 4),
	};
};

using namespace physx;

//pairFlagsでコールバックの呼ばれるタイミングを記述
PxFilterFlags TestFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);

class CSimulationEventCallback :public PxSimulationEventCallback {
public:
	void onConstraintBreak(PxConstraintInfo * 	constraints, PxU32 	count)override {}
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair * pairs, PxU32 nbPairs)override;
	//fechresultのタイミングでイベントが発生するので強制的にスリーブにした場合OnSleepになるわけではない
	void onSleep(PxActor ** actors,PxU32 count)override {}
	void onWake(PxActor ** 	actors, PxU32 count)override {}
	void onTrigger(PxTriggerPair* pairs, PxU32 count)override;
	void onAdvance(const PxRigidBody *const * bodyBuffer, const PxTransform * poseBuffer, const PxU32 count)override {}
};

class CSimulationFilterCallback :public PxSimulationFilterCallback {
public:
	PxFilterFlags pairFound(PxU32 pairID,
		PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
		PxPairFlags& pairFlags) {}

	void pairLost(PxU32 pairID,PxFilterObjectAttributes attributes0,PxFilterData filterData0,PxFilterObjectAttributes attributes1,PxFilterData filterData1,bool objectRemoved){}

	bool statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) { return true; }
};

class CPhysx final
{
	CPhysx() {};
	~CPhysx() {};

	static PxDefaultAllocator      m_allocator;
	static PxDefaultErrorCallback  m_errorCallback;
	static PxFoundation*           m_foundation;
	
	static PxDefaultCpuDispatcher* m_dispatcher;
	static PxScene*                m_scene;			//物理空間
	static PxPvd*                  m_pvd;


	static CSimulationEventCallback* m_eventCallbac;
	static inline PxSimulationFilterShader m_filterFlag;
	static inline CSimulationFilterCallback* m_filterCallbac;

	static inline float m_time = 0.f;
public:
	static PxPhysics*              m_physics;		//デバイスのようなもの
	CPhysx(const CPhysx&) = delete;
	CPhysx& operator=(const CPhysx&) = delete;
	CPhysx(CPhysx&&) = delete;
	CPhysx& operator=(CPhysx&&) = delete;

	static void Init();
	static void UnInit();

	inline static PxPhysics* GetPhysics() {
		return m_physics;
	}
	//剛体の生成
	[[nodiscard]]inline static PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* material) {
		//Pxrealは密度
		PxRigidDynamic* dynamic = PxCreateDynamic(*m_physics, t, geometry, *material,1.f);
		return dynamic;
	}
	
	
	[[nodiscard]]inline static PxRigidStatic* createStatic(const PxTransform& t, const PxGeometry& geometry, PxMaterial* material)
	{
		PxRigidStatic* static_actor = PxCreateStatic(*m_physics, t, geometry, *material);
		return static_actor;
	}

	inline static void StepPhysics(float _fps) {
		for (int i = 0; i < 2; i++)
		{
			m_scene->simulate(1.f / _fps);
			m_scene->fetchResults(true);
		}
	/*	m_scene->collide(1.0f / _fps);
		m_scene->fetchCollision();
		m_scene->advance();
		m_scene->fetchResults();*/
	}

	inline static void SetActor(PxRigidDynamic* _actor) {
		_actor->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		m_scene->addActor(*_actor);
	}

	inline static void SetActor(PxRigidStatic* actor_) {
		m_scene->addActor(*actor_);
	}

	inline static PxRigidActor* GetActor() {//最新の追加されたものを取得するのでSetActorとセットで使う
		PxU32 nbActors = m_scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		
		std::vector<PxRigidActor*> actors(nbActors);
		if (nbActors)
		{
			m_scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, (PxActor**)&actors[0], nbActors);
		}
		return actors.back();
	}

	inline static PxRigidActor* GetStaticActor() {//最新の追加されたものを取得するのでSetActorとセットで使う
		PxU32 nbActors = m_scene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);

		std::vector<PxRigidActor*> actors(nbActors);
		if (nbActors)
		{
			m_scene->getActors(PxActorTypeFlag::eRIGID_STATIC, (PxActor**)&actors[0], nbActors);
		}
		return actors.back();
	}

	//[[nodiscard]]static PxBoxController* CreateCapsuleController();

	inline static void DeleteActor(PxRigidActor* _a) { m_scene->removeActor(*_a); }

	static void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);

	static unsigned int GetFilterGroup(unsigned int filter);

};