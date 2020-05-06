
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>



using namespace std;

class Place
{
public:

    double x = 0.0;
    double y = 0.0;
};



int main()
{
    srand(time(0));
    fstream plikin1;
    fstream plikin2;
    fstream plikout;

    plikin1.open("plik1.txt", ios::in);
    plikin2.open("plik2.txt", ios::in);
    plikout.open("plikout.txt", ios::out);

    string line;
    unsigned int value;
    double Randoms1[12000];
    unsigned int Tab1[12000];
    double Randoms2[12000];
    unsigned int Tab2[12000];
    int size = 500;

    unsigned int maks1 = 0, maks2 = 0;
    double ratio1 = 0, ratio2 = 0;

    for (int j = 0; j < size; j++)
    {
        Place Park[12000];
        int k = 0;

        maks1 = 0;
        maks2 = 0;

        for (int i = 0; i < 12000; i++)
        {
            getline(plikin1, line);
            value = stoul(line);
            Tab1[i] = value;

            if (value > maks1)
                maks1 = value;

            getline(plikin2, line);
            value = stoul(line);
            Tab2[i] = value;

            if (value > maks2)
                maks2 = value;
        }

        ratio1 = 100.0 / maks1;
        ratio2 = 100.0 / maks2;

        for (int i = 0; i < 12000; i++)
        {
            Randoms1[i] = Tab1[i] * ratio1;
            Randoms2[i] = Tab2[i] * ratio2;
        }

        k++;

        Park[0].x = Randoms1[0];
        Park[0].y = Randoms2[0];

        for (int n = 1; n < 12000; n++)
        {
            bool collision = 0;
            double x = Randoms1[n];
            double y = Randoms2[n];


            for (int i = 0; i < k; i++)
            {
                if ((fabs(Park[i].x - x) <= 1.0) && (fabs(Park[i].y - y) <= 1.0))
                {
                    collision = 1;
                    break;
                }
            }

            if (!collision)
            {
                Park[k].x = x;
                Park[k].y = y;
                collision = 0;
                k++;
            }
        }
        double z = (k - 3523) / 21.9;

        plikout << z << endl;
    }
}

