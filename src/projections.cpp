#include "Projections.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"


cg::ObjParser::ObjParser(std::string filename) :
	filename(filename)
{
    faces.clear();
}


cg::ObjParser::~ObjParser()
{
	faces.clear();
}

void cg::ObjParser::Parse()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, 
        filename.string().c_str(), filename.parent_path().string().c_str(), true);

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error("Can't parse OBJ file");
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            face current_face;

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

                current_face.vertexes[v] = float4(vx, vy, vz, 1.f);
            }
            index_offset += fv;

            faces.push_back(current_face);
        }
    }
}

const std::vector<cg::face>& cg::ObjParser::GetFaces()
{
	return faces;
}



cg::Projections::Projections(unsigned short width, unsigned short height, std::string obj_file) : cg::LineDrawing(width, height)
{
    parser = new ObjParser(obj_file);
    parser->Parse();
}

cg::Projections::~Projections()
{
    delete parser;
}

void cg::Projections::DrawScene()
{
    for (auto face : parser->GetFaces()) {
        std::cout << face.vertexes[0] << " " << face.vertexes[1] << " " << face.vertexes[2]  << "\n";
    }
}


