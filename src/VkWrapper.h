#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct SDL_Window;

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
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    
    void createInstance(SDL_Window* window);
    bool checkValidationLayerSupport();
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    
    struct QueueFamilyIndices
    {
        QueueFamilyIndices() = default;
        ~QueueFamilyIndices()
        {
            if(graphicsFamily != nullptr)
            {
                delete graphicsFamily;
            }
        }
        
        bool isComplete()
        {
            return graphicsFamily != nullptr;
        }
        uint32_t* graphicsFamily = nullptr;
    };
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    
};
