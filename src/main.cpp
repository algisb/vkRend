#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#define ENABLE_DBG_PRINT

#ifdef ENABLE_DBG_PRINT
#define dbgPrint(_str, ...)\
{\
    printf(_str, ##__VA_ARGS__);\
}
#else
#define dbgPrint(_str, ...)
#endif



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
    
    VkWrapper()
    {}
    ~VkWrapper()
    {}
    void init(SDL_Window* window)
    {
        dbgPrint("init vulkan\n");
        createInstance(window);
    }
    
    void deinit()
    {
        dbgPrint("deinit vulkan\n");
        vkDestroyInstance(instance, nullptr);
    }
    
private:
    VkInstance instance;
    
    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers)
            {
                dbgPrint("layer name: %s\n", layerProperties.layerName);
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
    
    
    void createInstance(SDL_Window* window)
    {
        if (enableValidationLayers && !checkValidationLayerSupport()) 
        {
            dbgPrint("ERROR: validation layers requested, but not available\n");
        }
        
        dbgPrint("create instance\n");
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        // Get the required extension count
        unsigned int count;
        if (!SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr))
        {
            dbgPrint("ERROR: SDL_Vulkan_GetInstanceExtensions failed\n");
        }
        
        std::vector<const char*> extensions;
        extensions.resize(count);

        if (!SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data()))
        {
            dbgPrint("ERROR: SDL_Vulkan_GetInstanceExtensions failed\n");
        }

        // Now we can make the Vulkan instance
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            dbgPrint("ERROR: vkCreateInstance failed\n");
        }
        
        //Check available loaded extensions 
        uint32_t vkExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
        std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());
        dbgPrint("available extensions:\n");

        for (const auto& entry : vkExtensions)
        {
            dbgPrint("\t %s\n", entry.extensionName);
        }
    }
    
};

int main(int argc, char *argv[])
{
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
