#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;
double prop_g_0(double a, double sigma_a_x, double alfa, double sigma_alfa);
double prop_delta_g(double angolo, double sigma_angolo, double v_med, double sigma_v_med, double gamma, double sigma_gamma);
double prop_g(double a_x, double sigma_a_x, double angolo, double sigma_angolo, double lambda, double sigma_lambda, double v_x, double sigma_v_x);


int main()
{
	vector<double> vel;
	vector<double> err_vel;
	double v_med, sigma_v_med, delta_g, sigma_delta_g, angolo, sigma_gamma, sigma_gamma_massa, gamma, gamma_massa, sigma_angolo, g_0, sigma_g_0, g, sigma_g, a_x, sigma_a_x;
	string fwrite, fread;
	sigma_angolo = 3.029e-05; //da calibrazione iniziale
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

	v_med = (vel[0] + vel[6]) / 2.;										 //calclolo velocità media (v[40-50]+v[100-110]/2)
	sigma_v_med = (1 / 2) * (sqrt(pow(err_vel[0], 2) + pow(err_vel[6], 2))); //calcolo errore su v_med con propagazione

	//media ponderata
	gamma = 6.07214e-05;
	sigma_gamma = 1.83687e-05;
	gamma_massa = 0.00024894;
	sigma_gamma_massa = 1.71628e-05;

	//calcolo deltag, g, compatibilità
	if (fread == "15")
	{
		angolo = 0.25; //in gradi decimali
		a_x = 0.041252;
		sigma_a_x = 0.00082973;
		g_0 = a_x / sin((angolo * M_PI) / 180);
		sigma_g_0 = prop_g_0(a_x, sigma_a_x, angolo, sigma_angolo);
		delta_g = (gamma * v_med) / sin((angolo * M_PI) / 180);
		sigma_delta_g = prop_delta_g(angolo, sigma_angolo, v_med, sigma_v_med, gamma, sigma_gamma);
		g = g_0 + delta_g;
		sigma_g = prop_g(a_x, sigma_a_x, angolo, sigma_angolo, gamma, sigma_gamma, v_med, sigma_v_med);
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd::\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	if (fread == "30")
	{
		angolo = 0.5;
		a_x = 0.0836481;
		sigma_a_x = 0.00243711;
		g_0 = a_x / sin((angolo * M_PI) / 180);
		sigma_g_0 = prop_g_0(a_x, sigma_a_x, angolo, sigma_angolo);
		delta_g = (gamma * v_med) / sin((angolo * M_PI) / 180);
		sigma_delta_g = prop_delta_g(angolo, sigma_angolo, v_med, sigma_v_med, gamma, sigma_gamma);
		g = g_0 + delta_g;
		sigma_g = prop_g(a_x, sigma_a_x, angolo, sigma_angolo, gamma, sigma_gamma, v_med, sigma_v_med);
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd::\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	if (fread == "45")
	{
		angolo = 0.75;
		a_x = 0.124508;
		sigma_a_x = 0.000809781;
		g_0 = a_x / sin((angolo * M_PI) / 180);
		sigma_g_0 = prop_g_0(a_x, sigma_a_x, angolo, sigma_angolo);
		delta_g = (gamma * v_med) / sin((angolo * M_PI) / 180);
		sigma_delta_g = prop_delta_g(angolo, sigma_angolo, v_med, sigma_v_med, gamma, sigma_gamma);
		g = g_0 + delta_g;
		sigma_g = prop_g(a_x, sigma_a_x, angolo, sigma_angolo, gamma, sigma_gamma, v_med, sigma_v_med);
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd::\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}

	if (fread == "45p")
	{
		angolo = 0.75;
		a_x = 0.128128;
		sigma_a_x = 0.00139633;
		g_0 = a_x / sin((angolo * M_PI) / 180);
		sigma_g_0 = prop_g_0(a_x, sigma_a_x, angolo, sigma_angolo);
		delta_g = (gamma * v_med) / sin((angolo * M_PI) / 180);
		sigma_delta_g = prop_delta_g(angolo, sigma_angolo, v_med, sigma_v_med, gamma, sigma_gamma);
		g = g_0 + delta_g;
		sigma_g = prop_g(a_x, sigma_a_x, angolo, sigma_angolo, gamma, sigma_gamma, v_med, sigma_v_med);
		cout << "Delta g:\t" << delta_g << "+/-" << sigma_delta_g << endl;
		cout << "g=g_0+delta_g:\t" << g << "+/-" << sigma_g << endl;
		cout << "Compatibilita con g_pd::\t" << comp_3(g, g_pd, sigma_g, sigma_g_pd) << endl;
	}
	return 0;
}


double prop_delta_g(double angolo, double sigma_angolo, double v_med, double sigma_v_med, double gamma, double sigma_gamma)
{
	double sig;
	double dp_gamma, dp_v_x, dp_angolo, angolo_rad;
	angolo_rad = angolo * M_PI / 180;
	dp_gamma = v_med / sin(angolo_rad);
	dp_v_x = gamma / sin(angolo_rad);
	dp_angolo = v_med * gamma * cos(angolo_rad) / pow(sin(angolo_rad), 2);
	sig = sqrt(pow(dp_v_x * sigma_v_med, 2) + pow(dp_angolo * sigma_angolo, 2) + pow(dp_gamma * sigma_gamma, 2));
	return sig;
}
double prop_g_0(double a_x, double sigma_a_x, double angolo, double sigma_angolo)
{
	double sig;
	double angolo_rad = angolo * M_PI / 180;
	sig = sqrt(pow((sigma_a_x / sin(angolo_rad)), 2) + pow(((a_x * cos(angolo_rad) * sigma_angolo) / (pow(sin(angolo_rad), 2))), 2));
	return sig;
}
double prop_g(double a_x, double sigma_a_x, double angolo, double sigma_angolo, double lambda, double sigma_lambda, double v_x, double sigma_v_x)
{
	double prop_gg;
	double dp_a_x, dp_angolo, dp_lambda, dp_v_x, angolo_rad;
	angolo_rad = angolo * M_PI / 180;
	dp_a_x = 1. / sin(angolo_rad);
	dp_lambda = v_x / sin(angolo_rad);
	dp_v_x = lambda / sin(angolo_rad);
	dp_angolo = (cos(angolo_rad) / pow(sin(angolo_rad), 2)) * (a_x + lambda * v_x);
	prop_gg = sqrt(pow((dp_a_x * sigma_a_x), 2) + pow((dp_lambda * sigma_lambda), 2) + pow((dp_v_x * sigma_v_x), 2) + pow((dp_angolo * sigma_angolo), 2));
	return prop_gg;
}
