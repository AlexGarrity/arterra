#include <util/Resource.hpp>

namespace arterra {

	ResourceManager::ResourceManager() { Logger::Debug( "Resource Manager Initialised"); }

	ResourceManager& ResourceManager::Get()
	{
		// Create a local static resource.
		static ResourceManager resource;
		// Return the resource as a reference (can't be null).
		return resource;
	}

	bool ResourceManager::Load(std::string name)
	{
		// Only load a specific resource once into memory.
		if (_resources.find(name) != _resources.end()) {
			Logger::Error("Resource '", name, "' is already loaded");
			return false;
		}
		// Get the full filepath to the resource.
		std::string path = (std::filesystem::current_path() / "res" / name).string();
		if (!std::filesystem::exists(path)) {
			Logger::Error("Requested file '", name, "' does not exist");
			return false;
		}
		// Open the file if it exists.
		std::vector<uint8_t> data;
		std::ifstream file;
		try {
			file.open(path, std::ios::binary);
		} catch (const std::exception& e) {
			Logger::Error("Could not load resource '", name, "'");
			Logger::Debug(e.what());
			return false;
		}

		// Stop eating new lines in binary mode.
		file.unsetf(std::ios::skipws);

		// Get the filesize to allocate enough memory in the vector.
		std::streampos fileSize;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// Reserve capacity for the vector.
		data.reserve(fileSize);

		// Read the data in.
		data.insert(data.begin(), std::istream_iterator<char>(file), std::istream_iterator<char>());

		// Create new resource data and add it to the list.
		_resources[name] = Data { data };
		_memoryUsage += _resources[name]._data.size();

		Logger::Debug("Loaded a new resource '", name, "'; size=", _resources[name]._data.size(), " Bytes");

		return true;
	}

	void ResourceManager::Unload()
	{
		// No resources to check, to return early.
		if (_resources.empty())
			return;

		Logger::Debug( "Running automatic resource unloading");
		std::vector<std::unordered_map<std::string, Data>::iterator> unloadObjects;
		// Iterate all currently loaded resources.
		for (auto i = _resources.begin(); i != _resources.end(); i++) {
			// Check that it's not in use before unloading it.
			if (i->second.handleCount == 0) {
				unloadObjects.push_back(i);
			}
		}

		// No objects to unload so return.
		if (unloadObjects.empty())
			return;
		Logger::Debug( "Unloading ", unloadObjects.size(), " resource",
			(unloadObjects.size() == 1) ? "" : "s");

		// Get memory usage before freeing memory.
		auto memory = _memoryUsage;
		// Unload objects which are set to be unloaded.
		for (auto o : unloadObjects) {
			Logger::Debug( "Unloading a resource '", o->first, "'");
			_memoryUsage -= o->second._data.size();
			_resources.erase(o);
		}
		// Log memory saving statistic.
		Logger::Debug("Freed ", memory - _memoryUsage, " Bytes (", memory, "B => ", _memoryUsage, "B)");
	}

	ResourceManager::Handle ResourceManager::GetHandle(std::string name)
	{
		// Check that the resource exists.
		auto resource = _resources.find(name);
		if (resource != _resources.end()) {
			return { &resource->second };
		}
		return { nullptr };
	}

}