#include "ScanUtils.h"

bool ScanUtils::matchesField(void *buffer, ScannerField field) {
    ScannerPrimitive primitive = field.primitive;

    switch (primitive) {
        case SCANNER_PRIMITIVE_UINT8:
            return CriteriaMatcher<uint8_t>::numeric(*(uint8_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_UINT16:
            return CriteriaMatcher<uint16_t>::numeric(*(uint16_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_UINT32:
            return CriteriaMatcher<uint32_t>::numeric(*(uint32_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_UINT64:
            return CriteriaMatcher<uint64_t>::numeric(*(uint64_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_INT8:
            return CriteriaMatcher<int8_t>::numeric(*(int8_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_INT16:
            return CriteriaMatcher<int16_t>::numeric(*(int16_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_INT32:
            return CriteriaMatcher<int32_t>::numeric(*(int32_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_INT64:
            return CriteriaMatcher<int64_t>::numeric(*(int64_t*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_FLOAT:
            return CriteriaMatcher<float>::numeric(*(float*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_DOUBLE:
            return CriteriaMatcher<double>::numeric(*(double*) buffer, field.criteria);
        case SCANNER_PRIMITIVE_POINTER:
            return false;
        case SCANNER_PRIMITIVE_NONE:
            break;
    }

    return false;
}

size_t ScanUtils::getPrimitiveSize(ScannerPrimitive primitive) {
    return std::get<size_t>(PRIM_DETAILS[primitive]);
}

size_t ScanUtils::calculateStructureSize(const std::vector<ScannerField>& fields) {
    size_t size = 0;

    for (ScannerField field : fields) {
        size += getPrimitiveSize(field.primitive);
    }

    return size;
}

ScannerPrimitive ScanUtils::getPrimitiveByName(const std::string& name) {
    for (const auto & i : PRIM_DETAILS) {
        if (std::get<std::string>(i) == name) {
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
        case SCANNER_PRIMITIVE_NONE:
            break;
    }

    return nullptr;
}

