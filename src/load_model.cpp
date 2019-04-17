#include <cassert>
#include <iostream>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "load_model.hpp"

using namespace std;

namespace engine
{
	model_data load_model_file(const char* model_file)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(model_file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
		if(scene == nullptr)
		{
			throw runtime_error("Assimp::Importer::ReadFile failed!");
		}

		assert(scene->mNumMeshes == 1);
		const aiMesh* mesh = scene->mMeshes[0];
		assert(mesh->mNumVertices > 0 && mesh->mNumFaces > 0);

		cout << model_file << ", vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces << endl;

		real_buffer position_buffer, normal_buffer, texcoord_buffer;
		position_buffer.reserve(mesh->mNumVertices * 3);
		normal_buffer.reserve(mesh->mNumVertices * 3);
		texcoord_buffer.reserve(mesh->mNumVertices * 2);

		for (unsigned int i = 0 ; i < mesh->mNumVertices ; ++i)
		{
			const aiVector3D* position = &(mesh->mVertices[i]);
			const aiVector3D* normal = &(mesh->mNormals[i]);
			const aiVector3D* texcoord = &(mesh->mTextureCoords[0][i]);

			position_buffer.push_back(position->x);
			position_buffer.push_back(position->y);
			position_buffer.push_back(position->z);

			normal_buffer.push_back(normal->x);
			normal_buffer.push_back(normal->y);
			normal_buffer.push_back(normal->z);

			texcoord_buffer.push_back(texcoord->x);
			texcoord_buffer.push_back(texcoord->y);
		}

		int_buffer index_buffer;
		index_buffer.reserve(mesh->mNumFaces * 3);

		for (unsigned int i = 0 ; i < mesh->mNumFaces ; ++i)
		{
			const aiFace& face = mesh->mFaces[i];

			index_buffer.push_back(face.mIndices[0]);
			index_buffer.push_back(face.mIndices[1]);
			index_buffer.push_back(face.mIndices[2]);
		}

		return model_data{ position_buffer, normal_buffer, texcoord_buffer, index_buffer };
	}
}
