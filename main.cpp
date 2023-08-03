#include <iostream>
#include <utility>

#include "lib/argparse.h"

#include "scanner/Scanner.h"
#include "scanner/StructureParser.h"

std::pair<long, char*> file_read(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];

    if (file.read(buffer, size)) return std::pair<long, char*>{size, buffer};
    return std::pair<long, char*>{-1, nullptr};
}

void scan_file(const std::string& targetFilePath, std::string structureFilePath, const std::string& outputFilePath) {
    Scanner scanner {};
    StructureParser structureParser {std::move(structureFilePath)};
    std::pair<long, char*> targetBuffer = file_read(targetFilePath);

    if (targetBuffer.first == -1) {
        std::cout << "[-] Failed to read file." << std::endl;
        return;
    }

    scanner.setFields(structureParser.parse());
    scanner.setBuffer(targetBuffer.second, targetBuffer.first);

    std::vector<ScannerResult> results = scanner.scan();
    std::cout << "* Found " << results.size() << " results." << std::endl;

    Scanner::saveResults(results, outputFilePath);
    std::cout << "* Results saved in " << outputFilePath << "." << std::endl;
}

int main(int argc, char** argv) {
    argparse::Parser parser;

    auto filename = parser.AddArg<std::string>("filename", 'f', "The file to scan.");
    auto structure = parser.AddArg<std::string>("structure", 's', "The structure JSON file to search.");
    auto output = parser.AddArg<std::string>("output", 'o', "The output file to write results to.");

    parser.ParseArgs(argc, argv);

    if (filename && structure) {
        std::string outputFilePath = "output.txt";

        if (!output) {
            std::cout << "* No output file specified, using default output.txt" << std::endl;
        } else {
            outputFilePath = *output;
        }

        scan_file(*filename, *structure, outputFilePath);
    } else {
        std::cout << "Usage: " << argv[0] << " -f <filename> -s <structure> -o [output]" << std::endl;
    }

    return 0;
}
