#pragma once
#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <functional>
#include <unordered_map>

namespace Engine
{
    class Resource {};

    template<typename T>
    struct ResourceDescriptor;

    template <typename T>
    concept ResourceConcept = requires(ResourceDescriptor<T> descriptor)
    {
        requires std::derived_from<T, Resource>;
        T(descriptor);
    };

    template<ResourceConcept T>
    class ResourceManager;

    template<ResourceConcept T>
    class ResourceHandle
    {
        private:
            friend class ResourceManager<T>;

            ResourceManager<T>* m_Manager = nullptr;
            unsigned int m_Index = 0;

            ResourceHandle(ResourceManager<T>* p_Manager, unsigned int index) : m_Manager(p_Manager), m_Index(index) {}
            void Release()
            {
                if (m_Manager && m_Index > 0) m_Manager->RemoveReference(m_Index);
                m_Manager = nullptr;
                m_Index = 0;
            }

        public:
            ResourceHandle() = delete;
            ResourceHandle(const ResourceHandle& other) : m_Manager(other.m_Manager), m_Index(other.m_Index)
            {
                if (m_Manager && m_Index > 0) m_Manager->AddReference(m_Index);
            }
            ResourceHandle(ResourceHandle&& other) noexcept : m_Manager(other.m_Manager), m_Index(other.m_Index)
            {   // Something bad?
                other.m_Manager = nullptr;
                other.m_Index = 0;
            }
            ~ResourceHandle() { Release(); }
            ResourceHandle& operator=(const ResourceHandle& other)
            {
                if (this != &other)
                {
                    Release();
                    m_Manager = other.m_Manager;
                    m_Index = other.m_Index;
                    if (m_Manager && m_Index > 0) m_Manager->AddReference(m_Index);
                }
                return *this;
            }
            ResourceHandle& operator=(ResourceHandle&& other) noexcept {
                if (this != &other)
                {
                    Release();
                    m_Manager = other.m_Manager;
                    m_Index = other.m_Index;
                    other.m_Manager = nullptr;
                    other.m_Index = 0;
                }
                return *this;
            }
            T* Get() const { return m_Manager ? m_Manager->GetResource(m_Index) : nullptr; }
            T* operator->() const
            { 
                if (!IsValid()) throw std::runtime_error("Dereferencing an invalid resource handle.");
                return Get(); 
            }
            T& operator*() const
            { 
                if (!IsValid()) throw std::runtime_error("Dereferencing an invalid resource handle.");
                return *Get(); 
            }
            explicit operator bool() const { return Get() != nullptr; }
            bool IsValid() const { return Get() != nullptr; }
    };

    template<ResourceConcept T>
    class ResourceManager
    {
        struct ResourceEntry
        {
            T* resource = nullptr;
            ResourceDescriptor<T> descriptor;
            unsigned int referenceCount = 0;
            unsigned int nextFree = 0;
            
            bool IsActive() const { return resource != nullptr; }
        };
        
        private:
            friend class ResourceHandle<T>;
            
            mutable std::mutex m_Mutex;
            std::unordered_map<ResourceDescriptor<T>, unsigned int> m_DescriptorToIndex;
            std::vector<ResourceEntry> m_Entries;
            unsigned int m_FreeListHead = 0;

            T* GetResource(unsigned int index) const
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (index >= m_Entries.size()) return nullptr;
                const ResourceEntry& entry = m_Entries[index];
                return entry.IsActive() ? entry.resource : nullptr;
            }
            void AddReference(unsigned int index)
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (index >= m_Entries.size()) return;
                ResourceEntry& entry = m_Entries[index];
                if (!entry.IsActive()) throw std::runtime_error("Adding reference to inactive entry.");
                ++entry.referenceCount;
            }
            void RemoveReference(unsigned int index)
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (index >= m_Entries.size()) return;
                ResourceEntry& entry = m_Entries[index];
                if (!entry.IsActive()) throw std::runtime_error("Adding reference to inactive entry.");
                if (entry.referenceCount == 0) throw std::runtime_error("Reference count already at zero.");
                if (--entry.referenceCount == 0)
                {
                    m_DescriptorToIndex.erase(entry.descriptor);
                    delete entry.resource;
                    entry.resource = nullptr;
                    entry.descriptor = ResourceDescriptor<T>();
                    entry.nextFree = m_FreeListHead;
                    m_FreeListHead = index;
                }
            }
            unsigned int AllocateEntry() {
                if (m_FreeListHead != 0)
                {
                    unsigned int index = m_FreeListHead;
                    m_FreeListHead = m_Entries[index].nextFree;
                    return index;
                }
                unsigned int index = static_cast<unsigned int>(m_Entries.size());
                m_Entries.push_back(ResourceEntry());
                return index;
            }

        public:
            ResourceManager() : m_FreeListHead(0)
            {
                m_Entries.reserve(256);
                m_Entries.push_back(ResourceEntry());
            }
            ~ResourceManager() {}
            ResourceManager(const ResourceManager&) = delete;
            ResourceManager& operator=(const ResourceManager&) = delete;
            ResourceHandle<T> GetResource(const ResourceDescriptor<T>& descriptor)
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                auto descriptorIterator = m_DescriptorToIndex.find(descriptor);
                if (descriptorIterator != m_DescriptorToIndex.end())
                {
                    unsigned int index = descriptorIterator->second;
                    ResourceEntry& entry = m_Entries[index];
                    if (!entry.IsActive()) throw std::runtime_error("Entry should be active if in descriptor map");
                    entry.referenceCount++;
                    return ResourceHandle<T>(this, index);
                }
                std::unique_ptr<T> resource = std::make_unique<T>(descriptor);
                unsigned int index = AllocateEntry();
                ResourceEntry& entry = m_Entries[index];
                entry.resource = resource.release();
                entry.referenceCount = 1;
                entry.descriptor = descriptor;
                m_DescriptorToIndex[descriptor] = index;
                return ResourceHandle<T>(this, index);
            }
            unsigned int GetResourceCount() const
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                return m_DescriptorToIndex.size();
            }
    };
}
