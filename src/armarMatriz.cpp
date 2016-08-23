//Datos
int radio = 100;
int T_i = 1500;
int T_e = 50;

//Discretizacion
int deltaRadio = 1;
int deltaAngulo = 1;

//cantidad de segmentos de radio
int n = radio/deltaRadio

//cantidad de segmentos de angulo
int m = 360/deltaAngulo

//Matriz de Temperaturas
double[][] T = double[j][k];

//coeficientes
double a = ( (1/radio*radio) - (1/radio*deltaRadio) );
double b = ( (1/(radio*deltaRadio)) - (2/(deltaRadio*deltaRadio)) - (2/((deltaAngulo*deltaAngulo)*(deltaRadio*deltaRadio))) );
double c = ( (1/(deltaRadio*deltaRadio)) );
double d = ( (1/((radio*radio) * (deltaAngulo*deltaAngulo))) );
double e = ( (1/((radio*radio)*(deltaAngulo*deltaAngulo))) );

//Matriz de coeficientes
int cantidadIncognitas = n*m;
double[][] M = double[cantidadIncognitas][cantidadIncognitas];
for (int i = 0; i < cantidadIncognitas; ++i)
{
	int actualJ = i%m;
	int actualK = i%n;
	for (int l = 0; l < cantidadIncognitas; ++l)
	{
		double coeficiente = 0;
		int internoJ = l%m;
		int internoK = l%m;

		if (internoJ == actualJ -1 )
		{
			if (internoK == actualK)
			{
				coeficiente = a;
			}
		}
		if (internoJ == actualJ)
		{
			if (internoK == actualK -1 )
			{
				coeficiente = d;
			}
			if (internoK == actualK)
			{
				coeficiente = b;
			}
			if (internoK == internoK +1 )
			{
				coeficiente = e;
			}
		}
		if (internoJ == actualJ +1 )
		{
			if (internoK == actualK)
			{
				coeficiente = c;
			}
		}

//asigno a la matriz de coeficientes el coeficiente correspondiente
		M[i][l] = coeficiente;
	}
}

#include <stdio.h>
int main( int argc, const char* argv[] ){
	entrada();
	armado();
	salida();
}