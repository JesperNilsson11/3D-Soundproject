#pragma once

#include <memory>
#include <vector>
#include "Graphics.h"
#include "Vertexbuffer.h"
#include "IndexBuffer.h"
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include "texture.h"

class Model {
public:
	Model(Graphics& g, std::string filename);

	void Draw(Graphics& g);
	void SetModelMatrix(dx::XMFLOAT4X4 matrix) { model_matrix = matrix; }
	dx::XMFLOAT4X4 GetModelMatrix() { return model_matrix; }

private:
	std::vector<std::unique_ptr<VertexBuffer>> vbs;
	std::vector<std::unique_ptr<Indexbuffer>> ibs;
	//std::vector<Texture> textures;
	dx::XMFLOAT4X4 model_matrix;

private:
	void processNode(Graphics& g, aiNode* node, const aiScene* scene);
	void processMesh(Graphics& g, aiMesh* mesh, const aiScene* scene);
};