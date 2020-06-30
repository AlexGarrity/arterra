#pragma once

#include "PCH.hpp"

namespace arterra {
	
	struct ResourceData {
		
		// Resource properties
		int handleCount = 0;
		std::vector<char> _data;
		
		ResourceData() = default;
		
		// ctor
		ResourceData(std::vector<char> data) {
			_data = data;
		}
		
	};
	
	struct ResourceHandle {
		
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