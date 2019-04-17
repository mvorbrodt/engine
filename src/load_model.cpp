#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "load_model.hpp"

using namespace std;

namespace engine
{
	model_data_array load_model(const char* model_file)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(model_file,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices);
		if(scene == nullptr) throw runtime_error(string("Assimp::Importer::ReadFile failed! ") + importer.GetErrorString());

		assert(scene->mNumMeshes > 0);

		model_data_array result;

		for(unsigned int m = 0; m < scene->mNumMeshes; ++m)
		{
			const aiMesh* mesh = scene->mMeshes[m];
			assert(mesh->mNumVertices > 0 && mesh->mNumFaces > 0);

			cout << model_file << ", vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces << endl;

			point_buffer position_buffer;
			vector_buffer normal_buffer;
			vector_buffer tangent_buffer;
			texcoord_buffer texcoord_buffer;

			position_buffer.reserve(mesh->mNumVertices);
			normal_buffer.reserve(mesh->mNumVertices);
			tangent_buffer.reserve(mesh->mNumVertices);
			texcoord_buffer.reserve(mesh->mNumVertices);

			for (unsigned int i = 0 ; i < mesh->mNumVertices ; ++i)
			{
				const aiVector3D* position = &(mesh->mVertices[i]);
				const aiVector3D* normal = &(mesh->mNormals[i]);
				const aiVector3D* tangent = &(mesh->mTangents[i]);
				const aiVector3D* texture_coord = &(mesh->mTextureCoords[0][i]);

				position_buffer.push_back(point(position->x, position->y, position->z));
				normal_buffer.push_back(vector(normal->x, normal->y, normal->z));
				tangent_buffer.push_back(vector(tangent->x, tangent->y, tangent->z));
				texcoord_buffer.push_back(texcoord(texture_coord->x, texture_coord->y));
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

			result.push_back(make_shared<model_data>(position_buffer, normal_buffer, tangent_buffer, texcoord_buffer, index_buffer));
		}

		return result;
	}
}
