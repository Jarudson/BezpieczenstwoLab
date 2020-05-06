#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;


int main()
{
    fstream plikin;
    fstream plikout;

    plikin.open("plik.txt", ios::in);
    plikout.open("plikout.txt", ios::out);

   
    int size = 12000000;
    unsigned int * Randoms = new unsigned int[size];
    float* Tab = new float[size];
    float* Sums = new float[11999900];

    string line;
    unsigned int value;
    unsigned int maks = 0;
    int iterator = 0;
    
    for (int i = 0; i < size; i++)
    {
        getline(plikin, line);
        value = stoul(line);
        if (value > maks)
            maks = value;
        Randoms[i] = value;

    }
    
    float ratio = 1.0 / maks;

    for (int i = 0; i < size; i++)
    {
        Tab[i] = Randoms[i] * ratio;
    }
    
    for (int i = 0; i < size - 100; i++)
    {
        float sum = 0;
        for (int j = i; j < (i+100); j++)
        {
            sum = sum + Tab[j];
        }
        Sums[iterator] = sum;

       double z = (sum - 50) / 0.2887;
      //  z = z / sqrt(2.0);
     //   z = 1 + erf(z);
      //  z = z / 2;
     //   plikout << 1 - z << endl;
          plikout << z  << endl;

        iterator++;
    }


}

