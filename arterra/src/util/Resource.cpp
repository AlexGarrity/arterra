#include <util/Resource.hpp>

namespace arterra {
	
	Resource::Resource() {
		Logger::Get().Log(Logger::Debug, "Resource Manager Initialised");
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
			Logger::Get().Log(Logger::Error, "Resource '", name, "' is already loaded");
			return false;
		}
		// Get the full path to the resource
		std::string path = (std::filesystem::current_path()/ "res" / name).string();
		if (!std::filesystem::exists(path)) return false;
		// Open the file
		std::vector<uint8_t> data;
		std::ifstream file;
		try
		{
			file.open(path, std::ios::binary);
		}
		catch(const std::exception& e)
		{
			Logger::Get().Log(Logger::Error, "Could not load resource '", name, "'");
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
		_memoryUsage += _resources[name]._data.size();

		Logger::Get().Log(
			Logger::Debug, "Loaded a new resource '", name, "'; size=", _resources[name]._data.size(), " Bytes"
		);
		
		return true;
	}
	
	void Resource::Unload() {
		// No resources to check
		if (_resources.empty()) return;
		Logger::Get().Log(Logger::Debug, "Running automatic resource unloading");
		std::vector<std::unordered_map<std::string, ResourceData>::iterator> unloadObjects;
		// Iterate all resources currently loaded
		for (auto i = _resources.begin(); i != _resources.end(); i++) {
			// Check that it's not in use before unloading it
			if (i->second.handleCount == 0) {
				unloadObjects.push_back(i);
			}
		}
		// No objects to unload
		if (unloadObjects.empty()) return;
		Logger::Get().Log(
			Logger::Debug, "Unloading ", unloadObjects.size(), " resource", (unloadObjects.size() == 1)?"":"s"
		);
		// Get memory usage before running
		auto memory = _memoryUsage;
		for (auto o : unloadObjects) {
			Logger::Get().Log(Logger::Debug, "Unloading a resource '", o->first, "'");
			_memoryUsage -= o->second._data.size();
			_resources.erase(o);
		}
		// Log memory saving
		Logger::Get().Log(
			Logger::Debug, "Freed ", memory - _memoryUsage, " Bytes (", memory, "B => ", _memoryUsage, "B)"
		);
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