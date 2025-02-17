#pragma once
#include "Inc.h"

namespace ACJL 
{
	enum DataType
	{
		START,
		END,
		MESH,
		VERTEX,
		TEXTURE,
		MATERIAL,
		LIGHT,
		CAMERA,
		KEYFRAMES,
		BLENDSHAPES,
		ANIMATION
	};

	enum TextureType
	{
		DIFFUSE,
		NORMAL
	};

	enum MaterialType
	{
		LAMBERT,
		PHONG
	};

	enum VertexType
	{
		BASICVTX,
		SKELETONVTX
	};

	enum LightType
	{
		POINTLIGHT,
		DIRECTIONAL,
		SPOT,
		AREA,
		VOLUME
	};

	/*enum CameraType
	{

	};*/

	//____________________________________________ Generals ____________________________________________//

	struct Start
	{
		DataType dt = START;

		unsigned int nrOfMeshes = 0;
		unsigned int nrOfLight = 0;
		unsigned int nrOfCams = 0;
	};

	struct End
	{
		DataType dt = END;

		char endChar = '0';
	};

	//____________________________________________ Mesh ____________________________________________//
	struct Mesh
	{
		DataType dt = MESH;
		char meshName[50] = { "" };
		VertexType vt = BASICVTX;
		Vector3 trans = { 0.0f, 0.0f, 0.0f };
		Vector3 rot = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 0.0f, 0.0f, 0.0f };
		int nrOfAnimation = 0;
		int nrOfMaterial = 0;
		int nrOfTextures = 0;
		int nrOfVertices = 0;
		int nrOfIndices = 0;
	};

	//____________________________________________ Vertex ____________________________________________//
	struct Vertex
	{
		Vector3 pos = { 0.0f, 0.0f, 0.0f };
		Vector2 uv = { 0 ,0 };
		Vector3 normals = { 0.0f, 0.0f, 0.0f };
		Vector3 tangents = { 0.0f, 0.0f, 0.0f };
		Vector3 bitangents = { 0.0f, 0.0f, 0.0f };
	};

	//____________________________________________ Texture ____________________________________________//
	struct Texture
	{
		DataType dt = TEXTURE;
		TextureType tt;
		char path[200] = { "" };
		char name[100] = { "" };
	};

	//____________________________________________ Material ____________________________________________//
	struct MaterialID
	{
		char matName[100] = { "" };
	};

	struct Material
	{
		DataType dt = MATERIAL;
		char matName[50] = { "" };
		MaterialType mt;
		Vector3 ambient = { 0.0f, 0.0f, 0.0f };
		Vector3 diffuse = { 0.0f, 0.0f, 0.0f };
		Vector3 specular = { 0.0f, 0.0f, 0.0f };
		float specularIntensity = 0.0f;
		char albedoName[500] = { "" };
		char normalName[100] = { "" };
	};
	//____________________________________________ Light ____________________________________________//
	struct Light
	{
		DataType dt = LIGHT;
		LightType lt;
		char name[50] = { "" };
		float trans[3] = { 0.0f, 0.0f, 0.0f };
		float rot[3] = { 0.0f, 0.0f, 0.0f };
		float scale[3] = { 0.0f, 0.0f, 0.0f };
		//EVERYTHING FROM FBXLIGHT
		float rgb[3] = { 0.0f, 0.0f, 0.0f };
		float intensity = 0.0f;
		float outerAngle = 0.0f;
		float fog = 0.0f;
	};

	//____________________________________________ Camera ____________________________________________//
	struct Camera
	{
		DataType dt = CAMERA;
		char name[50] = { "" };
		Vector3 pos = { 0.0f, 0.0f, 0.0f };
		Vector3 upVec = { 0.0f, 0.0f, 0.0f };
		Vector3 forVec = { 0.0f, 0.0f, 0.0f };
		float FOV = 0.0f;
		float nearZ = 0.0f;
		float farZ = 0.0f;

	};

	//____________________________________________ Animation ____________________________________________//
	struct BlendShape
	{
		DataType dt = BLENDSHAPES;
	};

	struct BindPose
	{
		char ParentID[50] = { "" };
		VertexType vt = SKELETONVTX;

	};

	struct AnimationStart
	{

	};

	struct SkeletalAnimation
	{
		float duration = 0.0f;
		int nrOfKeyframes = 0;
		int nrOfJoints = 0;
	};

	struct Keyframe
	{
		DataType dt = KEYFRAMES;
		float timeStamp = 0.0f;
		Vector3 trans = { 0.0f, 0.0f, 0.0f };
		Vector3 rot = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 0.0f, 0.0f, 0.0f };
	};
}