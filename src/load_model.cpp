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

		real_buffer pb, nb, tb;
		pb.reserve(mesh->mNumVertices * 3);
		nb.reserve(mesh->mNumVertices * 3);
		tb.reserve(mesh->mNumVertices * 2);

		for (unsigned int i = 0 ; i < mesh->mNumVertices ; ++i)
		{
			const aiVector3D* pos = &(mesh->mVertices[i]);
			const aiVector3D* normal = &(mesh->mNormals[i]);
			const aiVector3D* texcoord = &(mesh->mTextureCoords[0][i]);

			pb.push_back(pos->x);
			pb.push_back(pos->y);
			pb.push_back(pos->z);

			nb.push_back(normal->x);
			nb.push_back(normal->y);
			nb.push_back(normal->z);

			tb.push_back(texcoord->x);
			tb.push_back(texcoord->y);
		}

		int_buffer ib;
		ib.reserve(mesh->mNumFaces * 3);

		for (unsigned int i = 0 ; i < mesh->mNumFaces ; ++i)
		{
			const aiFace& face = mesh->mFaces[i];

			ib.push_back(face.mIndices[0]);
			ib.push_back(face.mIndices[1]);
			ib.push_back(face.mIndices[2]);
		}

		return model_data{ pb, nb, tb, ib };
	}
}
