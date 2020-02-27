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

    /*per vedere se tutto funziona, e si, funziona tutto.
    addirittura meglio di marc perchè vengono fuori valori che non hanno senso con il programma di marc.
    esce che nel campione 15_110 dove ci sono tutte misure superiori a 3, hanno media di circa 2.
    MA COME CAZZO È POSSIBILE!!! 
    */

    /*
    for (auto d : tempi)
    {
        cout << d.posizione_inizio << "\t" << d.posizione_fine << "\t" << d.posizione_intermedia << "\t" << d.media_tempo << "\t" << d.dstd_tempo << "\t" << d.dstd_tempo_media << "\t" << d.media_vel << "\t" << d.dstd_vel << "\t" << d.dsts_vel_media << endl;
    }

    */

    for (int i = 0; i < numero_intervalli; i++)
    {
        intervalli[i].posizione_inizio = 40 + 10 * i;
        intervalli[i].posizione_fine = 50 + 10 * i;
        intervalli[i].posizione_intermedia = 45 + 10 * i;

        if (i == 0)
        {
            intervalli[i].media_tempo = tempi[i].media_tempo;     //solo nel primo intervallo perchè non è definito il precedente
            intervalli[i].dstd_tempo = tempi[i].dstd_tempo_media; //vale solo per il primo, negli alti casi si propaga
                                                                  //serve usare dstd_tempo_media dev std su media
            intervalli[i].tempo_intermedio = tempi[i].media_tempo / 2;
        }
        else
        {
            intervalli[i].media_tempo = tempi[i].media_tempo - tempi[i - 1].media_tempo; //si usa la differenza con l'intervallo successivo
            intervalli[i].dstd_tempo_media = 0;                                          //propagare differenza fra tempi dstd tempi media
            intervalli[i].tempo_intermedio = (tempi[i].media_tempo + tempi[i - i].media_tempo) / 2;
        }
        intervalli[i].media_vel = spazio / intervalli[i].media_tempo;
        intervalli[i].dstd_vel = 0; //usare di nuovo la propagazione con distrib triangolare su spazio e incertezza su tempi trovata prima
    }


/* per testare su excel e printare i dati
    for (auto d : intervalli)
    {
        cout << d.tempo_intermedio << "\t" << d.media_vel << endl;
    }
    
*/
    return 0;
}
