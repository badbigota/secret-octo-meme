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
    string angolo = "../Dati/PrimaParte/";
    string prefix; //modifica se vuoi cambiare l inclinazione da leggere
    cout << "Inserisci angolo (15_ 30_ 45_ d45_): ";
    cin >> prefix;
    string extension = ".txt";
    const int numero_intervalli = 7;                              //quanti intervalli di spazio ci sono per ogni angolazione
    vector<contenitore_statistico> tempi(numero_intervalli);      //storage di tutte le info dei tempi dai file a stessa angolazione
    vector<contenitore_statistico> intervalli(numero_intervalli); //storage intervalli di tempo per ciascuno spazio e di velocita con relativi errori
    double spazio = 0.1;                                          //spazio fra due rilevatori
    double dstd_spazio = 0.001 / (2 * sqrt(6));                   //da distribuz triangolare dove ptl 1mm e deltax 0.5mm

    for (int i = 0; i < numero_intervalli; i++)
    {
        ifstream fin(angolo + prefix + to_string(50 + 10 * i) + extension); //soluzione per leggere in automatico tutti i file con stessa angolazione
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
            intervalli[i].media_tempo = tempo;          //solo nel primo intervallo perchè non è definito il precedente
            intervalli[i].tempo_intermedio = tempo / 2; // // solo per il primo
            intervalli[i].dstd_tempo = dstd_tempo;      //vale solo per il primo, negli alti casi si propaga
                                                        //serve usare dstd_tempo_media dev std su media
            intervalli[i].dstd_vel = sqrt(2 * pow((dstd_spazio / tempo), 2) + pow(((spazio * dstd_tempo) / pow(tempo, 2)), 2));
        } //da 40 a 50 non si sommano due errori poichè il secondo fattoe è 0
        else
        {
            double tempo = tempi[i].media_tempo - tempi[i - 1].media_tempo; //comodo al posto che chiamare tutta la struttura
            double dstd_tempo_1 = tempi[i - 1].dstd_tempo_media;
            double dstd_tempo_2 = tempi[i].dstd_tempo_media;
            intervalli[i].media_tempo = tempo;                                                                                                                   //si usa la differenza con l'intervallo successivo
            intervalli[i].dstd_tempo = sqrt(pow(dstd_tempo_1, 2) + pow(dstd_tempo_2, 2));                                                                        //da propagazione errori casuali dev std della media dai tempi                                          //propagare differenza fra tempi dstd tempi media
            intervalli[i].tempo_intermedio = (tempi[i].media_tempo + tempi[i - 1].media_tempo) / 2;                                                              //formuletta della sada
            intervalli[i].dstd_vel = sqrt(2 * pow((dstd_spazio / tempo), 2) + pow((spazio / pow(tempo, 2)), 2) * (pow(dstd_tempo_1, 2) + pow(dstd_tempo_2, 2))); //usare di nuovo la propagazione con distrib triangolare su spazio e incertezza su tempi trovata prima
        }
        intervalli[i].media_vel = spazio / intervalli[i].media_tempo; //va bene in qls caso, senza distinzione fra prima o ultima misura
    }

    ofstream ftempi("../Stat/PerLatex/tempi_" + prefix + ".txt");
    if (!ftempi)
    {
        cout << "Errore scrittura tempi";
        return 1;
    }
    ofstream fspeed("../Stat/PerLatex/stat" + prefix + "_con_colonna_inizio_e_fine_.txt");
    if (!fspeed)
    {
        cout << "Errore scrittura velocità";
        return 1;
    }

    ofstream fchi("../Stat/PerLatex/chiquadro_" + prefix + ".txt");
    if (!fchi)
    {
        cout << "Errore scrittura chiquadro";
        return 1;
    }

    ofstream ferr("../Stat/PerLatex/errors_relativi_xy_" + prefix + ".txt");
    if (!ferr)
    {
        cout << "Errore scrittura errori";
        return 1;
    }

    ofstream fpos("../Stat/PerLatex/posteriori_dimostrazione_" + prefix + ".txt");
    if (!fpos)
    {
        cout << "Errore scrittura errori";
        return 1;
    }

    cout << "----------- TEMPI -----------" << endl;
    //scrivere tempi
    for (auto a : tempi)
    {
        cout << a.posizione_inizio << "\t" << a.posizione_fine << "\t" << a.media_tempo << "\t" << a.dstd_tempo << "\t" << a.dstd_tempo_media << endl;
        ftempi << a.posizione_inizio << "\t" << a.posizione_fine << "\t" << a.media_tempo << "\t" << a.dstd_tempo << "\t" << a.dstd_tempo_media << endl;
    }

    cout << "----------- VELOCITÀ -----------" << endl;
    // per testare su excel e printare i dati
    for (auto d : intervalli)
    {
        cout << d.posizione_inizio << "\t" << d.posizione_fine << "\t" << d.tempo_intermedio << "\t" << d.media_vel << "\t" << d.dstd_vel << endl;
        fspeed << d.posizione_inizio << "\t" << d.posizione_fine << "\t" << d.tempo_intermedio << "\t" << d.dstd_tempo << "\t" << d.media_vel << "\t" << d.dstd_vel << endl;
    }

    cout << "----------- CHI -----------" << endl;
    vector<double> time_tempo;
    vector<double> speed_vel;
    vector<double> speed_errors;
    vector<double> time_errors;
    vector<double> posizione;
    cout << "FUCJ";
    double speed_posteriori;
    double err_rel_x, err_rel_y, err_rel_y_2;
    double avg_speed, err_rel_post_y;

    for (auto d : intervalli)
    {
        time_tempo.push_back(d.tempo_intermedio);
        speed_vel.push_back(d.media_vel);
        speed_errors.push_back(d.dstd_vel);
        time_errors.push_back(d.dstd_tempo);
        posizione.push_back(d.posizione_fine);
    }

    speed_posteriori = sigma_y_posteriori(time_tempo, speed_vel);
    avg_speed = (speed_vel[0] + speed_vel[6]) / 2;
    // err_rel_post_y = (speed_posteriori / avg_speed) * 100;
    err_rel_post_y = speed_posteriori;

    cout << "A_intercetta: " << a_intercetta(time_tempo, speed_vel) << "\t SIGMA_a:" << sigma_a(time_tempo, speed_vel) << endl;
    cout << "B_angolare: " << b_angolare(time_tempo, speed_vel) << "\t SIGMA_b:" << sigma_b(time_tempo, speed_vel) << endl;

    fchi << "A_intercetta: " << a_intercetta(time_tempo, speed_vel) << "\t SIGMA_a:" << sigma_a(time_tempo, speed_vel) << endl;
    fchi << "B_angolare: " << b_angolare(time_tempo, speed_vel) << "\t SIGMA_b:" << sigma_b(time_tempo, speed_vel) << endl;

    cout << "----------- ERRORI -----------(Li stampa e basta su file =) )" << endl;
    ferr << "Fine\tE_rel_x\tE_rel_y" << endl;
    fpos << "Fine\tE_rel_y\tE_post" << endl;

    for (int i = 0; i < time_tempo.size(); i++)
    {
        err_rel_x = (time_errors[i] / time_tempo[i]) * 100;
        err_rel_y = (speed_errors[i] / speed_vel[i]) * 100;
        err_rel_y_2 = speed_errors[i];
        if (err_rel_x > err_rel_y)
        {
            ferr << posizione[i] << "\t" << err_rel_x << "\t > \t" << err_rel_y << endl;
        }
        else if (err_rel_x < err_rel_y)
        {
            ferr << posizione[i] << "\t" << err_rel_x << "\t < \t" << err_rel_y << endl;
        }
        else
        {
            ferr << posizione[i] << "\t" << err_rel_x << "\t = \t" << err_rel_y << endl;
        }

        if (err_rel_y_2 > err_rel_post_y)
        {
            fpos << posizione[i] << "\t" << err_rel_y_2 << "\t > \t" << err_rel_post_y << endl;
        }
        else if (err_rel_y_2 < err_rel_post_y)
        {
            fpos << posizione[i] << "\t" << err_rel_y_2 << "\t < \t" << err_rel_post_y << endl;
        }
        else
        {
            fpos << posizione[i] << "\t" << err_rel_y_2 << "\t = \t" << err_rel_post_y << endl;
        }
    }

    return 0;
}
