#include <util/Resource.hpp>

namespace arterra {
	
	Resource::Resource() {
		Logger::Get().Log("Resource Manager Initialised", Logger::Severity::Debug);
	}
	
	Resource &Resource::Get() {
		// Create a local static resource
		static Resource resource;
		// Return the resource as a reference (can't be null)
		return resource;
	}
	
	bool Resource::Load(std::string name) {
		// Only load a specific resource once
		if (_resources.find(name) != _resources.end()) {
			std::string msg = "Resource: " + name + " is already loaded";
			Logger::Get().Log(msg.data(), Logger::Severity::Error);
			return false;
		}
		// Get the full path to the resource
		std::string path = (std::filesystem::current_path()/ "res" / name).string();
		// Open the file
		std::vector<char> data;
		std::ifstream file;
		try
		{
			file.open(path, std::ios::binary);
		}
		catch(const std::exception& e)
		{
			std::string msg = "Could not load resource: " + name;
			Logger::Get().Log(msg.data(), Logger::Severity::Error);
			return false;
		}
		
		// Stop eating new lines in binary mod
		file.unsetf(std::ios::skipws);
		
		// Get file size
		std::streampos fileSize;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		
		// Reserve capacity for vector
		data.reserve(fileSize);
		
		// Read the data
		data.insert(data.begin(), std::istream_iterator<char>(file), std::istream_iterator<char>());
		
		// Add data to the resource list
		_resources[name] = ResourceData { data };
		
		return true;
	}
	
	void Resource::Unload() {
		for (auto i = _resources.begin(); i != _resources.end(); i++) {
			// Check that it's not in use before unloading it
			if (i->second.handleCount == 0) {
				_resources.erase(i);
			}
		}
	}
	
	ResourceHandle Resource::Get(std::string name) {
		// Check that the resource exists
		auto resource = _resources.find(name);
		if (resource != _resources.end()) {
			return { &resource->second };
		}
		return { nullptr };
	}
	
}