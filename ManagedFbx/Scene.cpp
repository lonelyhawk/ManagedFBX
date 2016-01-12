#include "stdafx.h"
#include "Scene.h"
#include "Manager.h"

using namespace ManagedFbx;

Scene::Scene(string ^name)
{
	m_nativeScene = FbxScene::Create(Manager::GetInstance(), StringHelper::ToNative(name));
	m_rootNode = gcnew SceneNode(m_nativeScene->GetRootNode());
}

Scene ^Scene::Import(string ^filename)
{
	auto importer = Manager::GetImporter();

	if(!importer->Initialize(StringHelper::ToNative(filename)))
		throw gcnew FbxException("Failed to initialise the FBX importer: {0}", gcnew string("error"));

	auto scene = gcnew Scene("");

	if(!importer->Import(scene->m_nativeScene))
		throw gcnew FbxException("Failed to import the scene: {0}", gcnew string("error"));

	// Needs refreshing
	scene->m_rootNode = gcnew SceneNode(scene->m_nativeScene->GetRootNode());

	return scene;

}


void Scene::Save(string ^filename)
{
	auto exporter = Manager::GetExporter();

	if(!exporter->Initialize(StringHelper::ToNative(filename)))
		throw gcnew FbxException("Failed to initialise the FBX exporter: {0}", gcnew string("error"));

	if(!exporter->Export(m_nativeScene))
		throw gcnew FbxException("Failed to export the scene: {0}", gcnew string("error"));
}

void Scene::Name::set(string ^value)
{
	m_nativeScene->SetName(StringHelper::ToNative(value));
}

string ^Scene::Name::get()
{
	return gcnew string(m_nativeScene->GetName());
}

SceneNode ^Scene::RootNode::get()
{
	return m_rootNode;
}

void Scene::Application::set(string ^value)
{
	m_nativeScene->GetSceneInfo()->LastSaved_ApplicationName.Set(StringHelper::ToNative(value));
}

string ^Scene::Application::get()
{
	auto name = m_nativeScene->GetSceneInfo()->LastSaved_ApplicationName.Get().Buffer();
	return gcnew string(name);
}

string ^Scene::UnitType::get()
{
	return gcnew string(m_nativeScene->GetGlobalSettings().GetSystemUnit().GetScaleFactorAsString_Plurial());
}

double Scene::UnitScale::get()
{
	return m_nativeScene->GetGlobalSettings().GetSystemUnit().GetScaleFactor();
}

void Scene::UnitScale::set(double scaleFactor)
{
	this->SetSceneScale(scaleFactor);
}

double Scene::UnitMultipler::get()
{
	return m_nativeScene->GetGlobalSettings().GetSystemUnit().GetMultiplier();
}

void Scene::UnitMultipler::set(double multiplier)
{
	double scaleFactor = this->UnitScale;
	m_nativeScene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit(scaleFactor, multiplier));
}

void Scene::ConvertUnits(Unit units)
{
	switch(units)
	{
	case Unit::Metres:
		FbxSystemUnit::m.ConvertScene(m_nativeScene);
		break;

	case Unit::Centimetres:
		FbxSystemUnit::cm.ConvertScene(m_nativeScene);
		break;

	default:
		throw gcnew NotImplementedException("The supplied unit type is not currently supported for conversion.");
	}
}

void Scene::ConvertAxes(AxisSystem axis)
{
	switch(axis)
	{
	case AxisSystem::Max:
		FbxAxisSystem::Max.ConvertScene(m_nativeScene);
		break;

	case AxisSystem::MayaYUp:
		FbxAxisSystem::MayaYUp.ConvertScene(m_nativeScene);
		break;

	case AxisSystem::MayaZUp:
		FbxAxisSystem::MayaZUp.ConvertScene(m_nativeScene);
		break;

	default:
		throw gcnew NotImplementedException("The supplied axis type is not currently supported for conversion.");
	}
}

void Scene::BakeTransform(SceneNode ^node)
{
	for each(SceneNode ^node in node->ChildNodes)
		BakeTransform(node);

	auto native = node->m_nativeNode;
	auto mesh = native->GetMesh();

	if(!mesh)
		return;

	FbxAMatrix geometry(native->GetGeometricTranslation(FbxNode::eSourcePivot),
		native->GetGeometricRotation(FbxNode::eSourcePivot),
		native->GetGeometricScaling(FbxNode::eSourcePivot));

	auto total = m_nativeScene->GetAnimationEvaluator()->GetNodeGlobalTransform(native) * geometry;

	for(int i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		auto pos = mesh->GetControlPointAt(i);
		mesh->SetControlPointAt(total.MultT(pos), i);
	}
}

SceneNode^ Scene::CreateNode(string^ name)
{
	marshal_context context;
	const char* lNativeName = context.marshal_as<const char*>(name);

	FbxNode* lNativeNode = FbxNode::Create(m_nativeScene, lNativeName);
	SceneNode^ result = gcnew SceneNode(lNativeNode);
	return result;
}

Mesh^ Scene::CreateMesh(string^ name){
	marshal_context context;
	const char* lNativeName = context.marshal_as<const char*>(name);

	FbxMesh* lNativeMesh = FbxMesh::Create(m_nativeScene, lNativeName);
	Mesh^ result = gcnew Mesh(lNativeMesh);
	return result;
}

void Scene::SetSceneScale(double scale)
{
	m_nativeScene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit(100));
}