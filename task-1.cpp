#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iomanip>
using namespace std;

// Register mapping
unordered_map<std::string, int> registers = {
    {"A", 0b000},
    {"B", 0b001},
    {"C", 0b010},
    {"D", 0b011},
    {"E", 0b100}
};

// Opcodes mapping
unordered_map<string, int> opcodes = {
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
string intToHex(int value, int width = 2) {
    stringstream ss;
    ss << setfill('0') << setw(width) << hex << value;
    return ss.str();
}

// Parse and process the command
vector<string> parseCommand(const string &command) {
    vector<string> tokens;
    istringstream stream(command);
    string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to process the ADD and MOVE instructions
string processInstruction(const vector<string>& tokens) {
    string instruction = tokens[0];
    if (opcodes.find(instruction) == opcodes.end()) {
        return "Invalid Instruction";
    }

    int opcode = opcodes[instruction];
    if (instruction == "ADD" || instruction == "MOVE") {
        if (tokens.size() != 3) return "Invalid Instruction Format";

        string destReg = tokens[1];
        string srcOrVal = tokens[2];

        if (registers.find(destReg) == registers.end()) return "Invalid Destination Register";

        int destRegCode = registers[destReg];
        int instrCode;

        // ADD/MOVE with register
        if (registers.find(srcOrVal) != registers.end()) {
            int srcRegCode = registers[srcOrVal];
            instrCode = (opcode << 6) | (destRegCode << 3) | srcRegCode;
            return intToHex(instrCode);
        } else { // ADD/MOVE with value
            int value = stoi(srcOrVal);
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
    string input;
    cout << "Enter assembly command: ";
    getline(cin, input);

    vector<string> tokens = parseCommand(input);
    if (tokens.empty()) {
        cerr << "Invalid command" << endl;
        return 1;
    }

    string result = processInstruction(tokens);
    cout << "Hexadecimal: " << result << endl;

    return 0;
}
