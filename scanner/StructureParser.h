#pragma once

#include <fstream>

#include "Scanner.h"
#include "ScanUtils.h"

#include "../lib/json.h"

class StructureParser {
public:
    explicit StructureParser(std::string filename);
    std::vector<ScannerField> parse();
private:
    std::string filename;
};