#include "pch.h"
#include "model.h"

Model::Model(Graphics& g, std::string filename) {
	dx::XMMATRIX mat = dx::XMMatrixIdentity();
	dx::XMStoreFloat4x4(&model_matrix, mat);
	Assimp::Importer importer;
	std::string path = assets + filename;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		MessageBoxA(NULL, importer.GetErrorString(), "Assimp Failed", 0);
		throw 4;
	}

	processNode(g, scene->mRootNode, scene);
}

void Model::processNode(Graphics& g, aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(g, mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(g, node->mChildren[i], scene);
	}
}

void Model::processMesh(Graphics& g, aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.tex[0] = mesh->mTextureCoords[0][i].x;
			vertex.tex[1] = 1 - mesh->mTextureCoords[0][i].y;
			
		}
		else {
			vertex.tex[0] = 0;
			vertex.tex[1] = 0;
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	vbs.push_back(std::make_unique<VertexBuffer>(g, vertices.data(), vertices.size()));
	ibs.push_back(std::make_unique<Indexbuffer>(g, indices.data(), indices.size()));

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			int temp = 3;
			//textures.emplace_back(g, str.C_Str());
		}
	}
}

void Model::Draw(Graphics& g) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < vbs.size(); i++) {
		vbs[i]->bind(g);
		ibs[i]->bind(g);
		//textures[i].bind(g);

		g.Context()->DrawIndexed(ibs[i]->getNum(), 0, 0);
	}
}