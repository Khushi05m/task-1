#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

// Register mapping
std::unordered_map<std::string, int> registers = {
    {"A", 0b000},
    {"B", 0b001},
    {"C", 0b010},
    {"D", 0b011},
    {"E", 0b100}
};

// Opcodes mapping
std::unordered_map<std::string, int> opcodes = {
    {"ADD", 0b00},
    {"MOVE", 0b01},
    {"STORE", 0b01},
    {"LOAD", 0b01},
    {"JMP", 0b10000000},
    {"JPZ", 0b10000001},
    {"JPP", 0b10000010},
    {"JPN", 0b10000011}
};

// Helper function to convert int to hex
std::string intToHex(int value, int width = 2) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::hex << value;
    return ss.str();
}

// Parse and process the command
std::vector<std::string> parseCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::istringstream stream(command);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to process the ADD and MOVE instructions
std::string processInstruction(const std::vector<std::string>& tokens) {
    std::string instruction = tokens[0];
    if (opcodes.find(instruction) == opcodes.end()) {
        return "Invalid Instruction";
    }

    int opcode = opcodes[instruction];
    if (instruction == "ADD" || instruction == "MOVE") {
        if (tokens.size() != 3) return "Invalid Instruction Format";

        std::string destReg = tokens[1];
        std::string srcOrVal = tokens[2];

        if (registers.find(destReg) == registers.end()) return "Invalid Destination Register";

        int destRegCode = registers[destReg];
        int instrCode;

        // ADD/MOVE with register
        if (registers.find(srcOrVal) != registers.end()) {
            int srcRegCode = registers[srcOrVal];
            instrCode = (opcode << 6) | (destRegCode << 3) | srcRegCode;
            return intToHex(instrCode);
        } else { // ADD/MOVE with value
            int value = std::stoi(srcOrVal);
            instrCode = (opcode << 6) | (destRegCode << 3) | 0b101;
            return intToHex(instrCode) + " " + intToHex(value);
        }
    } else if (instruction == "STORE" || instruction == "LOAD") {
        // Handle STORE or LOAD with addresses and registers
        // Similar processing would follow here
    } else if (instruction == "JMP" || instruction == "JPZ" || instruction == "JPP" || instruction == "JPN") {
        // Handle jump instructions with extension words
    }

    return "Unknown error";
}

int main() {
    std::string input;
    std::cout << "Enter assembly command: ";
    std::getline(std::cin, input);

    std::vector<std::string> tokens = parseCommand(input);
    if (tokens.empty()) {
        std::cerr << "Invalid command" << std::endl;
        return 1;
    }

    std::string result = processInstruction(tokens);
    std::cout << "Hexadecimal: " << result << std::endl;

    return 0;
}
