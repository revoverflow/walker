#include "StructureParser.h"

using json = nlohmann::json;

StructureParser::StructureParser(char *filename) {
    this->filename = filename;
}

StructureParser::~StructureParser() {
    filename = nullptr;
}

std::vector<ScannerField> StructureParser::parse() {
    std::ifstream file(filename);
    std::vector<ScannerField> fields{};

    if (!file.is_open()) {
        printf("Failed to open file: %s\n", filename);
        return fields;
    }

    json data = json::parse(file);

    // start parsing
    for (json::iterator it = data.begin(); it != data.end(); ++it) {
        json field = it.value();
        bool isValueSet = true;

        std::string type = field["type"];
        std::string criteria = field["criteria"];
        json value = field["value"];

        if (value.empty()) {
            isValueSet = false;
        }

        ScannerPrimitive primitive = ScanUtils::getPrimitiveByName(type);
        ScannerCriteriaType criteriaType = ScanUtils::getCriteriaByName(criteria, isValueSet);

        if (primitive == SCANNER_PRIMITIVE_NONE) {
            printf("Invalid primitive type: %s, ignoring field.\n", type.c_str());
            continue;
        }

        void* valuePtr = ScanUtils::castAsPrimitiveType(value, primitive);

        ScannerField c = {
            .primitive = primitive,
            .criteria = {
                .type = criteriaType,
                .value = valuePtr
            }
        };

        fields.push_back(c);
    }

    return fields;
}

