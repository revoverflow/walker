#include <iostream>

#include "scanner/Scanner.h"
#include "scanner/StructureParser.h"

int main() {
    Scanner scanner {};
    StructureParser parser {"test.json"};

    scanner.setFields(parser.parse());
    scanner.setBuffer(R"(test!!!!!!!!test!!!!!!!!)", 24);

    std::vector<ScannerResult> results = scanner.scan();

    std::cout << "[+] Found " << results.size() << " results." << std::endl;

    for (ScannerResult result : results) {
        std::cout << "- 0x" << std::hex << result.offset << std::endl;
    }

    return 0;
}
