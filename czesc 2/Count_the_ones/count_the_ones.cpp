#pragma warning(disable : 4996)
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string get_letter(uint8_t number) {
    uint8_t mask = 0x80;
    uint8_t ones = 0;
    string word;

    for (int i = 0; i < 8; i++) {
        if ((number & mask) > 0) {
            ones++;
        }
        mask = mask >> 1;
    }

    switch (ones)
    {
    case 0:
        word = "A";
        break;
    case 1:
        word = "A";
        break;
    case 2:
        word = "A";
        break;
    case 3:
        word = "B";
        break;
    case 4:
        word = "C";
        break;
    case 5:
        word = "D";
        break;
    case 6:
        word = "E";
        break;
    case 7:
        word = "E";
        break;
    case 8:
        word = "E";
        break;
    default:
        break;
    }

    return word;
}

int main()
{
    fstream input;
    ofstream output;
    input.open("plik.txt");
    output.open("output.txt", ios_base::app);
    string number;
    uint32_t counter = 0;
    string word[5];
    
    // catching first word
    int k = 0;
    while (k < 5) {
        getline(input, number);
        word[k] = get_letter(stoi(number));
        k++;
    }
    for (int i = 0; i < 5; i++) {
        output << word[i];
    }
    output << endl;
    
    for (int i = 0; i < 4; i++) {
        word[i] = word[i + 1];
    }
    
    while (counter < 255999) {
        getline(input, number);
        word[4] =  get_letter(stoi(number));
        for (int i = 0; i < 5; i++) {
            output << word[i];
        }
        output << endl;
        for (int i = 0; i < 4; i++) {
            word[i] = word[i + 1];
        }
        counter++;
        cout << counter << endl;
    }

    input.close();
    output.close();

    return 0;
}