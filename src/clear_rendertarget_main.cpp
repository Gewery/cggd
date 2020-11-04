#define STBI_WRITE_NO_STDIO
#include "clear_rendertarget.h"

#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
    try
    {
        cg::ClearRenderTarget* render = new cg::ClearRenderTarget(1920, 1080);

        auto start = std::chrono::high_resolution_clock::now();

        render->Clear();
        std::cout << "time: " << ((std::chrono::duration<double>)(std::chrono::high_resolution_clock::now() - start)).count() << "\n";

        render->Save("results/clear_rendertarget.png");

        // Just show the resulted image
        system("start results/clear_rendertarget.png");
    }
    catch (std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}