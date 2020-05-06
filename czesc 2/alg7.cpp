#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

int b2l(char byte)
{
    register int cnt1s = 0, tmp, j;

    for (j = 0; j < 8; ++j) {
        tmp = (byte >> j) & 1;
        if (tmp == 1) ++cnt1s;
    }

    if (cnt1s < 3) return 0;

    switch (cnt1s) {
    case 3: return 1;
    case 4: return 2;
    case 5: return 3;
    default: break;
    }

    return 4;
}

float count_the_ones(char Tab[])
{
    float prob[] = { 37 / 256.,56 / 256.,70 / 256.,56 / 256.,37 / 256. };
    int wd;
    int i;
    short j, s;
    int f4[625], f5[3125];
    int* f = f4;
    float Ef, chsq = 0, z;
    char byte;


    for (i = 0; i < 625; ++i) {
        f4[i] = 0;
        f5[i] = 0;
    }

    for (i = 625; i < 3125; ++i) {
        f5[i] = 0;
    }

    wd = (625 * b2l(Tab[0]) + 125 * b2l(Tab[1]) + 25 * b2l(Tab[2]) + 5 * b2l(Tab[3]) + b2l(Tab[4]));

    for (i = 1; i < 256000; ++i) {
        wd %= 625;                      
        ++f4[wd];

        wd = 5 * wd + b2l(Tab[i+4]);              
        ++f5[wd];
    }

    for (s = 4; s <= 5; ++s) {
        int ltrspwd, wdspos;
        int ltr;

        switch (s) {
        case 4: wdspos = 625; f = f4; ltrspwd = 4; break;
        case 5: wdspos = 3125; f = f5; chsq = -chsq; ltrspwd = 5; break;
        }

        for (i = 0; i < wdspos; ++i) {
            Ef = 256000;
            wd = i;

            for (j = 0; j < ltrspwd; ++j) {
                ltr = wd % 5;
                Ef *= prob[ltr];
                wd /= 5;
            }

            chsq += (*(f + i) - Ef) * (*(f + i) - Ef) / Ef;
        }
    }

    return chsq;
}



int main()
{
    fstream plikin;
    fstream plikout;

    plikin.open("plik.txt", ios::in);
    plikout.open("plikout.txt", ios::out);

   

    int liczba_testow = 187;
    string line;
    unsigned int value;
    char Tab[256004];

    for (int j = 0; j < liczba_testow; j++)
    {
        int iterator = 0;
        for (int i = 0; i < 64001; i++)
        {
            getline(plikin, line);
            value = stoul(line);
            
            unsigned int mask = 0x80000000;

            for (int k = 0; k < 4; k++)
            {
                int liczba = 0;
                for (int n = 7; n >= 0; n--)
                {
                    if ((mask & value) > 0)
                        liczba = liczba + pow(2, n);
                    mask = mask >> 1;
                }
                Tab[iterator] = liczba;
                iterator++;
            }
        }
        
        plikout << (count_the_ones(Tab) - 2500.0)/70.71 <<endl;

    }
}

