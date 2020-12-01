#include "DynamicType.hpp"
#include "../object.h"
#include "../CCollider.h"
#include "../CSkineMeshRenderer.h"
#include "../MeshRenderer.h"
#include "../Camera.h"

using namespace Egliss::Reflection;
using namespace std::string_literals;

std::vector<DynamicTypeDescription> DynamicTypeManager::_indexedTypes;
std::unordered_map<std::string, int> DynamicTypeManager::_typesindices;


void Egliss::Reflection::DynamicTypeManager::Initialize()
{
	_indexedTypes.emplace_back("Egliss::ComponentSystem::Component", true,std::vector<int>({0}),[](){return nullptr;});
	_typesindices.insert(std::make_pair("Egliss::ComponentSystem::Component",0));
	_indexedTypes.emplace_back("Transform", false,std::vector<int>({1 ,0}),[](){return new Egliss::ComponentSystem::CTransform();});
	_typesindices.insert(std::make_pair("Transform",1));
	_indexedTypes.emplace_back("Rigidbody", false,std::vector<int>({2,1,0}),[](){return new Egliss::ComponentSystem::CRigidbody();});
	_typesindices.insert(std::make_pair("Rigidbody",2));

	_indexedTypes.emplace_back("SphereColider", false, std::vector<int>({ 3,0 }), []() {return new Egliss::ComponentSystem::CSphereCollider(); });
	_typesindices.insert(std::make_pair("SphereColider", 3));
	_indexedTypes.emplace_back("BoxColider", false, std::vector<int>({ 4,0 }), []() {return new Egliss::ComponentSystem::CBoxCollider(); });
	_typesindices.insert(std::make_pair("BoxColider", 4));
	
	_indexedTypes.emplace_back("SkineMeshRenderer", false, std::vector<int>({ 5,0 }), []() {return new Egliss::ComponentSystem::CSkineMeshRenderer(); });
	_typesindices.insert(std::make_pair("SkineMeshRenderer", 5));

	_indexedTypes.emplace_back("MeshRenderer", false, std::vector<int>({ 6,0 }), []() {return new Egliss::ComponentSystem::CMeshRenderer(); });
	_typesindices.insert(std::make_pair("MeshRenderer", 6));

	_indexedTypes.emplace_back("Camera", false, std::vector<int>({ 7,0 }), []() {return new Egliss::ComponentSystem::CCamera(); });
	_typesindices.insert(std::make_pair("Camera", 7));
}
