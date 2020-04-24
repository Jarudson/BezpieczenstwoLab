#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main()
{
    fstream plikin;
    fstream plikout;

    plikin.open("plik.txt", ios::in);
    plikout.open("plikout.txt", ios::out);

    //int size = 996352;
   // int size = 1000;
    int size = 100000;
    unsigned int * Randoms = new unsigned int[size];
    float* Tab = new float[size];
    float* Sums = new float[size-100];

    string line;
    unsigned int value;
    

    for (int i = 0; i < size; i++)
    {
        getline(plikin, line);
        value = stoul(line);
        Randoms[i] = value;

        Tab[i] = (value % 100000) / 100000.0;
    }
    
    for (int i = 0; i < (size-100); i++)
    {
        float sum = 0;
        for (int j = i; j < (i+100); j++)
        {
            sum = sum + Tab[j];
        }
        Sums[i] = sum;
        Sums[i] = Sums[i] - 50.0;
        plikout << Sums[i] << endl;
    }


}

