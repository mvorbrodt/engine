#include <cassert>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

using namespace std;

class AssimpProgress : public Assimp::ProgressHandler
{
public:
	AssimpProgress(const char* model_file)
		: m_model_file{ model_file } {}

	virtual bool Update(float percentage)
	{
		if (percentage - m_last_percentage >= 0.1f)
		{
			cout << "Loading " << m_model_file << ", " << (int)(100 * percentage) << "% done..." << endl;
			m_last_percentage = percentage;
		}
		return true;
	}
private:
	const char* m_model_file;
	float m_last_percentage = 0.0f;
};

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "USAGE: " << argv[0] << " 'asset file name'" << endl;
		return -1;
	}

	Assimp::Importer importer;
	importer.SetProgressHandler(new AssimpProgress(argv[1]));

	const aiScene* scene = importer.ReadFile(argv[1],
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality);
	if (scene == nullptr)
	{
		cout << "Assimp::Importer::ReadFile failed! " << importer.GetErrorString() << endl;
		return -1;
	}

	const aiNode* root = scene->mRootNode;

	cout << endl;
	cout << "Root node      : " << root->mName.C_Str() << endl;
	cout << "Root transform : " << (root->mTransformation.IsIdentity() ? "no" : "yes") << endl << endl;

	cout << "Animations : " << scene->mNumAnimations << endl;
	cout << "Cameras    : " << scene->mNumCameras << endl;
	cout << "Lights     : " << scene->mNumLights << endl;
	cout << "Materials  : " << scene->mNumMaterials << endl;
	cout << "Textures   : " << scene->mNumTextures << endl;
	cout << "Meshes     : " << scene->mNumMeshes << endl;

	if (scene->HasCameras()) cout << endl;
	for (unsigned int i = 0; i < scene->mNumCameras; ++i)
	{
		cout << "Camera:" << endl;
		const aiCamera* camera = scene->mCameras[i];
		cout << "\tName     : " << camera->mName.C_Str() << endl;
		cout << "\tAspect   : " << camera->mAspect << endl;
		cout << "\tFOV      : " << camera->mHorizontalFOV * 180.0f / 3.14159265359f * 2.0f << endl;
		cout << "\tNear     : " << camera->mClipPlaneNear << endl;
		cout << "\tFar      : " << camera->mClipPlaneFar << endl;

		const aiNode* node = root->FindNode(camera->mName);
		assert(node->mParent == root);
		aiMatrix4x4 transformation = node->mTransformation;
		aiVector3D position = transformation * camera->mPosition;
		aiVector3D look_at = (aiMatrix3x3(transformation) * camera->mLookAt).Normalize();
		aiVector3D up = (aiMatrix3x3(transformation) * camera->mUp).Normalize();

		cout << "\tPosition : (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << endl;
		cout << "\tLook At  : (X: " << look_at.x << ", Y: " << look_at.y << ", Z: " << look_at.z << ")" << endl;
		cout << "\tUp       : (X: " << up.x << ", Y: " << up.y << ", Z: " << up.z << ")" << endl;
		cout << endl;
	}

	if (scene->HasLights()) cout << endl;
	for (unsigned int i = 0; i < scene->mNumLights; ++i)
	{
		cout << "Light:" << endl;
		const aiLight* light = scene->mLights[i];
		cout << "\tName       : " << light->mName.C_Str() << endl;
		cout << "\tType       : ";
		switch (light->mType)
		{
			case aiLightSource_UNDEFINED:   cout << "Undefined" << endl; break;
			case aiLightSource_DIRECTIONAL: cout << "Directional" << endl; break;
			case aiLightSource_POINT:       cout << "Point" << endl; break;
			case aiLightSource_SPOT:        cout << "Spot" << endl; break;
		}

		const aiNode* node = root->FindNode(light->mName);
		assert(node->mParent == root);
		aiMatrix4x4 transformation = node->mTransformation;
		aiVector3D position = transformation * light->mPosition;
		aiVector3D direction = (aiMatrix3x3(transformation) * light->mDirection).Normalize();

		cout << "\tPosition   : (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << endl;
		cout << "\tDirection  : (X: " << direction.x << ", Y: " << direction.y << ", Z: " << direction.z << ")" << endl;
		cout << "\tAmbient    : (R: " << light->mColorAmbient.r << ", G: " << light->mColorAmbient.g << ", B: " << light->mColorAmbient.b << ")" << endl;
		cout << "\tDiffuse    : (R: " << light->mColorDiffuse.r << ", G: " << light->mColorDiffuse.g << ", B: " << light->mColorDiffuse.b << ")" << endl;
		cout << "\tSpecular   : (R: " << light->mColorSpecular.r << ", G: " << light->mColorSpecular.g << ", B: " << light->mColorSpecular.b << ")" << endl;
		cout << "\tCone angle : (Inner: " << light->mAngleInnerCone << ", Outer: " << light->mAngleOuterCone << ")" << endl;
		cout << "\tAttenuation constant  : " << light->mAttenuationConstant << endl;
		cout << "\tAttenuation linear    : " << light->mAttenuationLinear << endl;
		cout << "\tAttenuation quadratic : " << light->mAttenuationQuadratic << endl;
		cout << endl;
	}

	if (scene->HasMaterials()) cout << endl;
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		cout << "Material:" << endl;
		const aiMaterial* material = scene->mMaterials[i];
		aiString str;
		material->Get(AI_MATKEY_NAME, str); cout << "\tName      : " << str.C_Str() << endl;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))  material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), str);  cout << "\tDiffuse   : " << str.C_Str() << endl;
		if (material->GetTextureCount(aiTextureType_SPECULAR)) material->Get(AI_MATKEY_TEXTURE_SPECULAR(0), str); cout << "\tSpecular  : " << str.C_Str() << endl;
		if (material->GetTextureCount(aiTextureType_NORMALS))  material->Get(AI_MATKEY_TEXTURE_NORMALS(0), str);  cout << "\tNormals   : " << str.C_Str() << endl;
	}

	if (scene->HasTextures()) cout << endl;
	for (unsigned int i = 0; i < scene->mNumTextures; ++i)
	{
		cout << "Texture:" << endl;
		const aiTexture* texture = scene->mTextures[i];
		cout << "\tHint     : " << texture->achFormatHint << endl;
		cout << "\tWidth    : " << texture->mWidth << endl;
		cout << "\tHeight   : " << texture->mHeight << endl;
		cout << endl;
	}

	if (scene->HasMeshes()) cout << endl;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		cout << "Mesh:" << endl;
		const aiMesh* mesh = scene->mMeshes[i];
		cout << "\tName                : " << mesh->mName.C_Str() << endl;

		const aiNode* node = root->FindNode(mesh->mName);
		aiMatrix4x4 transformation;
		if(node != nullptr)
		{
			while(node != nullptr)
			{
				cout << "\tNode name           : " << node->mName.C_Str() << endl;
				cout << "\tNode transformation : " << (node->mTransformation.IsIdentity() ? "no" : "yes") << endl;
				transformation = node->mTransformation * transformation;
				node = node->mParent;
			}
		}

		cout << "\tHas positions       : " << (mesh->HasPositions() ? "yes" : "no") << endl;
		cout << "\tHas faces           : " << (mesh->HasFaces() ? "yes" : "no") << endl;
		cout << "\tHas vertex colors   : " << (mesh->HasVertexColors(0) ? "yes" : "no") << endl;
		cout << "\tHas texture coords  : " << (mesh->HasTextureCoords(0) ? "yes" : "no") << endl;
		cout << "\tHas normals         : " << (mesh->HasNormals() ? "yes" : "no") << endl;
		cout << "\tHas tan/bitan       : " << (mesh->HasTangentsAndBitangents() ? "yes" : "no") << endl;
		cout << "\tHas bones           : " << (mesh->HasBones() ? "yes" : "no") << endl;
		cout << "\tColor channels      : " << mesh->GetNumColorChannels() << endl;
		cout << "\tUV channels         : " << mesh->GetNumUVChannels() << endl;
		cout << "\tUV components       : " << mesh->mNumUVComponents[0] << endl;
		cout << "\tMaterial index      : " << mesh->mMaterialIndex << endl;
		cout << "\tNumber of vertices  : " << mesh->mNumVertices << endl;
		cout << "\tNumber of faces     : " << mesh->mNumFaces << endl;
		cout << "\tNumber of bones     : " << mesh->mNumBones << endl;
		if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT)    cout << "\tPrimitive type      : Point" << endl;
		if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE)     cout << "\tPrimitive type      : Line" << endl;
		if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) cout << "\tPrimitive type      : Triangle" << endl;
		if (mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON)  cout << "\tPrimitive type      : Polygon" << endl;
		cout << endl;
	}
}
