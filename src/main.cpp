#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "VkWrapper.h"
#include "DbgPrint.h"

int main(int argc, char *argv[])
{
    //this won't work on windows
    char vkLayerPathEnv[] = "VK_LAYER_PATH=../external/Vulkan-ValidationLayers/layers";
    putenv(vkLayerPathEnv);
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    
    uint32_t width = 1280;
    uint32_t height = 720;
    
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan Sample",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 
        SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    
    VkWrapper vkWraper;
    vkWraper.init(window);
    
    

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

    vkWraper.deinit();
    SDL_Quit();

    return 0;
}
