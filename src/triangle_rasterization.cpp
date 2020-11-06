#include "triangle_rasterization.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


cg::TriangleRasterization::TriangleRasterization(unsigned short width, unsigned short height, std::string obj_file) : cg::Projections(width, height, obj_file)
{
}

cg::TriangleRasterization::~TriangleRasterization()
{
}

void cg::TriangleRasterization::DrawScene()
{
    unsigned id = 0;
    for (auto face : parser->GetFaces()) {
        //std::cout << face.vertexes[0] << " " << face.vertexes[1] << " " << face.vertexes[2] << "\n";
        face.primitive_id = id++;
        for (unsigned i = 0; i < 3; i++) {
            face.vertexes[i] = VertexShader(face.vertexes[i]);
        }

        Rasterizer(face);

        /*auto vs_out = VertexShader(face);
        auto raster_out = Rasterizer(vs_out);
        auto ps_out = PixelShader(raster_out);
        OutMerger(ps_out);*/
    }
}

void cg::TriangleRasterization::DrawTriangle(cg::face face)
{
    float x_min = std::min(face.vertexes[0].x, std::min(face.vertexes[1].x, face.vertexes[2].x));
    float y_min = std::min(face.vertexes[0].y, std::min(face.vertexes[1].y, face.vertexes[2].y));

    float x_max = std::max(face.vertexes[0].x, std::max(face.vertexes[1].x, face.vertexes[2].x));
    float y_max = std::max(face.vertexes[0].y, std::max(face.vertexes[1].y, face.vertexes[2].y));

    float direction = 1;
    float area = direction * EdgeFunction(face.vertexes[0].xy(), face.vertexes[1].xy(), face.vertexes[2].xy());

    for (float x = x_min; x <= x_max; x += 1.f)
        for (float y = y_min; y <= y_max; y += 1.f) {
            float2 point{ x, y };
            
            float e0 = direction * EdgeFunction(face.vertexes[0].xy(), face.vertexes[1].xy(), point);
            float e1 = direction * EdgeFunction(face.vertexes[1].xy(), face.vertexes[2].xy(), point);
            float e2 = direction * EdgeFunction(face.vertexes[2].xy(), face.vertexes[0].xy(), point);

            if (e0 >= 0.f && e1 >= 0.f && e2 >= 0.f) {
                float3 bary{ e0 / area, e1 / area, e2 / area };
                SetPixel(round(x), round(y), PixelShader(point, bary, face.primitive_id));
            }
        }


    //Wireframe
    DrawLine(face.vertexes[0].x, face.vertexes[0].y,
        face.vertexes[1].x, face.vertexes[1].y,
        color(255, 0, 0));

    DrawLine(face.vertexes[1].x, face.vertexes[1].y,
        face.vertexes[2].x, face.vertexes[2].y,
        color(0, 255, 0));

    DrawLine(face.vertexes[2].x, face.vertexes[2].y,
        face.vertexes[0].x, face.vertexes[0].y,
        color(0, 0, 255));
}

cg::color cg::TriangleRasterization::PixelShader(float2 coordinates, float3 bary, unsigned id)
{
    float3 inter_color = bary * 255;
    return color(round(inter_color.x), round(inter_color.y), round(inter_color.z));
}

float cg::TriangleRasterization::EdgeFunction(float2 a, float2 b, float2 c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}


