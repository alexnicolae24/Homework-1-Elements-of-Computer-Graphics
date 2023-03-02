#include "lab_m1/Tema1/Tema1.h"
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace objects;


Mesh* objects::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects::CreateTriangle(
	const std::string& name, 
	glm::vec3 leftBottomCorner, 
	float length, 
	glm::vec3 color, 
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2 };

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* objects::CreateCircle(
	const std::string& name, 
	glm::vec3 center,
	float length, 
	glm::vec3 color, 
	bool fill)
{

	std::vector<VertexFormat> vertices = {
		VertexFormat(center + glm::vec3(0, 0, 0), color)
	};

	for (int i = 0; i < 100 + 1; ++i) {
		float angle = -M_PI / 2 + M_PI / 45 * i;
		vertices.push_back(VertexFormat(center + glm::vec3(length * cos(angle), length * sin(angle), 0), color));
	}
	Mesh* circle = new Mesh(name);

	std::vector<unsigned int> indices;
	for (int i = 1; i <= 100; ++i) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(1 + i % 100);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}
