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
    VkDevice device; //logical device
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkQueue presentQueue;

    VkSurfaceFormatKHR swapchainImageFormat;
    VkExtent2D swapchainExtent;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    
    void createInstance(SDL_Window* window);
    bool checkValidationLayerSupport();
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    void createLogicalDevice();
    void createSwapChain(SDL_Window* window);
    void cleanUp();

    struct QueueFamilyIndices
    {
        QueueFamilyIndices() = default;
        ~QueueFamilyIndices()
        {
            if(graphicsFamily != nullptr)
            {
                delete graphicsFamily;
            }
            if(presentFamily != nullptr)
            {
                delete presentFamily;
            }
        }
        
        bool isComplete()
        {
            return (graphicsFamily != nullptr && presentFamily != nullptr);
        }
        uint32_t* graphicsFamily = nullptr;
        uint32_t* presentFamily = nullptr;
    };
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void createSurface(SDL_Window* window);

    //joys of swapchain
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(SDL_Window* window, const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();
    
};
