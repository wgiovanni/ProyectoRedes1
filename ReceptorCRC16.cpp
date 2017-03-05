#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;
//-----------------------------------------------------------------------------------------------------------------------------
void Salida(string mensaje);	
string Traduccion(string cadena);
void receptorCRC(string str, string polinomio, string &cadenaFinal, bool &error);
char XOR(char dividendo, char divisor);
string QuitarRelleno(string cadena);
bool VerificarOrdenDeTramas(string cabeceraCont, int contadorTramas);
//-----------------------------------------------------------------------------------------------------------------------------
int main()
{
	int contadorTramas=0;
	bool error, TFinal;
	string cadena, cadenaFinal, mostrar, crc, ct, polinomioGenerador, flag;
	list <string> cadenaBinaria;
	ifstream archivoEntrada ("canalcrc.txt");
	polinomioGenerador = "11000000000000101";
	error = false;
	TFinal = false;

	//Si existe el archivo
	if(archivoEntrada.is_open())
	{
		//Si el archivo no esta vacio
		if(archivoEntrada.peek() != std::ifstream::traits_type::eof())
		{
			while(getline(archivoEntrada,cadena))
				cadenaBinaria.push_back(cadena);
			
			archivoEntrada.close();

			// Si y solo si hay 16 tramas o menos 
			if(cadenaBinaria.size() <= 16)
			{
				// Mientras no ocurra error y no sea la ultima trama
				while(!TFinal && !error)
				{	
					cadena = cadenaBinaria.front();
					cadenaBinaria.pop_front();

					//ct tiene el contador de trama
					ct = cadena.substr(9,4);
		
					//Verifico que no sea la ultima trama
					if(cadena[13] == '1' && cadenaBinaria.empty())
						TFinal = true;
					else
					{
						//Hay una trama con el bit T en '1' y no es la ultima trama
						if(cadena[13] == '1' && !cadenaBinaria.empty())
							error = true;
						else
						{
							//La ultima trama tiene el bit T en '0'
							if(cadena[13] == '0' && cadenaBinaria.empty())
								error = true;
						}
					}
					//Verifico que las tramas lleguen ordenadas
					error = VerificarOrdenDeTramas(ct,contadorTramas);

					/*Verificando que las banderas no tengan errores*/
					// Bandera Izquierda
					flag = cadena.substr(0,8);
					cadena.erase(cadena.begin(),cadena.begin()+8);
					
					if(flag != "01111110")
						error = true;

					//Bandera Derecha
					flag = cadena.substr(cadena.size()-8,cadena.size());
					cadena.erase(cadena.end()-8,cadena.end());
					if(flag != "01111110")
						error = true;

					contadorTramas++;
					if(!error)
					{
						cadena = QuitarRelleno(cadena);
						//cadena.erase(0,6);
						receptorCRC(cadena, polinomioGenerador, cadenaFinal, error);
						cadenaFinal.erase(cadenaFinal.begin(),cadenaFinal.begin()+6);
						mostrar += Traduccion(cadenaFinal);
						cadenaFinal.clear();
						cadena.clear();
					}
					else
					{
						cout << "Se ha producido un error en la transmision, hay una trama estropeada!" << endl;
						mostrar = "Se ha producido un error en la transmision, hay una trama estropeada!";
					}
				}
			}
			else
			{
				cout << "El archivo canalcrc.txt tiene mas tramas de las permitidas por el protocolo!" << endl;
				mostrar = "El archivo canalcrc.txt tiene mas tramas de las permitidas por el protocolo!";
			}
		}
		else
		{
			cout << "El archivo esta vacio! no hay mensaje" << endl;
			mostrar = "El archivo esta vacio! no hay mensaje";
		}
	}			
	else
	{
		cout << "Ocurrio un error al abrir el archivo (NO EXISTE canalcrc.txt)" << endl;
		mostrar = "Ocurrio un error al abrir el archivo (NO EXISTE canalcrc.txt)";
	}

	Salida(mostrar);
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------
string QuitarRelleno(string cadena)
{
	string relleno, aux, str=cadena, salida;
	relleno = "11111";

	while(!str.empty())
	{
		if(str[0] == '1')
			aux += str[0];
		else
			aux.clear();

		salida += str[0];
		str.erase(str.begin());
		if(aux == relleno)
		{
			str.erase(str.begin());
			aux.clear();
		}		
	}
	return salida;
}
//-----------------------------------------------------------------------------------------------------------------------------
bool VerificarOrdenDeTramas(string cabeceraCont, int contadorTramas)
{
	bool error = true;

	if(cabeceraCont == "0000" && contadorTramas == 0)
		error = false;
	if(cabeceraCont == "0001" && contadorTramas == 1)
		error = false;
	if(cabeceraCont == "0010" && contadorTramas == 2)
		error = false;
	if(cabeceraCont == "0011" && contadorTramas == 3)
		error = false;
	if(cabeceraCont == "0100" && contadorTramas == 4)
		error = false;
	if(cabeceraCont == "0101" && contadorTramas == 5)
		error = false;
	if(cabeceraCont == "0110" && contadorTramas == 6)
		error = false;
	if(cabeceraCont == "0111" && contadorTramas == 7)
		error = false;
	if(cabeceraCont == "1000" && contadorTramas == 8)
		error = false;
	if(cabeceraCont == "1001" && contadorTramas == 9)
		error = false;
	if(cabeceraCont == "1010" && contadorTramas == 10)
		error = false;
	if(cabeceraCont == "1011" && contadorTramas == 11)
		error = false;
	if(cabeceraCont == "1100" && contadorTramas == 12)
		error = false;
	if(cabeceraCont == "1101" && contadorTramas == 13)
		error = false;
	if(cabeceraCont == "1110" && contadorTramas == 14)
		error = false;
	if(cabeceraCont == "1111" && contadorTramas == 15)
		error = false;

	return error;
}
//-----------------------------------------------------------------------------------------------------------------------------
void Salida(string mensaje)
{
	ofstream archivoSalida ("salidacrc.txt");

	if(archivoSalida.is_open())
	{
		archivoSalida << mensaje;
		archivoSalida.close();
	}
	else
		cout << "Hubo un error al intentar escribir en el archivo de salida" << endl;
}
//-----------------------------------------------------------------------------------------------------------------------------
string Traduccion(string cadena)
{
	int i=0,j=0,n;
	string sub;
	string mostrar;
	mostrar="";

	while(!cadena.empty())
	{
		sub = cadena.substr(0,8);
		cadena.erase(cadena.begin(),cadena.begin()+8);
		n = sub.size();
		i=0, j=0;
	    while(i < sub.size())
	    {
	    	if(sub[i]=='1')
	    		j += pow(2,n-(i+1));
	    	i++;	
	    }
		char c = (char)j;
		mostrar += c;
 	}
 	return mostrar;
}
//-----------------------------------------------------------------------------------------------------------------------------
char XOR(char dividendo, char divisor)
{
	char r;
	if(dividendo == divisor)
		r = '0';
	else
		r = '1';

	return r;
}
//-----------------------------------------------------------------------------------------------------------------------------
void receptorCRC(string str, string polinomio, string &cadenaFinal, bool &error)
{
	char dividendo, divisor;
	bool band = true, bandera = true, band2 = true;
	int gradoP;
	list<char> pGenerador,cadena, resto, trama;
	
	for (int i = 0; i < str.length(); i++)
	{
		trama.push_back(str[i]);
		cadena.push_back(str[i]);
	}
	//insertar caracteres del polinimio generador en la lista
	for (int i = 0; i < 17; i++)
		pGenerador.push_back(polinomio[i]);
	
	gradoP = pGenerador.size();
	while(trama.size() >= 17)
	{
		//Va verificando bits a bits dividendo y divisor
		for (int i = 0; i < 17; i++)
		{
			dividendo = trama.front();
			trama.pop_front();
			divisor = pGenerador.front();
			pGenerador.pop_front();
			pGenerador.push_back(divisor);
			resto.push_back(XOR(dividendo,divisor));
		}
		//Elimina los 0's que esten al principio de cada resto, hasta que encuentre un 1
		while(!resto.empty() && band)
		{
			if(resto.front() == '0')
				resto.pop_front();
			else
				band = false;
		}
		//Verifica que la cadena no sea vacia y si lo es, le asigna false a la bandera
		while(!resto.empty())
		{
			//inserta al frente de la lista original el elemento final del lista resto 
			trama.push_front(resto.back());
			resto.pop_back();
		}
		band = true;
	}
	//Aqui se le añade al resto final los 0's que hagan falta (dependiendo del grado del polinomio) 
	int n = polinomio.size() - trama.size();
	
	//cout << "n:" << n << endl;
	if(n > 0)
	{
		for (int i = 0; i < n-1; ++i)
		{
			trama.push_front('0');
			//cout << "entro" << endl;
		}
	}
	list<char>::iterator j;
		//cout << "resto " << endl;
		for( j = trama.begin(); j != trama.end(); ++j)
			//cout << *j;
	//cout << endl;
	int i=0;
	while(!trama.empty() && band)
	{
		if(trama.front() != '0')
			band = false;

		trama.pop_front();
	}
	if(band)
	{
		int j = 0;
		while((pGenerador.size()-1) > j)
		{
			cadena.pop_back();
			j++;
		}
	}
	else
		error = true;
		
	while(!cadena.empty())
	{
		cadenaFinal += cadena.front();
		cadena.pop_front();
	}
}
//-----------------------------------------------------------------------------------------------------------------------------