#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;

int main()
{
	double vel_med, err_vel_med;
	vector<double> vel;
	vector<double> err_vel;
	double delta_g, sigma_delta_g, sigma_ponderata, sigma_ponderata_massa, media_ponderata, media_ponderata_massa, sigma_angolo;
	string fwrite, fread;
	double g_0, sigma_g_0, g, sigma_g;
	sigma_angolo = M_PI / (38880 * sqrt(24));
	double g_pd = 9.801;
	double sigma_g_pd = 0.001;

	cout << "Inserire il postfisso di lettura, stat__.txt ";
	cin >> fread;

	ifstream fin("../Stat/stat" + fread + ".txt");
	if (!fin)
	{
		cout << "Errore lettura file";
		return 1;
	}

	int index = 0;
	double t;

	while (fin >> t)
	{
		if (index == 1)
		{
			vel.push_back(t);
			index++;
		}
		if (index == 2)
		{
			err_vel.push_back(t);
			index = 0;
		}
		else
			index++;
	}

	//calclolo velocità media (v[40-50]+v[100-110]/2)
	vel_med = (vel[0] + vel[6]) / 2.;
	//calcolo errore su vel_med con propagazione
	err_vel_med = (1 / 2) * (sqrt(pow(err_vel[0], 2) + pow(err_vel[6], 2)));

	//media ponderata
	media_ponderata = 6.07214e-05;
	sigma_ponderata = 1.83687e-05;
	media_ponderata_massa = 0.00024894;
	sigma_ponderata_massa = 1.71628e-05;
	//calcolo deltag
	if (fread == "15")
	{
		g_0 = 9.45429;
		sigma_g_0 = 0.19349;
		delta_g = (media_ponderata * vel_med) / sin((0.25 * M_PI) / 180);
		sigma_delta_g = sqrt(pow(media_ponderata / sin((0.25 * M_PI) / 180), 2) * pow(err_vel_med, 2) + pow(vel_med * media_ponderata * (-cos((0.25 * M_PI) / 180) / pow(sin((0.25 * M_PI) / 180), 2)), 2) * pow(sigma_angolo, 2) + pow(vel_med / (sin((0.25 * M_PI) / 180)), 2) * pow(sigma_ponderata, 2));
		g = g_0 + delta_g;
		sigma_g = 0.5 * sqrt(pow(sigma_g_0, 2) + pow(sigma_delta_g, 2));
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd:\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	if (fread == "30")
	{
		g_0 = 9.58549;
		sigma_g_0 = 0.279863;
		delta_g = (media_ponderata * vel_med) / sin((0.5 * M_PI) / 180);
		sigma_delta_g = sqrt(pow(media_ponderata / sin((0.5 * M_PI) / 180), 2) * pow(err_vel_med, 2) + pow(vel_med * media_ponderata * (-cos((0.5 * M_PI) / 180) / pow(sin((0.5 * M_PI) / 180), 2)), 2) * pow(sigma_angolo, 2) + pow(vel_med / (sin((0.5 * M_PI) / 180)), 2) * pow(sigma_ponderata, 2));
		g = g_0 + delta_g;
		sigma_g = 0.5 * sqrt(pow(sigma_g_0, 2) + pow(sigma_delta_g, 2));
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd:\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	if (fread == "45")
	{
		g_0 = 9.51198;
		sigma_g_0 = 0.0630146;
		delta_g = (media_ponderata * vel_med) / sin((0.75 * M_PI) / 180);
		sigma_delta_g = sqrt(pow(media_ponderata / sin((0.75 * M_PI) / 180), 2) * pow(err_vel_med, 2) + pow(vel_med * media_ponderata * (-cos((0.75 * M_PI) / 180) / pow(sin((0.75 * M_PI) / 180), 2)), 2) * pow(sigma_angolo, 2) + pow(vel_med / (sin((0.75 * M_PI) / 180)), 2) * pow(sigma_ponderata, 2));
		g = g_0 + delta_g;
		sigma_g = 0.5 * sqrt(pow(sigma_g_0, 2) + pow(sigma_delta_g, 2));
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd:\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}

	if (fread == "45p")
	{
		g_0 = 9.78854;
		sigma_g_0 = 0.107385;
		delta_g = (media_ponderata_massa * vel_med) / sin((0.75 * M_PI) / 180);
		sigma_delta_g = sqrt(pow(media_ponderata_massa / sin((0.75 * M_PI) / 180), 2) * pow(err_vel_med, 2) + pow(vel_med * media_ponderata_massa * (-cos((0.75 * M_PI) / 180) / pow(sin((0.75 * M_PI) / 180), 2)), 2) * pow(sigma_angolo, 2) + pow(vel_med / (sin((0.75 * M_PI) / 180)), 2) * pow(sigma_ponderata_massa, 2));
		g = g_0 + delta_g;
		sigma_g = 0.5 * sqrt(pow(sigma_g_0, 2) + pow(sigma_delta_g, 2));
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd:\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	return 0;
}
