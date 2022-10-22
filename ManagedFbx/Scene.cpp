#include "stdafx.h"
#include "Scene.h"
#include "Manager.h"

namespace ManagedFbx
{

	Scene::Scene(string^ name)
	{
		m_nativeScene = FbxScene::Create(Manager::GetInstance(), StringHelper::ToNative(name));
		m_rootNode = gcnew SceneNode(m_nativeScene->GetRootNode());
	}

	Scene^ Scene::CreateScene(string^ name)
	{
		auto scene = gcnew Scene(name);
		scene->m_rootNode = gcnew SceneNode(scene->m_nativeScene->GetRootNode());
		return scene;
	}

	Scene^ Scene::Import(string^ filename)
	{
		auto importer = Manager::GetImporter();

		if (!importer->Initialize(StringHelper::ToNative(filename)))
			throw gcnew FbxException("Failed to initialise the FBX importer: {0}", gcnew string(importer->GetStatus().GetErrorString()));

		auto scene = gcnew Scene("");

		if (!importer->Import(scene->m_nativeScene))
			throw gcnew FbxException("Failed to import the scene: {0}", gcnew string(importer->GetStatus().GetErrorString()));

		// Needs refreshing
		scene->m_rootNode = gcnew SceneNode(scene->m_nativeScene->GetRootNode());

		return scene;

	}


	void Scene::Save(string^ filename)
	{
		auto exporter = Manager::GetExporter();
		exporter->SetFileExportVersion(FBX_2014_00_COMPATIBLE);

		if (!exporter->Initialize(StringHelper::ToNative(filename)))
			throw gcnew FbxException("Failed to initialise the FBX exporter: {0}", gcnew string(exporter->GetStatus().GetErrorString()));

		if (!exporter->Export(m_nativeScene))
			throw gcnew FbxException("Failed to export the scene: {0}", gcnew string(exporter->GetStatus().GetErrorString()));
	}

	void Scene::Save(string^ filename, int format)
	{
		auto exporter = Manager::GetExporter();

		if (!exporter->Initialize(StringHelper::ToNative(filename), format))
			throw gcnew FbxException("Failed to initialise the FBX exporter: {0}", gcnew string(exporter->GetStatus().GetErrorString()));

		if (!exporter->Export(m_nativeScene))
			throw gcnew FbxException("Failed to export the scene: {0}", gcnew string(exporter->GetStatus().GetErrorString()));
	}

	void Scene::Name::set(string^ value)
	{
		m_nativeScene->SetName(StringHelper::ToNative(value));
	}

	string^ Scene::Name::get()
	{
		return StringHelper::ToManaged(m_nativeScene->GetName());
	}

	SceneNode^ Scene::RootNode::get()
	{
		return m_rootNode;
	}

	void Scene::Application::set(string^ value)
	{
		m_nativeScene->GetSceneInfo()->LastSaved_ApplicationName.Set(StringHelper::ToNative(value));
	}

	string^ Scene::Application::get()
	{
		auto name = m_nativeScene->GetSceneInfo()->LastSaved_ApplicationName.Get().Buffer();
		return StringHelper::ToManaged(name);
	}

	string^ Scene::UnitType::get()
	{
		return StringHelper::ToManaged(m_nativeScene->GetGlobalSettings().GetSystemUnit().GetScaleFactorAsString_Plurial());
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
		switch (units)
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
		switch (axis)
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

	void Scene::BakeTransform(SceneNode^ node)
	{
		for each (SceneNode ^ node in node->ChildNodes)
			BakeTransform(node);

		auto native = node->m_nativeNode;
		auto mesh = native->GetMesh();

		if (!mesh)
			return;

		FbxAMatrix geometry(native->GetGeometricTranslation(FbxNode::eSourcePivot),
			native->GetGeometricRotation(FbxNode::eSourcePivot),
			native->GetGeometricScaling(FbxNode::eSourcePivot));

		auto total = m_nativeScene->GetAnimationEvaluator()->GetNodeGlobalTransform(native) * geometry;

		for (int i = 0; i < mesh->GetControlPointsCount(); i++)
		{
			auto pos = mesh->GetControlPointAt(i);
			mesh->SetControlPointAt(total.MultT(pos), i);
		}
	}

	SceneNode^ Scene::CreateNode(Scene^ scene, string^ name)
	{
		const char* lNativeName = StringHelper::ToNative(name);

		FbxNode* lNativeNode = FbxNode::Create(scene->m_nativeScene, lNativeName);
		SceneNode^ result = gcnew SceneNode(lNativeNode);
		return result;
	}

	Mesh^ Scene::CreateMesh(Scene^ scene, SceneNode^ scenenode, string^ name) {
		const char* lNativeName = StringHelper::ToNative(name);

		FbxMesh* lNativeMesh = FbxMesh::Create(scene->m_nativeScene, lNativeName);
		FbxGeometryElementMaterial* lMaterialElement = lNativeMesh->CreateElementMaterial();
		lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
		lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
		FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(scene->m_nativeScene, "material");
		lMaterial->Diffuse.Set((FbxDouble3)(0.5, 0.5, 0.5));
		scenenode->m_nativeNode->AddMaterial(lMaterial);
		Mesh^ result = gcnew Mesh(lNativeMesh);
		return result;
	}

	void Scene::SetSceneScale(double scale)
	{
		m_nativeScene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit(100));
	}

	PhongMaterial^ Scene::CreatePhongMaterial(String^ name)
	{
		const char* lNativeName = StringHelper::ToNative(name);
		FbxSurfacePhong* phong = FbxSurfacePhong::Create(m_nativeScene, lNativeName);
		phong->ShadingModel.Set(FbxString("Phong"));
		PhongMaterial^ material = gcnew PhongMaterial(phong);
		return material;
	}

	ManagedTexture^ Scene::CreateFileTexture(string^ name)
	{
		const char* lNativeName = StringHelper::ToNative(name);
		FbxFileTexture* ltexture = FbxFileTexture::Create(m_nativeScene, lNativeName);
		ltexture->SetTextureUse(FbxTexture::eStandard);
		ltexture->SetMappingType(FbxTexture::eUV);
		ltexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
		ltexture->SetSwapUV(false);
		ltexture->SetTranslation(0.0, 0.0);
		ltexture->SetScale(1.0, 1.0);
		ltexture->SetRotation(0.0, 0.0);

		ManagedTexture^ lManagedTexture = gcnew ManagedTexture(ltexture);
		return lManagedTexture;
	}

	IEnumerable<string^>^ Scene::ExportFormats::get()
	{
		auto formats = gcnew List<string^>();
		auto registry = Manager::GetInstance()->GetIOPluginRegistry();
		int formatCount = registry->GetWriterFormatCount();
		for (int formatIndex = 0; formatIndex < formatCount; ++formatIndex)
		{
			formats->Add(gcnew string(registry->GetWriterFormatDescription(formatIndex)));
		}

		return formats;
	}
}