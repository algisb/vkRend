#include "VkWrapper.h"

VkWrapper::VkWrapper()
{
    
}

VkWrapper::~VkWrapper()
{
    
}

void VkWrapper::init(SDL_Window* window)
{
    dbgPrint("init vulkan\n");
    createInstance(window);
}

void VkWrapper::deinit()
{
    dbgPrint("deinit vulkan\n");
    vkDestroyInstance(instance, nullptr);
}

bool VkWrapper::checkValidationLayerSupport()
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
            //dbgPrint("layer name: %s\n", layerProperties.layerName);
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
    
    
void VkWrapper::createInstance(SDL_Window* window)
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
    
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } 
    else 
    {
        createInfo.enabledLayerCount = 0;
    }
    
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
