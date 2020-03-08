#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "statistica.h"
using namespace std;

struct contenitore_statistico
{
    double posizione_inizio = 0;
    double posizione_fine = 0;
    double posizione_intermedia = 0;
    double tempo_intermedio = 0;
    double media_tempo = 0;
    double dstd_tempo = 0;
    double dstd_tempo_media = 0;
    double media_vel = 0;
    double dstd_vel = 0;
    double dsts_vel_media = 0;
};

int main()
{
    string angolo = "../Dati/SecondaParte/";
    string prefix; //modifica se vuoi cambiare il set di dati
    cout << "Inserisci prefisso file (nm_na_ nm_sa_ sm_na_): ";
    cin >> prefix;
    string extension = ".txt";
    const int numero_intervalli = 6;                              //quanti intervalli di spazio ci sono per ogni set
    vector<contenitore_statistico> tempi(numero_intervalli);      //storage di tutte le info dei tempi dai file a stessa angolazione
    vector<contenitore_statistico> intervalli(numero_intervalli); //storage intervalli di tempo per ciascuno spazio e di velocita con relativi errori
    double spazio = 0.2;                                          //spazio fra due rilevatori
    double dstd_spazio = 0.001 / sqrt(6);                         //sigma spazio

    for (int i = 0; i < numero_intervalli; i++)
    {
        ifstream fin(angolo + prefix + to_string(40 + 10 * i) + "_" + to_string(60 + 10 * i) + extension);
        if (!fin)
        {
            cout << "Errore lettura " << angolo + prefix + to_string(40 + 10 * i) + "_" + to_string(60 + 10 * i) + extension << endl;
        }

        double temp;              //ausiliaria per il fin
        vector<double> temp_time; //vettore solo per leggere dai file
        while (fin >> temp)
        {
            temp_time.push_back(temp);
        }
        tempi[i].posizione_inizio = 40 + 10 * i; //tanto per tenere traccia di cosa cavolo stiamo facendo
        tempi[i].posizione_fine = 60 + 10 * i;   //dove finisce líntervallo di spazio
        tempi[i].posizione_intermedia = (tempi[i].posizione_inizio + tempi[i].posizione_fine) / 2;
        tempi[i].media_tempo = media(temp_time);
        tempi[i].dstd_tempo = dstd(temp_time);
        tempi[i].dstd_tempo_media = dstd_media(temp_time);
        tempi[i].media_vel = spazio / tempi[i].media_tempo;
        tempi[i].dstd_vel =sqrt((2*(1/pow(tempi[i].posizione_intermedia,2))*pow(dstd_spazio,2))+(0.2*0.2*(1/pow(tempi[i].posizione_intermedia,4))*(pow(tempi[i].dstd_tempo,2))));

  
    }

    ofstream fdati("../Stat2/dati_grafico_" + prefix + ".txt");
    ofstream fchi("../Stat2/chi_" + prefix + ".txt");
    if (!fdati)
    {
        cout << "Errore scrittura";
        return 1;
    }

    if (!fchi)
    {
        cout << "Errore scrittura";
        return 1;
    }

    vector<double> posizione_x;
    vector<double> vel_y;

    //stampare dati per grafico
    cout << "DATI PER GRAFICO CON ERRORI VERTICALI" << endl;
    for (auto d : tempi)
    {
        posizione_x.push_back(d.posizione_intermedia);
        vel_y.push_back(d.media_vel);
        cout << d.posizione_intermedia << "\t" << d.media_vel << "\t" << d.dstd_vel << endl;
        fdati << d.posizione_intermedia << "\t" << d.media_vel << "\t" << d.dstd_vel << endl;
    }

    //stampare risultato chi quadro
    cout << "ERRORI DI CHI QUADRO" << endl;
    cout << "A_intercetta: " << a_intercetta(posizione_x, vel_y) << "SIGMA_A: " << sigma_a(posizione_x, vel_y) << endl;
    cout << "B_Angolare: " << b_angolare(posizione_x, vel_y) << "SIGMA_B: " << sigma_b(posizione_x, vel_y) << endl;
    fchi << a_intercetta(posizione_x, vel_y) << "\t" << sigma_a(posizione_x, vel_y) << endl;
    fchi << b_angolare(posizione_x, vel_y) << "\t" << sigma_b(posizione_x, vel_y) << endl;

    //aggiungi tutti i valori ad un file solo... DA eseguire solo una volta...
    ofstream error_file;
    error_file.open("../Stat2/file_errori.txt", std::ios_base::app);
    error_file << b_angolare(posizione_x, vel_y) << "\t" << sigma_b(posizione_x, vel_y) << endl;

}
