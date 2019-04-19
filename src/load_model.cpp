#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "load_model.hpp"

using namespace std;

namespace
{
	class AssimpProgress : public Assimp::ProgressHandler
	{
	public:
		AssimpProgress(const char* model_file)
		: m_model_file{ model_file } {}

		virtual bool Update(float percentage)
		{
			if(percentage - m_last_percentage >= 0.1f)
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
}
namespace engine
{
	flat_model_data_ptr make_flat_model_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer)
	{
		return make_shared<flat_model_data>(
			point_buffer,
			color_buffer,
			texcoord_buffer,
			normal_buffer,
			tangent_buffer,
			bitangent_buffer);
	}

	indexed_model_data_ptr make_indexed_model_data(
		const point_buffer& point_buffer,
		const color_buffer& color_buffer,
		const texcoord_buffer& texcoord_buffer,
		const normal_buffer& normal_buffer,
		const tangent_buffer& tangent_buffer,
		const bitangent_buffer& bitangent_buffer,
		const index_buffer& index_buffer)
	{
		return make_shared<indexed_model_data>(
			point_buffer,
			color_buffer,
			texcoord_buffer,
			normal_buffer,
			tangent_buffer,
			bitangent_buffer,
			index_buffer);
	}

	indexed_model_data_array load_indexed_model_data(const char* model_file)
	{
		Assimp::Importer importer;
		importer.SetProgressHandler(new AssimpProgress(model_file));

		const aiScene* scene = importer.ReadFile(model_file,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ImproveCacheLocality);
		if(scene == nullptr) throw runtime_error(string("Assimp::Importer::ReadFile failed! ") + importer.GetErrorString());

		assert(scene->mNumMeshes > 0);

		cout << "Processing " << model_file << "..." << endl;

		indexed_model_data_array result;

		for(unsigned int m = 0; m < scene->mNumMeshes; ++m)
		{
			const aiMesh* mesh = scene->mMeshes[m];
			assert(mesh->mNumVertices > 0 && mesh->mNumFaces > 0);
			assert(mesh->HasPositions());

			cout << "\tmesh #" << (m + 1) << endl <<
				"\t\tvertices: " << mesh->mNumVertices << endl <<
				"\t\tfaces: " << mesh->mNumFaces <<  endl <<
				"\t\tnormals: " << mesh->HasNormals() << endl <<
				"\t\ttan/bi: " << mesh->HasTangentsAndBitangents() << endl <<
				"\t\ttexcoords: " << mesh->HasTextureCoords(0) << " (channels: " << mesh->GetNumUVChannels() << ")" << endl;

			point_buffer position_buffer;
			color_buffer color_buffer;
			texcoord_buffer texcoord_buffer;
			normal_buffer normal_buffer;
			tangent_buffer tangent_buffer;
			bitangent_buffer bitangent_buffer;

			position_buffer.reserve(mesh->mNumVertices);
			if(mesh->HasVertexColors(0)) color_buffer.reserve(mesh->mNumVertices);
			if(mesh->HasTextureCoords(0)) texcoord_buffer.reserve(mesh->mNumVertices);
			if(mesh->HasNormals()) normal_buffer.reserve(mesh->mNumVertices);
			if(mesh->HasTangentsAndBitangents()) tangent_buffer.reserve(mesh->mNumVertices);
			if(mesh->HasTangentsAndBitangents()) bitangent_buffer.reserve(mesh->mNumVertices);

			for (unsigned int i = 0 ; i < mesh->mNumVertices ; ++i)
			{
				const aiVector3D* position = &(mesh->mVertices[i]);
				position_buffer.push_back(point(position->x, position->y, position->z));

				if(mesh->HasVertexColors(0))
				{
					const aiColor4D* c = &(mesh->mColors[0][i]);
					color_buffer.push_back(color(c->r, c->g, c->b, c->a));
				}

				if(mesh->HasTextureCoords(0))
				{
					const aiVector3D* texture_coord = &(mesh->mTextureCoords[0][i]);
					texcoord_buffer.push_back(texcoord(texture_coord->x, texture_coord->y));
				}

				if(mesh->HasNormals())
				{
					const aiVector3D* normal = &(mesh->mNormals[i]);
					normal_buffer.push_back(vector(normal->x, normal->y, normal->z));
				}

				if(mesh->HasTangentsAndBitangents())
				{
					const aiVector3D* tangent = &(mesh->mTangents[i]);
					const aiVector3D* bitangent = &(mesh->mBitangents[i]);
					tangent_buffer.push_back(vector(tangent->x, tangent->y, tangent->z));
					bitangent_buffer.push_back(vector(bitangent->x, bitangent->y, bitangent->z));
				}
			}

			index_buffer index_buffer;
			index_buffer.reserve(mesh->mNumFaces * 3);

			for (unsigned int i = 0 ; i < mesh->mNumFaces ; ++i)
			{
				const aiFace& face = mesh->mFaces[i];

				index_buffer.push_back(face.mIndices[0]);
				index_buffer.push_back(face.mIndices[1]);
				index_buffer.push_back(face.mIndices[2]);
			}

			result.push_back(make_indexed_model_data(
				position_buffer,
				color_buffer,
				texcoord_buffer,
				normal_buffer,
				tangent_buffer,
				bitangent_buffer,
				index_buffer));
		}

		return result;
	}
}
