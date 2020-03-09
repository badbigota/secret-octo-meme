#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "statistica.h"
using namespace std;

int main()
{
    string massa;
    cout << "Massa? (si/no): ";
    cin >> massa;
    if (massa == "si")
    {
        //conti con la massa
        string b, sigma_coeff_b;
        //conti senza massa
        ifstream fin("../Stat2/sm_chi.txt");
        if (!fin)
        {
            cout << "Errore lettura";
            return 1;
        }
        fin >> b >> sigma_coeff_b;
        cout << "COEFF ATTRITO" << endl;
        cout << b << " +/- " << sigma_coeff_b << endl;
    }
    else if (massa == "no")
    {
        double b_, sigma_coeff_b_;
        vector<double> b;
        vector<double> sigma_coeff_b;

        //conti senza massa
        ifstream fin("../Stat2/nm_chi.txt");
        if (!fin)
        {
            cout << "Errore lettura";
            return 1;
        }
        while (fin >> b_ >> sigma_coeff_b_)
        {
            b.push_back(b_);
            sigma_coeff_b.push_back(sigma_coeff_b_);
        }
        cout << "COEFF ATTRITO" << endl;
        cout << media_ponderata(b, sigma_coeff_b) << " +/- ";
        cout << errore_v_vero(sigma_coeff_b) << endl;
    }

    return 0;
}
