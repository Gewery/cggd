#include "depth_buffer.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


cg::ZCulling::ZCulling(unsigned short width, unsigned short height, std::string obj_file) : TriangleRasterization(width, height, obj_file)
{
    depth_buffer.reserve(static_cast<size_t>(width * height));
}

cg::ZCulling::~ZCulling()
{
}

void cg::ZCulling::Clear()
{
    depth_buffer.clear();
    frame_buffer.resize(width * height);
    depth_buffer.resize(width * height, 1);
}

void cg::ZCulling::DrawTriangle(cg::face face)
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
                float3 bary{ e1 / area, e2 / area, e0 / area };
                float z = 1.0f - (bary.x * face.vertexes[0].z +
                    bary.y * face.vertexes[1].z +
                    bary.z * face.vertexes[2].z);

                SetPixel(round(x), round(y), PixelShader(point, bary, face.primitive_id), z);
            }
        }


    //Wireframe
   /* DrawLine(face.vertexes[0].x, face.vertexes[0].y,
        face.vertexes[1].x, face.vertexes[1].y,
        color(255, 0, 0));

    DrawLine(face.vertexes[1].x, face.vertexes[1].y,
        face.vertexes[2].x, face.vertexes[2].y,
        color(0, 255, 0));

    DrawLine(face.vertexes[2].x, face.vertexes[2].y,
        face.vertexes[0].x, face.vertexes[0].y,
        color(0, 0, 255));*/
}

void cg::ZCulling::SetPixel(unsigned short x, unsigned short y, color color, float z)
{
    if (depth_buffer[y * width + x] > z) {
        frame_buffer[y * width + x] = color;
        depth_buffer[y * width + x] = z;
    }
}


