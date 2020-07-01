#pragma once

#include "PCH.hpp"

namespace arterra {
	
	struct ResourceData : public DataObject {
		
		// Resource properties
		uint32_t handleCount = 0;
		std::vector<uint8_t> _data;
		
		ResourceData() = default;
		
		// ctor
		ResourceData(std::vector<uint8_t> data) {
			_data = data;
		}

		// DataObject overrides
		void DumpToLog(std::string title = "ResourceData") override {
			// Write handle count and data size
			Logger::Get().Log(
				"\t", title, 
				" - ", "handleCount: ", handleCount, 
				"; Data size:", _data.size()
			);
		}

		std::vector<uint8_t> Serialize() override {
			return _data;
		}
		
	};
	
	struct ResourceHandle : public DataObject {
		
		// Pointer to the resource
		ResourceData* _resource;
		
		// ctor
		ResourceHandle(ResourceData* resource) {
			// Increase the resource use since there is a new pointer
			// pointing to it
			_resource = resource;
			_resource->handleCount += 1;
		}
		
		// dtor
		~ResourceHandle() {
			// Decrease the resource use since a pointer pointing to
			// it is getting deleted
			_resource->handleCount -= 1;
		}
		
		// Dump the resource handle
		void DumpToLog(std::string title = "ResourceHandle") override {
			Logger::Get().Log(
				"\n\t", title, 
				" - ", "handleCount: ", _resource
			);
		}

	};
	
	class Resource {
		
		public:
			// ctor
			Resource();
			
			// Singleton getter
			static Resource& Get();
			
			// Load a specific resource
			bool Load(std::string name);
			// Regularly unload any resources which aren't in use
			// Simple garbage collection
			void Unload();
			// Get a resource handle, which has a pointer to the resource
			// and marks the resource for unload if its not in use anymore
			ResourceHandle Get(std::string name);
			
		private:
			// Loaded resources
			// string = path to file (name), ResourceData = data for this resource
			std::unordered_map<std::string, ResourceData> _resources;
		
		
	};
	
}