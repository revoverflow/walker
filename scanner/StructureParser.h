#pragma once

#include <fstream>

#include "Scanner.h"
#include "ScanUtils.h"

#include "../lib/json.h"

class StructureParser {
public:
    explicit StructureParser(char* filename);
    ~StructureParser();

    std::vector<ScannerField> parse();
private:
    char* filename;
};