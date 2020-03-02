#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
    string angolo = "../Dati/PrimaParte/15_"; //modifica se vuoi cambiare l inclinazione da leggere
    string extension = ".txt";
    const int numero_intervalli = 7;                              //quanti intervalli di spazio ci sono per ogni angolazione
    vector<contenitore_statistico> tempi(numero_intervalli);      //storage di tutte le info dei tempi dai file a stessa angolazione
    vector<contenitore_statistico> intervalli(numero_intervalli); //storage intervalli di tempo per ciascuno spazio e di velocita con relativi errori
    double spazio = 0.1;                                          //spazio fra due rilevatori
    double dstd_spazio = 0.001 / sqrt(6);                         //da distribuz triangolare

    for (int i = 0; i < numero_intervalli; i++)
    {
        ifstream fin(angolo + to_string(50 + 10 * i) + extension); //soluzione per leggere in automatico tutti i file con stessa angolazione
        if (!fin)
        {
            cout << "Errore lettura " << angolo + to_string(50 + 10 * i) + extension << endl;
        }

        double temp;              //ausiliaria per il fin
        vector<double> temp_time; //vettore solo per leggere dai file
        while (fin >> temp)
        {
            temp_time.push_back(temp);
        }

        //tutte le info che Marc metteva in file
        tempi[i].posizione_inizio = 40;        //tanto per tenere traccia di cosa cavolo stiamo facendo
        tempi[i].posizione_fine = 50 + 10 * i; //dove finisce líntervallo di spazio
        tempi[i].media_tempo = media(temp_time);
        tempi[i].dstd_tempo = dstd(temp_time);
        tempi[i].dstd_tempo_media = dstd_media(temp_time);
    }

    for (int i = 0; i < numero_intervalli; i++)
    {
        double tempo = 0;
        double dstd_tempo = 0;

        intervalli[i].posizione_inizio = 40 + 10 * i;
        intervalli[i].posizione_fine = 50 + 10 * i;
        intervalli[i].posizione_intermedia = 45 + 10 * i;

        if (i == 0)
        {
            tempo = tempi[i].media_tempo;
            dstd_tempo = tempi[i].dstd_tempo_media;
            intervalli[i].media_tempo = tempi[i].media_tempo;     //solo nel primo intervallo perchè non è definito il precedente
            intervalli[i].dstd_tempo = tempi[i].dstd_tempo_media; //vale solo per il primo, negli alti casi si propaga
                                                                  //serve usare dstd_tempo_media dev std su media
            intervalli[i].tempo_intermedio = tempi[i].media_tempo / 2;
            //intervalli[i].dstd_vel = sqrt(pow((1 / tempo), 2) * pow(dstd_spazio, 2) + pow((spazio / pow(dstd_tempo, 2)), 2) * pow(dstd_tempo, 2)); // solo per il primo
            intervalli[i].dstd_vel = sqrt(pow((dstd_tempo / tempo), 2) + pow(((dstd_spazio * spazio) / (pow(tempo, 2))), 2));
        }
        else
        {
            double tempo = tempi[i].media_tempo - tempi[i - 1].media_tempo; //comodo al posto che chiamare tutta la struttura
            double dstd_tempo_1 = tempi[i - 1].dstd_tempo_media;
            double dstd_tempo_2 = tempi[i].dstd_tempo_media;
            intervalli[i].media_tempo = tempi[i].media_tempo - tempi[i - 1].media_tempo;                                                                                                                      //si usa la differenza con l'intervallo successivo
            intervalli[i].dstd_tempo = sqrt(pow(tempi[i].dstd_tempo_media, 2) + pow(tempi[i - 1].dstd_tempo_media, 2));                                                                                       //da propagazione errori casuali dev std della media dai tempi                                          //propagare differenza fra tempi dstd tempi media
            intervalli[i].tempo_intermedio = (tempi[i].media_tempo + tempi[i - i].media_tempo) / 2;                                                                                                           //formuletta della sada
            intervalli[i].dstd_vel = sqrt(2 * pow((1 / tempo), 2) * pow(dstd_spazio, 2) + pow((spazio / pow(tempo, 2)), 2) * pow(dstd_tempo_1, 2) + pow((spazio / pow(tempo, 2)), 2) * pow(dstd_tempo_2, 2)); //usare di nuovo la propagazione con distrib triangolare su spazio e incertezza su tempi trovata prima
        }
        intervalli[i].media_vel = spazio / intervalli[i].media_tempo; //va bene in qls caso, senza distinzione fra prima o ultima misura
    }

    // per testare su excel e printare i dati
    for (auto d : intervalli)
    {
        cout << d.tempo_intermedio << "\t" << d.media_vel << "\t" << d.dstd_vel << endl;
    }

    return 0;
}
