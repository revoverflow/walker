#include "ScanUtils.h"

bool ScanUtils::matchesField(void *buffer, ScannerField field) {
    ScannerPrimitive primitive = field.primitive;
    bool matches = true;

    for (const auto& criteria : field.criterias) {
        matches &= ScanUtils::matchesCriteria(buffer, criteria, primitive, field.size);
        if (!matches) break;
    }

    return matches;
}

size_t ScanUtils::getPrimitiveSize(ScannerPrimitive primitive) {
    return std::get<size_t>(PRIM_DETAILS[primitive]);
}

size_t ScanUtils::calculateStructureSize(const std::vector<ScannerField>& fields) {
    size_t size = 0;

    for (const ScannerField& field : fields) {
        if(ScanUtils::isPrimitiveSizeSet(field.primitive)) {
            size += field.size;
        } else {
            size += ScanUtils::getPrimitiveSize(field.primitive);
        }
    }

    return size;
}

ScannerPrimitive ScanUtils::getPrimitiveByName(const std::string& name, bool isSizeSet) {
    for (const auto & i : PRIM_DETAILS) {
        if (std::get<std::string>(i) == name && std::get<bool>(i) == isSizeSet) {
            return std::get<ScannerPrimitive>(i);
        }
    }

    return SCANNER_PRIMITIVE_NONE;
}

ScannerCriteriaType ScanUtils::getCriteriaByName(const std::string& name, bool isValueSet) {
    for (const auto & i : CRIT_DETAILS) {
        std::vector<std::string> keywords = std::get<std::vector<std::string>>(i);
        bool isValueNeeded = std::get<bool>(i);

        if (isValueNeeded != isValueSet) continue;

        for (const std::string& keyword : keywords) {
            if (keyword == name) {
                return std::get<ScannerCriteriaType>(i);
            }
        }
    }

    return SCANNER_CRITERIA_NONE;
}

void *ScanUtils::castAsPrimitiveType(const json& value, ScannerPrimitive primitive) {
    switch (primitive) {
        case SCANNER_PRIMITIVE_UINT8:
            return (void*) new uint8_t(value.get<uint8_t>());
        case SCANNER_PRIMITIVE_UINT16:
            return (void*) new uint16_t(value.get<uint16_t>());
        case SCANNER_PRIMITIVE_UINT32:
            return (void*) new uint32_t(value.get<uint32_t>());
        case SCANNER_PRIMITIVE_UINT64:
            return (void*) new uint64_t(value.get<uint64_t>());
        case SCANNER_PRIMITIVE_INT8:
            return (void*) new int8_t(value.get<int8_t>());
        case SCANNER_PRIMITIVE_INT16:
            return (void*) new int16_t(value.get<int16_t>());
        case SCANNER_PRIMITIVE_INT32:
            return (void*) new int32_t(value.get<int32_t>());
        case SCANNER_PRIMITIVE_INT64:
            return (void*) new int64_t(value.get<int64_t>());
        case SCANNER_PRIMITIVE_FLOAT:
            return (void*) new float(value.get<float>());
        case SCANNER_PRIMITIVE_DOUBLE:
            return (void*) new double(value.get<double>());
        case SCANNER_PRIMITIVE_POINTER:
            return (void*) new uintptr_t(value.get<uintptr_t>());
        case SCANNER_PRIMITIVE_BYTES:
            return (void*) new std::string(value.get<std::string>());
        case SCANNER_PRIMITIVE_STRING: {
            char *str = new char[value.get<std::string>().length() + 1];
            strcpy(str, value.get<std::string>().c_str());
            return (char *) str;
        }
        case SCANNER_PRIMITIVE_NONE:
            break;
    }

    return nullptr;
}

bool ScanUtils::matchesCriteria(void *buffer, ScannerCriteria criteria, ScannerPrimitive primitive, size_t size) {
    switch (primitive) {
        case SCANNER_PRIMITIVE_UINT8:
            return CriteriaMatcher<uint8_t>::numeric(*(uint8_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_UINT16:
            return CriteriaMatcher<uint16_t>::numeric(*(uint16_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_UINT32:
            return CriteriaMatcher<uint32_t>::numeric(*(uint32_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_UINT64:
            return CriteriaMatcher<uint64_t>::numeric(*(uint64_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_INT8:
            return CriteriaMatcher<int8_t>::numeric(*(int8_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_INT16:
            return CriteriaMatcher<int16_t>::numeric(*(int16_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_INT32:
            return CriteriaMatcher<int32_t>::numeric(*(int32_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_INT64:
            return CriteriaMatcher<int64_t>::numeric(*(int64_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_FLOAT:
            return CriteriaMatcher<float>::numeric(*(float*) buffer, criteria);
        case SCANNER_PRIMITIVE_DOUBLE:
            return CriteriaMatcher<double>::numeric(*(double*) buffer, criteria);
        case SCANNER_PRIMITIVE_POINTER:
            return CriteriaMatcher<uintptr_t>::pointer(*(uintptr_t*) buffer, criteria);
        case SCANNER_PRIMITIVE_BYTES:
            return CriteriaMatcher<void*>::bytes(buffer, criteria, size);
        case SCANNER_PRIMITIVE_STRING: {
            return CriteriaMatcher<char *>::string((char *) buffer, criteria, size);
        }
        case SCANNER_PRIMITIVE_NONE:
            break;
    }

    return false;
}

bool ScanUtils::comparePattern(void *buffer, const std::string& pattern, size_t size) {
    // This function is used to compare a buffer to a pattern, e.g. to check if a buffer matches a signature
    // pattern is an IDA-style pattern, e.g. "A1 ? ? ? ? 8B 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
    std::vector<std::string> tokens = ScanUtils::splitString(pattern, " ");
    size_t patternSize = tokens.size();

    if (patternSize != size) {
        printf("Pattern size mismatch: %zu != %zu\n", patternSize, size);
        return false;
    }

    for (size_t i = 0; i < patternSize; i++) {
        std::string token = tokens[i];

        if (token == "?" || token == "??") continue;
        if (!ScanUtils::isHex(token) || token.size() > 2) return false;

        auto byte = (uint8_t) std::stoi(token, nullptr, 16);
        auto bufferByte = *(uint8_t*) ((uintptr_t) buffer + i);

        if (byte != bufferByte) return false;
    }

    return true;
}

std::vector<std::string> ScanUtils::splitString(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type lastPos = 0, pos = 0;

    while ((pos = str.find(delimiter, pos)) != std::string::npos) {
        std::string token = str.substr(lastPos, pos - lastPos);
        tokens.push_back(token);
        lastPos = ++pos;
    }

    tokens.push_back(str.substr(lastPos, pos - lastPos));

    return tokens;
}

bool ScanUtils::isHex(const std::string& str) {
    return std::all_of(str.begin(), str.end(), ::isxdigit);
}

bool ScanUtils::isPrimitiveSizeSet(ScannerPrimitive primitive) {
    for (const auto & i : PRIM_DETAILS) {
        if (std::get<ScannerPrimitive>(i) == primitive) {
            return std::get<bool>(i);
        }
    }

    return false;
}

