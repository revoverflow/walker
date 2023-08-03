#ifndef WALKER_SCANNER_H
#define WALKER_SCANNER_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <utility>

#include "ScanUtils.h"

class Scanner {
public:
    Scanner();
    ~Scanner();

    void addField(ScannerField field);

    void setFields(std::vector<ScannerField> inputFields);
    void setBuffer(char* inputBuffer, size_t bufferSize);

    std::vector<ScannerResult> scan();
private:
    std::vector<ScannerField> fields;

    size_t bufferSize;
    char* buffer;
};


#endif //WALKER_SCANNER_H
