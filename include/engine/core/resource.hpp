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

// #pragma once
// #include <stdexcept>
// #include <unordered_map>

// namespace Engine
// {
//     class Resource
//     {
//         public:
//             virtual ~Resource() = default;
//     };

//     template <typename T>
//     concept ResourceConcept = std::is_base_of_v<Resource, T>;

//     template<typename T>
//     struct ResourceDescriptor
//     {
//         bool operator==(const ResourceDescriptor&) const = default;
//     };

//     template <ResourceConcept R>
//     class ResourceHandle
//     {
//         private:
//             unsigned int m_ID = 0;

//         public:
//             static ResourceHandle INVALID_RESOURCE_HANDLE;
//             ResourceHandle() = default;
//             ResourceHandle(unsigned int id) : m_ID(id) {}
//             bool operator==(const ResourceHandle& other) const { return m_ID == other.m_ID; }
//             bool IsValid() const { return m_ID != 0; }
//             unsigned int GetID() const { return m_ID; }
//     };

//     template <ResourceConcept R>
//     ResourceHandle<R> ResourceHandle<R>::INVALID_RESOURCE_HANDLE{ 0 };

//     template <ResourceConcept R>
//     class ResourceManager
//     {
//         struct ResourceEntry
//         {
//             R resource;
//             unsigned int referenceCount = 0;
//         };

//         private:
//             unsigned int m_NextID = 1;
//             std::unordered_map<ResourceDescriptor<R>, ResourceHandle<R>> m_ResourceHandles;
//             std::unordered_map<ResourceHandle<R>, ResourceEntry> m_Resources;

//         public:
//             ResourceManager() = default;
//             ~ResourceManager() { for (auto& [handle, entry] : m_Resources) entry.resource.Unload(); }
//             ResourceHandle<R> GetHandle(const ResourceDescriptor<R>& description)
//             {
//                 auto iterator = m_ResourceHandles.find(description);
//                 if (iterator != m_ResourceHandles.end())
//                 {
//                     auto handle = iterator->second;
//                     m_Resources[handle].referenceCount++;
//                     return handle;
//                 }

//                 ResourceHandle<R> handle(m_NextID++);

//                 ResourceEntry entry;
//                 entry.referenceCount = 1;

//                 if (!entry.resource.Load(description))
//                     throw std::runtime_error("Failed to load resource.");

//                 m_Resources.emplace(handle, std::move(entry));
//                 m_ResourceHandles.emplace(description, handle);

//                 return handle;
//             }

//             const R& Get(const ResourceHandle<R>& handle) const
//             {
//                 auto iterator = m_Resources.find(handle);
//                 if (iterator == m_Resources.end())
//                     throw std::runtime_error("Invalid resource handle");

//                 return iterator->second.resource;
//             }

//             R& Get(const ResourceHandle<R>& handle)
//             {
//                 auto iterator = m_Resources.find(handle);
//                 if (iterator == m_Resources.end())
//                     throw std::runtime_error("Invalid resource handle");

//                 return iterator->second.resource;
//             }

//             void Release(const ResourceHandle<R>& handle)
//             {
//                 auto iterator = m_Resources.find(handle);
//                 if (iterator == m_Resources.end())
//                     return;

//                 auto& entry = iterator->second;
//                 if (--entry.referenceCount == 0)
//                 {
//                     entry.resource.Unload();

//                     for (auto it = m_ResourceHandles.begin(); it != m_ResourceHandles.end(); ++it)
//                     {
//                         if (it->second == handle)
//                         {
//                             m_ResourceHandles.erase(it);
//                             break;
//                         }
//                     }

//                     m_Resources.erase(iterator);
//                 }
//             }
//     };
// }

// namespace std
// {
//     template <Engine::ResourceConcept R>
//     struct hash<Engine::ResourceHandle<R>>
//     {
//         size_t operator()(const Engine::ResourceHandle<R>& handle) const noexcept
//         {
//             return std::hash<unsigned int>{}(handle.GetID());
//         }
//     };
// }
