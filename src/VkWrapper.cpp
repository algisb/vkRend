#include "VkWrapper.h"

#include <cstring>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#include <SDL2/SDL_vulkan.h>
#include "DbgPrint.h"

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
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}

void VkWrapper::deinit()
{
    dbgPrint("deinit vulkan\n");
    vkDestroyInstance(instance, nullptr);
    vkDestroyDevice(device, nullptr);
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

void VkWrapper::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (deviceCount == 0)
    {
        dbgPrint("ERROR: failed to find GPUs with Vulkan support!");
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    
    //TODO: atm we just select the first available device
    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        dbgPrint("ERROR: failed to find a suitable GPU!");
    }


}

bool VkWrapper::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    QueueFamilyIndices indices = findQueueFamilies(device);
    
    if(indices.isComplete() && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader)
    {
        dbgPrint("Selected GPU device: %s \n", deviceProperties.deviceName);
        return true;
    }
    else
    {
        return false;
    }
    //return true;
    
}

void VkWrapper::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = *indices.graphicsFamily;
    queueCreateInfo.queueCount = 1;
    
    float queuePriority = 1.0;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else 
    {
        createInfo.enabledLayerCount = 0;
    }
    
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        dbgPrint("ERROR:failed to create logical device!");
    }
    
    vkGetDeviceQueue(device, *indices.graphicsFamily, 0, &graphicsQueue);
}

VkWrapper::QueueFamilyIndices VkWrapper::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            if (indices.graphicsFamily == nullptr)
            {
                indices.graphicsFamily = new uint32_t;
            }
            *indices.graphicsFamily = i;
            break;
        }

        i++;
    }
    
    // Logic to find queue family indices to populate struct with
    return indices;
}

void VkWrapper::createSurface()
{
    
}

