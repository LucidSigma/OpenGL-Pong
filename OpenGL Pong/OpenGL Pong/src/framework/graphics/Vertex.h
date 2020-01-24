#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

#include <array>

enum VertexAttributes
	: unsigned int
{
	Position,
	TextureCoordinates
};

struct Vertex
{
	glm::vec2 position;
	glm::vec2 textureCoordinates;
};

namespace vertices
{
	constexpr std::array<Vertex, 4u> QuadVertices{
		Vertex{ { 0.0f, 1.0f }, { 0.0f, 1.0f } },
		Vertex{ { 1.0f, 0.0f }, { 1.0f, 0.0f } },
		Vertex{ { 0.0f, 0.0f }, { 0.0f, 0.0f } },
		Vertex{ { 1.0f, 1.0f }, { 1.0f, 1.0f } }
	};

	constexpr std::array<unsigned short, 6u> QuadIndices{
		0u, 1u, 2u,
		0u, 3u, 1u
	};

	constexpr std::array<std::pair<float, float>, 4u> OutlineVertices{
		std::pair<float, float>{ 0.0f, 1.0f },
		std::pair<float, float>{ 1.0f, 0.0f },
		std::pair<float, float>{ 0.0f, 0.0f },
		std::pair<float, float>{ 1.0f, 1.0f }
	};

	constexpr std::array<Vertex, 6u> FrameVertices{
		Vertex{ { -1.0f, -1.0f }, { 0.0f, 0.0f } },
		Vertex{ { 1.0f, 1.0f }, { 1.0f, 1.0f } },
		Vertex{ { -1.0f, 1.0f }, { 0.0f, 1.0f } },
		Vertex{ { 1.0f, -1.0f }, { 1.0f, 0.0f } },
	};
}

#endif