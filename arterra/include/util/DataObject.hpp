#pragma once

#include "PCH.hpp"

namespace arterra {
    
    class DataObject {
        public:
        // Dump entire object to console
        virtual void DumpToLog(std::string title = "--- OBJECT ---") { Logger::Get().Log(Logger::Debug, title); };
        // Serialise entire object
        virtual std::vector<uint8_t> Serialize() { return {}; };
    };

}