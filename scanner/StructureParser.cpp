#include "StructureParser.h"

#include <utility>

using json = nlohmann::json;

StructureParser::StructureParser(std::string filename) {
    this->filename = std::move(filename);
}

std::vector<ScannerField> StructureParser::parse() {
    std::ifstream file(filename);
    std::vector<ScannerField> fields{};

    if (!file.is_open()) {
        printf("Failed to open file: %s\n", filename.c_str());
        return fields;
    }

    json data = json::parse(file);

    // start parsing
    for (json::iterator it = data.begin(); it != data.end(); ++it) {
        json field = it.value();

        std::string type = field["type"];
        json size = field["size"];
        json criterias = field["criterias"];

        if (criterias.empty()) {
            printf("No criteria set for field: %s, ignoring field.\n", type.c_str());
            continue;
        } else if (!criterias.is_array()) {
            printf("Invalid criteria type for field: %s, ignoring field.\n", type.c_str());
            continue;
        }

        ScannerPrimitive primitive = ScanUtils::getPrimitiveByName(type, !size.empty());

        if (primitive == SCANNER_PRIMITIVE_NONE) {
            printf("Invalid primitive type: %s, ignoring field. For primitives without size, make sure you specified the size attribute.\n", type.c_str());
            continue;
        }

        std::vector<ScannerCriteria> criteriaList{};

        for(json::iterator itc = criterias.begin(); itc != criterias.end(); ++itc) {
            json criteria = itc.value();

            std::string name = criteria["type"];
            json value = criteria["value"];

            ScannerCriteriaType criteriaType = ScanUtils::getCriteriaByName(name, !value.empty());

            if (criteriaType == SCANNER_CRITERIA_NONE) {
                printf("Invalid criteria type: %s, ignoring criteria.\n", name.c_str());
                continue;
            }

            void* valuePtr = nullptr;
            if (!value.empty()) valuePtr = ScanUtils::castAsPrimitiveType(value, primitive);

            ScannerCriteria c = {
                    .type = criteriaType,
                    .value = valuePtr
            };

            criteriaList.push_back(c);
        }

        fields.push_back({
            .primitive = primitive,
            .criterias = criteriaList,
            .size = size.empty() ? 0 : size.get<size_t>()
        });
    }

    printf("Parsed %lu fields.\n", fields.size());

    return fields;
}

