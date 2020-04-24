#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void word_to_number(int size, string Tab[], int W[])
{
    for (int i = 0; i < size; i++)
    {
        int val = 0;
        int x = 0;

        x = (int)Tab[i][0] - 65;
        val = val + (x * 625);

        x = (int)Tab[i][1] - 65;
        val = val + (x * 125);

        x = (int)Tab[i][2] - 65;
        val = val + (x * 25);

        x = (int)Tab[i][3] - 65;
        val = val + (x * 5);

        x = (int)Tab[i][4] - 65;
        val = val + x;

        W[i] = val;
    }
}

void number_to_word(string Tab[], string WN[])
{
    int i = 0;
    char X[5] = { 0 };

    for (int a = 0; a < 5; a++)
    {
        for (int b = 0; b < 5; b++)
         {
            for (int c = 0; c < 5; c++)
            {
                for (int d = 0; d < 5; d++)
                {
                    for (int e = 0; e < 5; e++)
                    {
                        X[0] = (char) (a + 65);
                        X[1] = (char) (b + 65);
                        X[2] = (char) (c + 65);
                        X[3] = (char) (d + 65);
                        X[4] = (char) (e + 65);

                        WN[i] = X[0];
                        WN[i] = WN[i] + X[1];
                        WN[i] = WN[i] + X[2];
                        WN[i] = WN[i] + X[3];
                        WN[i] = WN[i] + X[4];

                        WN[i] = WN[i] + " = " + Tab[i];
                        i++;
                    }
                }
            }
        }
    }
}


int main()
{
    fstream words;
    fstream numbersofwords;
    fstream numbersdensity;
    fstream wordsdensity;

    words.open("words.txt", ios::in);
    numbersofwords.open("number_of_words.txt", ios::out);
    numbersdensity.open("numbers_density.txt", ios::in);
   // wordsdensity.open("words_density.txt", ios::out);
  
    string line;
    int size = 256000;

    string * Tab = new string[size];
    string* WN = new string[size];
    int* W = new int[size];
    
   
        for (int i = 0; i < size; i++)
        {
            getline(words, line);
            Tab[i] = line;
        }

        word_to_number(size, Tab, W);
       
       for (int i = 0; i < (size); i++)
       {
            if (i == (size / 2))
                numbersofwords << endl;

            numbersofwords << W[i] << ", ";
       }
  /*
    for (int i = 0; i < size; i++)
    {
        getline(numbersdensity, line);
        Tab[i] = line;
    }

    number_to_word(Tab, WN);

    for (int i = 0; i < size; i++)
    {
        wordsdensity << WN[i] << endl;
    }*/
}

