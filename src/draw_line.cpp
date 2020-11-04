#include "draw_line.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <algorithm>

cg::LineDrawing::LineDrawing(unsigned width, unsigned height): cg::ClearRenderTarget(width, height)
{
}

cg::LineDrawing::~LineDrawing()
{
}

void cg::LineDrawing::DrawLine(unsigned x_begin, unsigned y_begin, unsigned x_end, unsigned y_end, color color)
{
	int A = 1.0 * y_begin - y_end;
	int B = 1.0 * x_end - x_begin;
	int C = -1.0 * A * x_begin - 1.0 * B * y_begin;

	if (std::abs((int)(x_end - x_begin)) > abs((int)(y_end - y_begin))) {
		for (unsigned x = std::min(x_begin, x_end); x <= std::max(x_begin, x_end); x++) {
			double y = std::max(std::min((1.0 * A * x + C) / -B, height - 1.0), 0.0);
			SetPixel(x, round(y), color);
		}
	}
	else {
		for (unsigned y = std::min(y_begin, y_end); y <= std::max(y_begin, y_end); y++) {
			double x = std::max(std::min((1.0 * B * y + C) / -A, width - 1.0), 0.0);
			SetPixel(round(x), y, color);
		}
	}
}

void cg::LineDrawing::DrawScene()
{
    unsigned x_center = width / 2;
    unsigned y_center = height / 2;
	unsigned radius = std::min(x_center, y_center) - 1;
	for (double angle = 0.0; angle < 360.0; angle += 5.0)
	{
		DrawLine(x_center, y_center, static_cast<unsigned>(x_center + radius * cos(angle * M_PI / 180)), static_cast<unsigned>(y_center + radius * sin(angle * M_PI / 180)),
            color(static_cast<unsigned char>(255 * sin(angle * M_PI / 180)), static_cast<unsigned char>(255 * cos(angle * M_PI / 180)), 255));
	}

}

