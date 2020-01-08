//  Author: James T. Sipowicz III
//  Assignment: HW 4
//  Date: 11/25/19
//  Class: CS-3339
//  Instructor: Dan Tamir

// Name space Declaration
using namespace std;

// Import Packages
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

// Classes
class CacheLine{
    public:
        CacheLine();
        CacheLine(long* cacheTag, bool isValid);
        void setTag(long* cacheTag);
        void setValid(bool isValid);
        long* getTag();
        bool getValid();
    private:
        long* cacheTag;
        bool isValid;
};
// CacheLine Implementation
CacheLine::CacheLine(){
    CacheLine(new long(0), false);
}
CacheLine::CacheLine(long *cacheTag, bool isValid){
    setTag(cacheTag);
    setValid(isValid);
}
void CacheLine::setTag(long *cacheTag){
    this->cacheTag = cacheTag;
}
void CacheLine::setValid(bool isValid){
    this->isValid = isValid;
}
long* CacheLine::getTag(){
    return cacheTag;
}
bool CacheLine::getValid(){
    return isValid;
}
class CacheTable{
    public:
        CacheTable(int cacheTag, int index, int offset);
        void read(long instruction);
        void print();
        void printTable();
    private:
        CacheLine* table;
        int indexRange;
        int offsetBits;
        int numberOfHits;
        int numberOfCompulsoryMisses;
        int numberOfMisses;
        void fillTable();
};
// CacheTable Implementation
CacheTable::CacheTable(int cacheTag, int index, int offset){
    indexRange = pow(2, index);
    offsetBits = pow(2, offset);
    numberOfHits = 0;
    numberOfCompulsoryMisses = 0;
    numberOfMisses = 0;
    table = new CacheLine[indexRange];
    fillTable();
}
void CacheTable::read(long currentInstruction){
    long cacheTag = (currentInstruction / offsetBits / indexRange);
    int index = (int) ((currentInstruction / offsetBits) % cacheTag);
    if(cacheTag == *table[index].getTag()){
        numberOfHits++;
    }
    else{
        if(table[index].getValid() == true){
            numberOfMisses++;
        }
        else{
            numberOfCompulsoryMisses++;
            table[index].setValid(true);
        }
        table[index].setTag(new long(cacheTag));
    }
}
void CacheTable::print(){
    double total = numberOfHits + numberOfCompulsoryMisses + numberOfMisses;
    cout << "# of Hits: " << numberOfHits << endl;
    cout << "# of Compulsory Misses: " << numberOfCompulsoryMisses << endl;
    cout << "# of Misses: " << numberOfMisses << endl;
    cout << endl;
    cout << "Overall Hit ratio: " << (numberOfHits / total) << endl;
    cout << "Overall Compulsory Miss ratio: " << (numberOfCompulsoryMisses / total) << endl;
}
void CacheTable::fillTable(){
    for(int i = 0; i < indexRange; i++){
        table[i].setTag(new long(0));
        table[i].setValid(false);
    }
}
void CacheTable::printTable(){
    for(int i = 0; i < indexRange; i++){
        cout << i + 1 << " " << *table[i].getTag() << endl;
    }
}
long* hexidecimalToLong(string* hex){
    long* num = new long();
    istringstream iss(*hex);
    iss >> std::hex >> *num;
    return num;
}
// Instances
long* hexidecimalToLong(string* hex);
CacheTable Cache1(16, 4, 4);
CacheTable Cache2(15, 5, 4);

// Main Method
int main(){
    ifstream f;
    f.open("Trace.txt");
    if(f.is_open()){
        string* hexLine = new string("");
        while(f){
            getline(f, *hexLine);
            if(hexLine->empty() == false){
                Cache1.read(*hexidecimalToLong(hexLine));
                Cache2.read(*hexidecimalToLong(hexLine));
            }
        }
    }
    cout << "Welcome to my C++ Cache Program" << endl;
    cout << endl;
    cout << "Simulation 1: n = 24, m = 4, l = 4" << endl;
    Cache1.print();
    cout << endl;
    cout << "Simulation 2: n = 24, m = 4, l = 5" << endl;
    Cache2.print();
    cout << endl;
    cout << "Implemented by: James T. Sipowicz III" << endl;
    return -1;
}
