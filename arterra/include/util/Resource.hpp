#pragma once

#include "PCH.hpp"
#include "util/DataObject.hpp"

namespace arterra {

	class ResourceManager {

		struct Data : public DataObject {

			// Resource properties.
			uint32_t handleCount = 0;
			std::vector<uint8_t> _data;

			Data() = default;

			// ctor
			Data(std::vector<uint8_t> data) { _data = data; }

			// Log the resource data.
			void DumpToLog(const std::string &title = "Resource.Data") const override
			{
				// Write handle count and data size.
				Logger::Log("\t", title, " - ", "handleCount: ", handleCount, "; Data size:", _data.size());
			}

			// Serialise resource data.
			std::vector<uint8_t> Serialize() override { return _data; }
		};

		struct Handle : public DataObject {

			// Pointer to the resource.
			Data* _resource;

			// ctor
			Handle(Data* resource)
			{
				if (!resource)
					return;
				// Increase the resource use since there is a new pointer
				// pointing to it.
				_resource = resource;
				_resource->handleCount += 1;
			}

			// dtor
			~Handle()
			{
				// Decrease the resource use since a pointer pointing to
				// it is getting deleted.
				_resource->handleCount -= 1;
			}

			// Log the resource data handle.
			void DumpToLog(const std::string &title = "Resource.Handle") const override
			{
				Logger::Log("\n\t", title, " - ", "handleCount: ", _resource);
			}
		};

	public:
		// ctor
		ResourceManager();

		// Singleton getter
		static ResourceManager& Get();

		// Load a specific resource.
		bool Load(std::string name);
		// Regularly unload any resources which aren't in use.
		// Simple garbage collection.
		void Unload();
		// Get a resource handle, which has a pointer to the resource
		// and marks the resource for unload if its not in use anymore.
		Handle GetHandle(std::string name);

	private:
		// Loaded resources,
		// string = path to file (name), ResourceData = data for this resource.
		std::unordered_map<std::string, Data> _resources;
		// Memory footprint of all loaded resources in bytes.
		uint64_t _memoryUsage;
	};

}