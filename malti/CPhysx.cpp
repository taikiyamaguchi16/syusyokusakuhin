#include "CPhysx.h"
#include "UserData.h"

PxDefaultAllocator CPhysx::m_allocator;
PxDefaultErrorCallback CPhysx::m_errorCallback;
PxFoundation* CPhysx::m_foundation = nullptr;
PxPhysics* CPhysx::m_physics = nullptr;
PxDefaultCpuDispatcher* CPhysx::m_dispatcher = nullptr;
PxScene* CPhysx::m_scene = nullptr;
PxPvd* CPhysx::m_pvd = nullptr;
PxControllerManager* CPhysx::m_controllerManager = nullptr;
CSimulationEventCallback* CPhysx::m_eventCallbac;
//PxSimulationFilterShader CPhysx::m_filterFlag;
//CSimulationFilterCallback* CPhysx::m_filterCallbac;

void CPhysx::Init()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	// PVD(PhysX Visual Debugger) setting. To use PVD, we need to build the project as Debug mode.
	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);

	// Scene setting
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);          // Right-hand coordinate system, Y-UP.
	m_dispatcher = PxDefaultCpuDispatcherCreate(1);         // The number of worker threads is one.
	sceneDesc.cpuDispatcher = m_dispatcher;

	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = TestFilterShader;
	sceneDesc.filterCallback= m_filterCallbac;

	m_scene = m_physics->createScene(sceneDesc);
	m_eventCallbac = new CSimulationEventCallback();
	m_scene->setSimulationEventCallback(m_eventCallbac);
	m_controllerManager = PxCreateControllerManager(*m_scene);


	// PVD setting
	PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	//コールバック関数を呼ぶタイミング
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void CPhysx::UnInit()
{
	m_scene->release();
	m_scene = nullptr;

	m_physics->release();
	m_physics = nullptr;

	m_controllerManager->release();
	//m_controllerManager = nullptr;

	

	m_foundation->release();
	m_foundation = nullptr;
	

	/*m_pvd->release();
	m_pvd = nullptr;*/

//	delete m_filterCallbac;
	delete m_eventCallbac;
}

PxBoxController * CPhysx::CreateCapsuleController()
{
	PxBoxControllerDesc desc;
	desc.slopeLimit = 2.0f;
	desc.stepOffset = 2.0f;
	desc.position.x = 0.0f;
	desc.position.y = 100.0f;
	desc.position.z = 0.0f;
	desc.material= GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);

	desc.halfHeight = 1.5f;
	desc.halfSideExtent = 1.5f;
	desc.halfForwardExtent = 1.5f;
	desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);

	PxController* c = m_controllerManager->createController(desc);
	
	PxBoxController* as = (PxBoxController*)c;

	return as;
}

PxFilterFlags TestFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags & pairFlags, const void* constantBlock,PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		//pairFlagsによって呼び出されるタイミングが変わる
		pairFlags = PxPairFlag::eNOTIFY_TOUCH_LOST|PxPairFlag::eNOTIFY_TOUCH_FOUND| PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return PxFilterFlag::eCALLBACK;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eCALLBACK;
}

unsigned int GetFilterGroup(unsigned int filter)
{
	//ここに記述しておくことで当たり判定をとりたくないレイヤーがreturnで帰ってくるので
	//setupFilteringの際に間違えない
	unsigned int mask;
	switch (filter)
	{
	case FilterGroup::eDEFAULT:
		break;
	case FilterGroup::ePLAYER:
		mask = FilterGroup::eFLOOR;
		break;
	case FilterGroup::eENEMY:
		break;
	case FilterGroup::eFLOOR:
		break;
	case FilterGroup::eWALL:
		break;
	default:
		mask = 0;
		break;
	}
	return mask;
}


//filterGroupに自身のレイヤー	filterMaskに当たり判定をとりたくないレイヤーを記述
void setupFiltering(PxRigidActor * actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	free(shapes);
}

void CSimulationEventCallback::onTrigger(PxTriggerPair * pairs, PxU32 count)
{
	//自分で設定したトリガーのフラグをtrueにする
	USERDATA* user = (USERDATA*)pairs->triggerActor->userData;
	USERDATA* otherUser = (USERDATA*)pairs->otherActor->userData;
	//pairs->triggerActor->userData = user;

	if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		user->obj->OnTriggerEnter(otherUser->obj);
	}
	else if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_LOST) {
		user->obj->OnTriggerExit(otherUser->obj);
	}
	else if (pairs->status&PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
		user->obj->OnTriggerStay(otherUser->obj);
	}
	user->Status = NULL;
}
