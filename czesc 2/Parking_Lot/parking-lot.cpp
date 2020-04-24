#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Place
{
public:

    double x = 0.0;
};



int main()
{
    fstream plikin;
    fstream plikout;

    plikin.open("plik.txt", ios::in);
    plikout.open("plikout.txt", ios::out);

    int K[830];
    string line;
    int value;
    int size = 12000;
    double Randoms[12000];
    double X[830];

    for (int j = 0; j < 830; j++)
    {
        Place Park[12000];
        int k = 0;

        for (int i = 0; i < size; i++)
        {
            getline(plikin, line);
            value = stoi(line);
            Randoms[i] = value / 100.0;
        }

        for (int n = 0; n < size; n++)
        {
            bool collision = 0;
            double x = Randoms[n];

            for (int i = 0; i < k; i++)
            {
                if ((fabs(Park[i].x - x) <= 1.0))
                {
                    collision = 1;
                    break;
                }
            }

            if (!collision)
            {
                Park[k].x = x;
                collision = 0;
                k++;
            }
        }
        K[j] = k;
        X[j] = (K[j] - 3523) / 21.9;
        
        cout << "busy" << endl;

        plikout << X[j] << endl;
    }
    cout << "Finish!" << endl;
}

