#pragma once

#include "PCH.hpp"

namespace arterra {

	class DataObject {

	public:
		// Dump the entire object to the console.
		virtual void DumpToLog(std::string title = "--- OBJECT ---") { Logger::Get().Log(Logger::Debug, title); };
		// Serialise the entire object.
		virtual std::vector<uint8_t> Serialize() { return {}; };
	};

}