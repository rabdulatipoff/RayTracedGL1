#include "PhysicalDevice.h"

PhysicalDevice::PhysicalDevice(VkInstance instance, uint32_t selectedPhysDevice)
{
    VkResult r;

    uint32_t physCount = 0;
    r = vkEnumeratePhysicalDevices(instance, &physCount, nullptr);
    assert(physCount > 0);
    assert(selectedPhysDevice < physCount);

    std::vector<VkPhysicalDevice> physicalDevices;
    physicalDevices.resize(physCount);
    r = vkEnumeratePhysicalDevices(instance, &physCount, physicalDevices.data());
    VK_CHECKERROR(r);

    physDevice = physicalDevices[selectedPhysDevice];

    vkGetPhysicalDeviceMemoryProperties(physDevice, &memoryProperties);

    rayTracingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_KHR;
    VkPhysicalDeviceProperties2 deviceProp2 = {};
    deviceProp2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    deviceProp2.pNext = &rayTracingProperties;
    vkGetPhysicalDeviceProperties2(physDevice, &deviceProp2);
}

void PhysicalDevice::SetDevice(VkDevice device)
{
    this->device = device;
}

VkPhysicalDevice PhysicalDevice::Get() const
{
    return physDevice;
}

uint32_t PhysicalDevice::GetMemoryTypeIndex(uint32_t memoryTypeBits, VkFlags requirementsMask) const
{
    // for each memory type available for this device
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        // if type is available
        if ((memoryTypeBits & 1u) == 1)
        {
            if ((memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask)
            {
                return i;
            }
        }

        memoryTypeBits >>= 1u;
    }

    assert(0);
    return 0;
}

VkDeviceMemory PhysicalDevice::AllocDeviceMemory(const VkMemoryRequirements& memReqs, bool addressQuery) const
{
    VkDeviceMemory memory;

    VkMemoryAllocateInfo memAllocInfo = {};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memReqs.size;
    memAllocInfo.memoryTypeIndex = GetMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkMemoryAllocateFlagsInfo allocFlagInfo = {};

    if (addressQuery)
    {
        allocFlagInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
        allocFlagInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

        memAllocInfo.pNext = &allocFlagInfo;
    }

    VkResult r = vkAllocateMemory(device, &memAllocInfo, nullptr, &memory);
    VK_CHECKERROR(r);

    return memory;
}

VkDeviceMemory PhysicalDevice::AllocDeviceMemory(const VkMemoryRequirements2& memReqs2, bool addressQuery) const
{
    return AllocDeviceMemory(memReqs2.memoryRequirements, addressQuery);
}

VkPhysicalDeviceMemoryProperties PhysicalDevice::GetMemoryProperties() const
{
    return memoryProperties;
}

VkPhysicalDeviceRayTracingPropertiesKHR PhysicalDevice::GetRayTracingProperties() const
{
    return rayTracingProperties;
}
