#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <vulkan/vulkan.h>


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    
    uint32_t width = 1280;
    uint32_t height = 720;
    
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan Sample",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0
    );

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(window, &info);

    int run = true;
    while (run)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                run = 0;
            }
        }

    }

    SDL_Quit();

    return 0;
}
