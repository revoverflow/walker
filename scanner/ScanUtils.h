#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "../lib/json.h"

using json = nlohmann::json;

typedef enum {
    SCANNER_PRIMITIVE_NONE,
    SCANNER_PRIMITIVE_UINT8,
    SCANNER_PRIMITIVE_UINT16,
    SCANNER_PRIMITIVE_UINT32,
    SCANNER_PRIMITIVE_UINT64,
    SCANNER_PRIMITIVE_INT8,
    SCANNER_PRIMITIVE_INT16,
    SCANNER_PRIMITIVE_INT32,
    SCANNER_PRIMITIVE_INT64,
    SCANNER_PRIMITIVE_FLOAT,
    SCANNER_PRIMITIVE_DOUBLE,
    SCANNER_PRIMITIVE_STRING,
    SCANNER_PRIMITIVE_POINTER
} ScannerPrimitive;

typedef enum {
    SCANNER_CRITERIA_NONE,
    SCANNER_CRITERIA_EQUAL,
    SCANNER_CRITERIA_NOT_EQUAL,
    SCANNER_CRITERIA_GREATER_THAN,
    SCANNER_CRITERIA_LESS_THAN,
    SCANNER_CRITERIA_GREATER_THAN_OR_EQUAL,
    SCANNER_CRITERIA_LESS_THAN_OR_EQUAL,
    SCANNER_CRITERIA_PTR_NOTNULL,
    SCANNER_CRITERIA_PTR_NULL
} ScannerCriteriaType;

struct ScannerCriteria {
    ScannerCriteriaType type;
    void* value;
};

struct ScannerField {
    ScannerPrimitive primitive;
    ScannerCriteria criteria;
};

struct ScannerResult {
    size_t valueSize;
    size_t offset;
    void* value;
};

// List of all supported numeric primitives
const std::vector<ScannerPrimitive> NUMERIC_PRIMITIVES = {
    SCANNER_PRIMITIVE_UINT8,
    SCANNER_PRIMITIVE_UINT16,
    SCANNER_PRIMITIVE_UINT32,
    SCANNER_PRIMITIVE_UINT64,
    SCANNER_PRIMITIVE_INT8,
    SCANNER_PRIMITIVE_INT16,
    SCANNER_PRIMITIVE_INT32,
    SCANNER_PRIMITIVE_INT64,
    SCANNER_PRIMITIVE_FLOAT,
    SCANNER_PRIMITIVE_DOUBLE
};

// List of all supported pointer primitives
const std::vector<ScannerPrimitive> POINTER_PRIMITIVES = {
    SCANNER_PRIMITIVE_POINTER
};

// Details about each primitive
const std::vector<std::tuple<ScannerPrimitive, size_t, std::string>> PRIM_DETAILS = {
    { SCANNER_PRIMITIVE_NONE, 0, "none" },
    { SCANNER_PRIMITIVE_UINT8, sizeof(uint8_t), "uint8" },
    { SCANNER_PRIMITIVE_UINT16, sizeof(uint16_t), "uint16" },
    { SCANNER_PRIMITIVE_UINT32, sizeof(uint32_t), "uint32" },
    { SCANNER_PRIMITIVE_UINT64, sizeof(uint64_t), "uint64" },
    { SCANNER_PRIMITIVE_INT8, sizeof(int8_t), "int8" },
    { SCANNER_PRIMITIVE_INT16, sizeof(int16_t), "int16" },
    { SCANNER_PRIMITIVE_INT32, sizeof(int32_t), "int32" },
    { SCANNER_PRIMITIVE_INT64, sizeof(int64_t), "int64" },
    { SCANNER_PRIMITIVE_FLOAT, sizeof(float), "float" },
    { SCANNER_PRIMITIVE_DOUBLE, sizeof(double), "double" },
    { SCANNER_PRIMITIVE_POINTER, sizeof(void*), "pointer" }
};

// Details about criteria
const std::vector<std::tuple<ScannerCriteriaType, std::vector<std::string>, std::vector<ScannerPrimitive>, bool>> CRIT_DETAILS = {
    { SCANNER_CRITERIA_EQUAL, { "equals", "=", "==", "===" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_NOT_EQUAL, { "not_equals", "!=", "!==" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_GREATER_THAN, { "greater_than", ">" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_LESS_THAN, { "less_than", "<" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_GREATER_THAN_OR_EQUAL, { "greater_than_or_equal", ">=" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_LESS_THAN_OR_EQUAL, { "less_than_or_equal", "<=" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_PTR_NOTNULL, { "ptr_not_null", "!nullptr", "!= nullptr" }, POINTER_PRIMITIVES, false },
    { SCANNER_CRITERIA_PTR_NULL, { "ptr_null", "nullptr" }, POINTER_PRIMITIVES, false }
};

class ScanUtils {
public:
    static bool matchesField(void* buffer, ScannerField field);

    static size_t getPrimitiveSize(ScannerPrimitive primitive);
    static size_t calculateStructureSize(const std::vector<ScannerField>& fields);

    static ScannerPrimitive getPrimitiveByName(const std::string& name);
    static ScannerCriteriaType getCriteriaByName(const std::string& name, bool isValueSet);

    static void* castAsPrimitiveType(const json& value, ScannerPrimitive primitive);
};

template<typename T>
struct CriteriaMatcher {
    static bool numeric(T value, ScannerCriteria criteria) {
        switch (criteria.type) {
            case SCANNER_CRITERIA_EQUAL:
                return value == *(T*) criteria.value;
            case SCANNER_CRITERIA_NOT_EQUAL:
                return value != *(T*) criteria.value;
            case SCANNER_CRITERIA_GREATER_THAN:
                return value > *(T*) criteria.value;
            case SCANNER_CRITERIA_LESS_THAN:
                return value < *(T*) criteria.value;
            case SCANNER_CRITERIA_GREATER_THAN_OR_EQUAL:
                return value >= *(T*) criteria.value;
            case SCANNER_CRITERIA_LESS_THAN_OR_EQUAL:
                return value <= *(T*) criteria.value;
            default:
                return false;
        }
    }

    static bool pointer(T value, ScannerCriteria criteria) {
        switch (criteria.type) {
            case SCANNER_CRITERIA_EQUAL:
                return value == *(T*) criteria.value;
            case SCANNER_CRITERIA_PTR_NOTNULL:
                return value != nullptr;
            case SCANNER_CRITERIA_PTR_NULL:
                return value == nullptr;
            default:
                return false;
        }
    }
};