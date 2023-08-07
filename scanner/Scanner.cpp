#include "Scanner.h"

Scanner::Scanner() {
    buffer = nullptr;
    fields = std::vector<ScannerField>{};
}

Scanner::~Scanner() {
    delete[] buffer;
    fields.clear();
}

void Scanner::addField(ScannerField field) {
    fields.push_back(field);
}

void Scanner::setBuffer(char* inputBuffer, size_t inputBufferSize) {
    this->bufferSize = inputBufferSize;
    this->buffer = new char[bufferSize];
    memcpy(this->buffer, inputBuffer, bufferSize);
}

std::vector<ScannerResult> Scanner::scan() {
    std::vector<ScannerResult> results;

    if (buffer == nullptr) return results;
    if (fields.empty()) return results;

    size_t structureSize = ScanUtils::calculateStructureSize(fields);

    for (size_t i = 0; i < bufferSize - structureSize; i++) {
        size_t offset = 0;

        for (const auto& field : fields) {
            if (ScanUtils::matchesField(buffer + i + offset, field)) {
                if (ScanUtils::isPrimitiveSizeSet(field.primitive)) {
                    offset += field.size;
                } else {
                    offset += ScanUtils::getPrimitiveSize(field.primitive);
                }
            } else {
                break;
            }
        }

        if (offset == structureSize) {
            results.push_back(ScannerResult{ structureSize, i, buffer + i });
        }
    }

    return results;
}

void Scanner::setFields(std::vector<ScannerField> inputFields) {
    this->fields = std::move(inputFields);
}

void Scanner::saveResults(const std::vector<ScannerResult>& results, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    file << "~ walker scan results ~" << std::endl;
    file << "found " << results.size() << " results" << std::endl;
    file << "------------------------" << std::endl;

    for (ScannerResult result : results) {
        file << "0x" << std::hex << result.offset << std::endl;
    }

    file << "------------------------" << std::endl;

    file.close();
}
