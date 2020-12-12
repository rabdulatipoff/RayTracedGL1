#pragma once

#include "Buffer.h"
#include <list>

class ScratchBuffer
{
public:
    explicit ScratchBuffer(VkDevice device, std::shared_ptr<PhysicalDevice> physDevice);

    ScratchBuffer(const ScratchBuffer& other) = delete;
    ScratchBuffer(ScratchBuffer&& other) noexcept = delete;
    ScratchBuffer& operator=(const ScratchBuffer& other) = delete;
    ScratchBuffer& operator=(ScratchBuffer&& other) noexcept = delete;

    // get scratch buffer address for given acceleration structure
    VkDeviceAddress GetScratchAddress(VkAccelerationStructureKHR as, bool update = false);
    void Reset();

private:
    VkDeviceSize GetScratchSize(VkAccelerationStructureKHR as, bool update) const;
    void AddChunk(VkDeviceSize size);

private:
    struct ChunkBuffer
    {
        Buffer buffer;
        uint32_t currentOffset = 0;
    };

    VkDevice device;

    std::weak_ptr<PhysicalDevice> physDevice;
    std::list<ChunkBuffer> chunks;
};