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
    SCANNER_PRIMITIVE_POINTER,
    SCANNER_PRIMITIVE_BYTES,
    SCANNER_PRIMITIVE_STRING
} ScannerPrimitive;

typedef enum {
    SCANNER_CRITERIA_NONE,
    SCANNER_CRITERIA_ANY,
    SCANNER_CRITERIA_EQUAL,
    SCANNER_CRITERIA_NOT_EQUAL,
    SCANNER_CRITERIA_GREATER_THAN,
    SCANNER_CRITERIA_LESS_THAN,
    SCANNER_CRITERIA_GREATER_THAN_OR_EQUAL,
    SCANNER_CRITERIA_LESS_THAN_OR_EQUAL,
    SCANNER_CRITERIA_PTR_NOTNULL,
    SCANNER_CRITERIA_PTR_NULL,
    SCANNER_CRITERIA_BYTES_MATCH,
    SCANNER_CRITERIA_BYTES_NOT_MATCH
} ScannerCriteriaType;

struct ScannerCriteria {
    ScannerCriteriaType type;
    void* value;
};

struct ScannerField {
    ScannerPrimitive primitive;
    std::vector<ScannerCriteria> criterias;
    size_t size = 0;
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

// List of all supported bytes primitives
const std::vector<ScannerPrimitive> BYTES_PRIMITIVES = {
    SCANNER_PRIMITIVE_BYTES
};

// Primitives that support the equal criteria
const std::vector<ScannerPrimitive> EQ_SUPPORTED_PRIM = {
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
        SCANNER_PRIMITIVE_STRING
};

// Details about each primitive
// { primitive, size, name, sizeDynamic }
const std::vector<std::tuple<ScannerPrimitive, size_t, std::string, bool>> PRIM_DETAILS = {
    { SCANNER_PRIMITIVE_NONE, 0, "none", false },
    { SCANNER_PRIMITIVE_UINT8, sizeof(uint8_t), "uint8", false },
    { SCANNER_PRIMITIVE_UINT16, sizeof(uint16_t), "uint16", false },
    { SCANNER_PRIMITIVE_UINT32, sizeof(uint32_t), "uint32", false },
    { SCANNER_PRIMITIVE_UINT64, sizeof(uint64_t), "uint64", false },
    { SCANNER_PRIMITIVE_INT8, sizeof(int8_t), "int8", false },
    { SCANNER_PRIMITIVE_INT16, sizeof(int16_t), "int16", false },
    { SCANNER_PRIMITIVE_INT32, sizeof(int32_t), "int32", false },
    { SCANNER_PRIMITIVE_INT64, sizeof(int64_t), "int64", false },
    { SCANNER_PRIMITIVE_FLOAT, sizeof(float), "float", false },
    { SCANNER_PRIMITIVE_DOUBLE, sizeof(double), "double", false },
    { SCANNER_PRIMITIVE_POINTER, sizeof(void*), "pointer", false },
    { SCANNER_PRIMITIVE_BYTES, 0, "bytes", true },
    { SCANNER_PRIMITIVE_STRING, 0, "string", true }
};

// Details about criteria
// { criteria, names, supportedPrimitives, valueDynamic }
const std::vector<std::tuple<ScannerCriteriaType, std::vector<std::string>, std::vector<ScannerPrimitive>, bool>> CRIT_DETAILS = {
    { SCANNER_CRITERIA_ANY, { "any" }, {}, false },
    { SCANNER_CRITERIA_EQUAL, { "equals", "eq", "=", "==", "===" }, EQ_SUPPORTED_PRIM, true },
    { SCANNER_CRITERIA_NOT_EQUAL, { "not_equals", "neq", "!=", "!==" }, EQ_SUPPORTED_PRIM, true },
    { SCANNER_CRITERIA_GREATER_THAN, { "greater_than", "gt", ">" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_LESS_THAN, { "less_than", "lt", "<" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_GREATER_THAN_OR_EQUAL, { "greater_than_or_equal", "gte", ">=" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_LESS_THAN_OR_EQUAL, { "less_than_or_equal", "lte", "<=" }, NUMERIC_PRIMITIVES, true },
    { SCANNER_CRITERIA_PTR_NOTNULL, { "ptr_not_null", "notnullptr", "!nullptr", "!= nullptr" }, POINTER_PRIMITIVES, false },
    { SCANNER_CRITERIA_PTR_NULL, { "ptr_null", "nullptr" }, POINTER_PRIMITIVES, false },
    { SCANNER_CRITERIA_BYTES_MATCH, { "match", "pattern", "?" }, BYTES_PRIMITIVES, true },
    { SCANNER_CRITERIA_BYTES_NOT_MATCH, { "not_match", "!pattern", "!?" }, BYTES_PRIMITIVES, true }
};


class ScanUtils {
public:
    static bool matchesField(void* buffer, ScannerField field);

    static size_t getPrimitiveSize(ScannerPrimitive primitive);
    static size_t calculateStructureSize(const std::vector<ScannerField>& fields);

    static ScannerPrimitive getPrimitiveByName(const std::string& name, bool isSizeSet);
    static ScannerCriteriaType getCriteriaByName(const std::string& name, bool isValueSet);

    static void* castAsPrimitiveType(const json& value, ScannerPrimitive primitive);

    static bool comparePattern(void* buffer, const std::string& pattern, size_t maxSize);

    static bool isPrimitiveSizeSet(ScannerPrimitive primitive);

private:
    static bool matchesCriteria(void* buffer, ScannerCriteria criteria, ScannerPrimitive primitive, size_t size);
    static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
    static bool isHex(const std::string& str);
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
            case SCANNER_CRITERIA_ANY:
                return true;
            default:
                return false;
        }
    }

    static bool pointer(T value, ScannerCriteria criteria) {
        switch (criteria.type) {
            case SCANNER_CRITERIA_EQUAL:
                return value == *(T*) criteria.value;
            case SCANNER_CRITERIA_PTR_NOTNULL:
                return value != (T) nullptr;
            case SCANNER_CRITERIA_PTR_NULL:
                return value == (T) nullptr;
            case SCANNER_CRITERIA_ANY:
                return true;
            default:
                return false;
        }
    }

    static bool bytes(T value, ScannerCriteria criteria, size_t size) {
        switch (criteria.type) {
            case SCANNER_CRITERIA_BYTES_MATCH:
                return ScanUtils::comparePattern(value, *(std::string*) criteria.value, size);
            case SCANNER_CRITERIA_BYTES_NOT_MATCH:
                return !ScanUtils::comparePattern(value, *(std::string*) criteria.value, size);
            case SCANNER_CRITERIA_ANY:
                return true;
            default:
                return false;
        }
    }

    static bool string(T value, ScannerCriteria criteria, size_t size) {
        switch (criteria.type) {
            case SCANNER_CRITERIA_EQUAL:
                return memcmp(value, criteria.value, size) == 0;
            case SCANNER_CRITERIA_NOT_EQUAL:
                return memcmp(value, criteria.value, size) != 0;
            case SCANNER_CRITERIA_ANY:
                return true;
            default:
                return false;
        }
    }
};