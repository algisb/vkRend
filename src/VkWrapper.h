#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>
#include "DbgPrint.h"

class VkWrapper
{
public:
    const std::vector<const char*> validationLayers =
    {
    "VK_LAYER_KHRONOS_validation"
        
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    
    VkWrapper();
    ~VkWrapper();
    
    void init(SDL_Window* window);
    void deinit();
    
private:
    VkInstance instance;
    
    bool checkValidationLayerSupport();
    void createInstance(SDL_Window* window);
    
};
