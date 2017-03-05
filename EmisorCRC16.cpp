#include <iostream>
#include <bitset>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
//---------------------------------------------------------------------------------------------------------------------------------------------
string cadenaABinario(string c);
char XOR(char dividendo, char divisor);
string CRC(string str, string polinomio);
string rellenoDeBits(string c);
list<string> entradamado(string mensaje);
void Salida(list<string> mensaje);
void SalidaNula();
bool verificarCaracteres(string m);
//---------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
	string mensaje,mostrar; 
	list<string> trama;
	ifstream archivoEntrada ("entrada.txt");

	if(archivoEntrada.is_open())
	{
		if(archivoEntrada.peek() != std::ifstream::traits_type::eof())
		{
			getline(archivoEntrada,mensaje);
			archivoEntrada.close();
			if(mensaje.size() <= 2000)
			{
				if(!verificarCaracteres(mensaje))
				{
					trama = entradamado(mensaje);
					Salida(trama);
				}
				else
				{
					cout << "El archivo entrada.txt posee caracteres invalidos" << endl;
					SalidaNula();
				}
			}
			else
			{
				cout << "El archivo entrada.txt tiene mas caracteres de los permitidos por el protocolo!" << endl;
				SalidaNula();
			}
		}
		else
		{
			cout << "El archivo esta vacio! no hay mensaje" << endl;
			SalidaNula();
		}
	}			
	else
	{
		cout << "Ocurrio un error al abrir el archivo (NO EXISTE entrada.txt)" << endl;
		SalidaNula();
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
/*Transformar una cadena a Binario y viceversa */
string cadenaABinario(string c)
{
	string cadenaBinaria;
	for (std::size_t i = 0; i < c.size(); ++i)
  	{
  		bitset<8> bin(c.c_str()[i]);
  		//devuelve el binario de cada caracter
      	cadenaBinaria += bin.to_string();
      	//concatena el binario de cada caracter en una nueva cadena que representa el binario de la cadena original
  	}
	return cadenaBinaria;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
char XOR(char dividendo, char divisor)
{
	char r;
	if(dividendo == divisor)
	{
		r = '0';
	}
	else
	{
		r = '1';
	}
	return r;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
string CRC(string str, string polinomio)
{
	char dividendo, divisor;
	bool band = true, bandera = true;
	int gradoP;
	string cadenaFinal, resultado;
	list<char> pGenerador,cadena, resto, trama;
	
	resultado = str;
	//Insetar caracteres de la cadena en la lista
	for (int i = 0; i < str.length(); i++)
	{
		cadena.push_back(str[i]);
		trama.push_back(str[i]);
		
	}

	//insertar caracteres del polinimio generador en la lista
	for (int i = 0; i < 17; i++)
		pGenerador.push_back(polinomio[i]);

	//insertar ceros segun el polinomio generador
	for (int i = 1; i < 17; i++)
		cadena.push_back('0');

	while(!cadena.empty() && band)
	{
			if(cadena.front() == '0')
				cadena.pop_front();
			else
				band = false;
	}

	band = true;
	
	while(cadena.size() >= 17)
	{
		//Va verificando bits a bits dividendo y divisor
		for (int i = 0; i < 17; i++)
		{
			dividendo = cadena.front();
			divisor = pGenerador.front();
			
			resto.push_back(XOR(dividendo,divisor)); //XOR

			cadena.pop_front();
			pGenerador.pop_front();
			pGenerador.push_back(divisor);
			
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
			cadena.push_front(resto.back());
			resto.pop_back();
		}
		band = true;
	}
	
	//Aqui se le añade al resto final los 0's que hagan falta (dependiendo del grado del polinomio) 
	int n = polinomio.size() - cadena.size();
	
	if(n > 0)
	{
		for (int i = 0; i < n-1; ++i)
			cadena.push_front('0');
	}
	//contatena el resto final a la cadena original
	while(!cadena.empty())
	{
		resultado += cadena.front();
		cadena.pop_front();
	}

	return resultado;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
string rellenoDeBits(string c)
{
	string relleno = "11111", str, aux, cadenaOriginal;

	str = c;
	while(!str.empty())
	{	
		
		if(str[0] == '1')
			aux += str[0];
		else
			aux.clear();

		cadenaOriginal += str[0];
		str.erase(str.begin());
		if(aux == relleno)
		{
			cadenaOriginal += "0";
			aux.clear();
		}
	}

	return cadenaOriginal;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
list<string> entradamado(string mensaje)
{
	string cadena,cadenaBinaria, c,polinomioGenerador = "11000000000000101"; 
	bool bandera = true, t = false;
	int i = 0;
	string nTrama[16] = {"00000", "00001", "00010", "00011","00100","00101", "00110",
	"00111", "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111"};
	string crc, aux,flag = "01111110";
	list<string> trama;

	while(bandera)
	{
		if(mensaje.size() > 125)
		{
			aux = mensaje.substr(0,125);
			mensaje.erase(mensaje.begin(),mensaje.begin()+125);
			cadenaBinaria = cadenaABinario(aux);
		}
		else
		{
			cadenaBinaria = cadenaABinario(mensaje);
			mensaje.clear();
			t = true;
		}
						
		c = flag;
		cadena += nTrama[i];

		if(!t)
			cadena += "0";
		else
			cadena += "1";

		cadena += cadenaBinaria;
		crc = CRC(cadena, polinomioGenerador);
		cadena = crc;

		c += rellenoDeBits(cadena);
							
		c += flag;
		trama.push_back(c);				
						
		if(mensaje.empty())
			bandera = false;
		aux.clear();
		c.clear();
		cadenaBinaria.clear();
		crc.clear();
		cadena.clear();
		i++;
	}
	return trama;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void Salida(list<string> mensaje)
{
	ofstream archivoSalida ("canalcrc.txt");

	if(archivoSalida.is_open())
	{
		list<string>::iterator i;
		for( i = mensaje.begin(); i != mensaje.end(); ++i)
			archivoSalida << *i << endl;

		archivoSalida.close();
	}
	else
		cout << "Hubo un error al intentar escribir en el archivo de salida" << endl;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void SalidaNula()
{
	ofstream archivoSalida ("canalcrc.txt");

	if(archivoSalida.is_open())
		archivoSalida.close();
	else
		cout << "Hubo un error al intentar escribir en el archivo de salida" << endl;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
bool verificarCaracteres(string m)
{
	int i = 0,c, n = m.size();
	bool incorrecto = false;
	while(i < n && !incorrecto)
	{
		if(m[i] < 32 || 126 < m[i])
			incorrecto = true;
		i++;
	}
	return incorrecto;
}
