#pragma once

#include "SceneNode.h"
#include "ConversionTypes.h"
#include "ManagedTexture.h"

using namespace System::Collections::Generic;

namespace ManagedFbx
{
	/// <summary>
	/// Represents an FBX scene.
	/// </summary>
	public ref class Scene
	{
	public:
		/// <summary>
		/// Constructs a new empty scene with the given name.
		/// </summary>
		Scene(string ^name);

		/// <summary>
		/// Imports a scene from a file.
		/// </summary>
		static Scene ^Import(string ^filepath);
		
		static Scene ^CreateScene(string ^name);
		static SceneNode ^CreateNode(Scene ^scene, string ^name);
		static Mesh ^CreateMesh(Scene ^scene, SceneNode ^scenenode, string ^name);

		/// <summary>
		/// Saves the scene to a file.
		/// </summary>
		void Save(string ^filepath);

		/// <summary>
		/// Converts the scene to a given unit system.
		/// </summary>
		void ConvertUnits(Unit units);

		/// <summary>
		/// Converts the scene to a given orientation.
		/// </summary>
		void ConvertAxes(AxisSystem axis);

		/// <summary>
		/// Bakes node transforms into vertex positions.
		/// </summary>
		void BakeTransform(SceneNode ^node);

		/// <summary>
		/// Gets the root node of this scene.
		/// </summary>
		property_r(SceneNode^, RootNode);

		/// <summary>
		/// Gets and sets the name of this scene.
		/// </summary>
		property_rw(string^, Name);

		/// <summary>
		/// Gets and sets the name of the application used to create this scene.
		/// </summary>
		property_rw(string^, Application);

		property_r(string^, UnitType);

		property_rw(double, UnitScale);

		property_rw(double, UnitMultipler);

		void SetSceneScale(double scale);

		PhongMaterial ^CreatePhongMaterial(String^ name);
		ManagedTexture ^CreateFileTexture(string^ name);

		FbxScene *m_nativeScene;

	private:
		SceneNode ^m_rootNode;
		FbxManager *m_manager;

		static int  nameCounter = 0;
	};
}