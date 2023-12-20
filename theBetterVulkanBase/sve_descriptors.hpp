#ifndef SVE_DESCRIPTORS_HPP
#define SVE_DESCRIPTORS_HPP

#include "sve_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace sve {

    class SveDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(SveDevice& sveDevice) : sveDevice{ sveDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<SveDescriptorSetLayout> build() const;

        private:
            SveDevice& sveDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        SveDescriptorSetLayout(
            SveDevice& sveDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~SveDescriptorSetLayout();
        SveDescriptorSetLayout(const SveDescriptorSetLayout&) = delete;
        SveDescriptorSetLayout& operator=(const SveDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        SveDevice& sveDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class SveDescriptorWriter;
    };


    class SveDescriptorPool {
    public:
        class Builder {
        public:
            Builder(SveDevice& sveDevice) : sveDevice{ sveDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<SveDescriptorPool> build() const;

        private:
            SveDevice& sveDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        SveDescriptorPool(
            SveDevice& sveDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~SveDescriptorPool();
        SveDescriptorPool(const SveDescriptorPool&) = delete;
        SveDescriptorPool& operator=(const SveDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        SveDevice& sveDevice;
        VkDescriptorPool descriptorPool;

        friend class SveDescriptorWriter;
    };


    class SveDescriptorWriter {
    public:
        SveDescriptorWriter(SveDescriptorSetLayout& setLayout, SveDescriptorPool& pool);

        SveDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        SveDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        SveDescriptorSetLayout& setLayout;
        SveDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}
#endif