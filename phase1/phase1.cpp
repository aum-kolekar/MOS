#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

//declaring memory and GP Registors
char MEMORY[100][4];
char IR[4];
char R[4];
int IC;
bool C;

void printMemory(){
    for (int i = 0; i < 100; i++)
    {
        cout << "M[" << setfill('0') << setw(2) << i << "]";

        for (int j = 0; j < 4; j++)
        {
            cout << MEMORY[i][j];
        }
        cout << endl;
    }
    
}

void initialiseMemory() {
    cout << "into initialiseMemory{}." << endl;
    //initialising MEMORY
    for (int i = 0; i < 100; i++){
        for (int j = 0; j < 4; j++){
            MEMORY[i][j] = ' ';
        }
    }
    cout << "memory done..." << endl;

    //initialising Instruction Registors
    for (int i = 0; i < 4; i++){
        IR[i] = ' ';
    }
    cout << "IR done..." << endl;
    
    //initialising Registors
    for (int i = 0; i < 4; i++){
        R[i] = ' ';
    }
    cout << "R done..." << endl;

    //initialising Instruction Counter
    IC = 0;
    cout << "IC done..." << endl;

    //initialising toggol 
    C = false;
    cout << "toggol done..." << endl;
}

void loadIntoMemory(string line) {
    int memIndex = 0;
    int lineIndex = 0;

    cout << "loading memory" << endl;

    while (lineIndex < line.size() && memIndex < 100)
    {
        for(int i=0 ; i<4 && lineIndex < line.size() ; i++){
            MEMORY[memIndex][i] = line[lineIndex++];
        }
        memIndex++;
    }
    
    cout << "program loaded" << endl;
}

void executeProgram(ifstream &inputFile , ofstream &outputFile) {
    int SI = 0;
    string data;

    cout << "executing job" << endl;
    while(true) {
        for(int i=0 ; i< 4 ; i++) {
            IR[i] = MEMORY[IC][i];
        }

        //decoding instructions
        string opcode = string(1, IR[0]) + string(1, IR[1]);
        int address = (IR[2] -'0')*10 + (IR[3] - '0');

        if (opcode == "GD") {
            SI = 1;
        }
        else if(opcode == "PD"){
            SI = 2;
        }
        else if (opcode == "H") {
            SI = 3;
        }
        else if(opcode == "LR") {
            for(int i=0 ; i<4 ; i++){
                R[i] = MEMORY[address][i];
            }
        }
        else if(opcode == "SR") {
            for(int i=0 ; i<4 ; i++){
                MEMORY[address][i] = R[i];
            }
        }
        else if(opcode == "CR"){
            C = true;
            for(int i=0 ; i<4 ; i++){
                if(R[i] != MEMORY[address][i]){
                    C = false;
                    break;
                }
            }
        }
        else if(opcode == "BT"){
            if(C){
                IC = address;
                continue;
            }
        }

        if(SI == 1){
            if(!getline(inputFile, data)){
                cout << "Error : no input data avable" << endl;
                return;
            }

            int memIndex = address;
            int lineIndex = 0;

            while (lineIndex < data.size() && memIndex < (address + 10)){
                for(int i=0 ; i<4 && lineIndex < data.size() ; i++){
                    MEMORY[memIndex][i] = data[lineIndex++];
                }
                memIndex++;
            }
            SI = 0;
        }
        else if(SI == 2) {
            for(int i=0 ; i<10 ; i++){
                for(int j=0 ; j<4 ; j++){
                    outputFile << MEMORY[address + i][j];
                }
                // outputFile << " ";
            }
            outputFile << endl;
            SI = 0;
            printMemory();
        }
        else if(SI == 3){
            outputFile << "\n" << endl;
            break;
        }
        
        IC++;
        
    }
    
}

void readFile(ifstream &inputFile, ofstream &outputFile) {
    cout << "into readFile{}." << endl;
    string line;
    bool flag = false;
    
    cout << "Reading file" << endl;

    while(getline(inputFile, line)){
        if(line.substr(0, 4) == "$AMJ"){
            cout << "initialising memory and other registors" << endl;
            cout << "going into initialMemory{}." << endl;
            initialiseMemory();
        }
        else if (line.substr(0, 4) == "$DTA") {
            flag = true;
            executeProgram(inputFile, outputFile);
        }
        else if (line.substr(0, 4) == "$END") {
            flag = false;
            break;
        }
        else{
            if(!flag){
            cout << "loading memory" << endl;
            loadIntoMemory(line);
            }
        }
    }
}



int main() {
    cout << "in main{}." << endl;
    //take input file
    ifstream inputFile ("job.txt");
    ofstream outputFile ("jobOutput.txt");

    //read file
    cout << "going into readFile{}." << endl;
    readFile(inputFile, outputFile);

    cout << "printing memory" << endl;
    printMemory();
    cout << "memory printed" << endl;
}