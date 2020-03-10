#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "statistica.h"
using namespace std;

double prop_g_0(double a, double sigma_a_x, double alfa, double sigma_alfa);

int main()
{
    string filename = "../Stat/a_x.txt";
    string outputfile = "../Stat/g_0.txt";
    double a_x_, sigma_a_x_;
    vector<double> a_x;
    vector<double> sigma_a_x;
    double sigma_alfa_rad = 3.029e-05;//da misurazioni inizali
    double g_0, sigma_g_0_rad;
    double angolo_deg, angolo_rad;
    double comp_pd;
    double g_pd = 9.806;
    double sigma_g_pd = 0.001;
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Errore lettura";
        return 1;
    }
    ofstream fout(outputfile);
    if (!fout)
    {
        cout << "Errore scrittura";
        return 1;
    }
    while (fin >> a_x_ >> sigma_a_x_) //legge il file e archivia in vettori e siamo sicuri di questi perche escono da chi quadro
    {
        a_x.push_back(a_x_);
        sigma_a_x.push_back(sigma_a_x_);
    }

    //15'
    angolo_deg = 0.25;                    //convertito in decimali
    angolo_rad = angolo_deg * M_PI / 180; //convertito in radianti
    g_0 = a_x[0] / sin(angolo_rad);       //g_0 da a_x dividendo per il seno dell'angolo
                                          //propagazione senza approssimazione di angolo (sin x = x)
    sigma_g_0_rad = prop_g_0(a_x[0], sigma_a_x[0], angolo_rad, sigma_alfa_rad);
    comp_pd = comp_3(g_0, g_pd, sigma_g_0_rad, sigma_g_pd);
    cout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;
    fout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;

    //30'
    angolo_deg = 0.50;
    angolo_rad = angolo_deg * M_PI / 180;
    g_0 = a_x[1] / sin(angolo_rad);
    sigma_g_0_rad = prop_g_0(a_x[1], sigma_a_x[1], angolo_rad, sigma_alfa_rad);
    comp_pd = comp_3(g_0, g_pd, sigma_g_0_rad, sigma_g_pd);
    cout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;
    fout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;

    //45'
    angolo_deg = 0.75;
    angolo_rad = angolo_deg * M_PI / 180;
    g_0 = a_x[2] / sin(angolo_rad);
    sigma_g_0_rad = prop_g_0(a_x[2], sigma_a_x[2], angolo_rad, sigma_alfa_rad);
    comp_pd = comp_3(g_0, g_pd, sigma_g_0_rad, sigma_g_pd);
    cout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;
    fout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;

    //45'p
    angolo_deg = 0.75;
    angolo_rad = angolo_deg * M_PI / 180;
    g_0 = a_x[3] / sin(angolo_rad);
    sigma_g_0_rad = prop_g_0(a_x[3], sigma_a_x[3], angolo_rad, sigma_alfa_rad);
    comp_pd = comp_3(g_0, g_pd, sigma_g_0_rad, sigma_g_pd);
    cout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;
    fout << angolo_deg << "\t" << g_0 << "\t" << sigma_g_0_rad << "\t" << comp_pd << endl;

    return 0;
}

double prop_g_0(double a, double sigma_a_x, double alfa, double sigma_alfa)
{
    double sig;
    sig = sqrt(pow((sigma_a_x / sin(alfa)), 2) + pow(((a * cos(alfa) * sigma_alfa) / (pow(sin(alfa), 2))), 2));
    return sig;
}
