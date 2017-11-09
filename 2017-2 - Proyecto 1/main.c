#define _CRT_SECURE_NO_DEPRECATE 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



// NOMBRES Y CODIGOS DE LOS INTEGRANTES



//-- Constantes
#define TAMANIO_MAX 10000    //Tamanio maximo del archivo
#define NUMERO_CODIGOS 26      //Maximo de valores distintos en el archivo

//-- Definicion de la estructura para el archivo de entrada y el comprimido
typedef struct archivo
{
	int tamanio;
	unsigned char *informacion;
} Archivo;

//-- Arreglos que contendran el codigo de compresion
// arreglo que contiene el codigo. Cada byte contiene un codigo de compresion

unsigned char codigoCompresion[NUMERO_CODIGOS];
// arreglo con la longitud. Cada byte tiene la longitud en bits del codigo de compresion correspondiente 
// en el arreglo de arriba
int longitudCodigo[NUMERO_CODIGOS];



//-- Prototipos de las funciones
int readFile(Archivo * archivo, char *);
int readFileCode(unsigned char[], char *);
void writeFile(int, Archivo * archivoCodificado, char *);
int codificar(Archivo * archivo, Archivo * archivoCodificado);
void agregarAlArreglo(unsigned char[], unsigned char, int, int, int);
void uploadCode(unsigned char[], char *);


//-- Funciones

// Esta funcion se encarga de abrir un archivo y leerlo en el vector datos.
// Retorna el n�mero de bytes leidos.
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int readFile( Archivo * archivo, char * nombreArchivo )
{
	FILE *file;
	int n;
	int x;

	if (!(file = fopen(nombreArchivo, "rb"))) {
		printf("No se puede abrir el archivo: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	n = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	archivo -> tamanio = n;	
	archivo -> informacion = (unsigned char *) calloc (n, sizeof (unsigned char));

	for ( x = 0; x < n; x++) {
			fread (&archivo -> informacion [x], sizeof(unsigned char ), 1, file);
		}
		
	fclose(file);
	
	return n;
}

// Esta funcion se encarga de abrir un archivo de codigo
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int readFileCode(unsigned char datos[], char *nombreArchivo)
{
	FILE *file;
	int n;

	if (!(file = fopen(nombreArchivo, "rb"))) {
		printf("No se puede abrir el archivo: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	n = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (n > TAMANIO_MAX) {
		printf("Archivo demasiado grande: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}

	fread(datos, 1, n, file);

	fclose(file);

	return n;
}

// Esta funcion se encarga de abrir el archivo que contiene el codigo de compresion.
// Hace uso de la funcion readFile
// Guarda el codigo de compresion en el arreglo llamado codigo
// No hay que completar nada en esta funcion.
// NO MODIFICAR
void uploadCode(unsigned char codigo[], char *nombreArchivoCodigo)
{
	unsigned char archivoCodigo[TAMANIO_MAX];
	//en la primera mitad guardamos el codigo de compresion, en la segunda mitad, en cada byte, 
	//guardamos la lognitud en bits de cada traduccion

	int tamanioCodigoTotal = readFileCode(archivoCodigo, nombreArchivoCodigo); // esto deberia ser 52
	int tamanioCodigo = tamanioCodigoTotal / 2; // deberia ser 26
	int i;
	
	printf("El tamanio del codigo es %d \n", tamanioCodigoTotal);

	//guardamos el codigo y su longitud en los arreglos de arriba
	for ( i = 0; i < tamanioCodigo; i++) {
		codigo[i]   = archivoCodigo[i];
	}

	for ( i = tamanioCodigo; i < tamanioCodigoTotal; i++) {
		longitudCodigo[i - tamanioCodigo] = (int)archivoCodigo[i];
	}
}


// Esta funcion se encarga de escribir un archivo a partir del vector datos.
// El numero de bytes que se deben escribir viene en el parametro n.
// No hay que completar nada en esta funcion.
void writeFile(int n, Archivo * archivoCodificado, char *nombreArchivo)
{
     FILE *file; 
      if (!(file = fopen(nombreArchivo, "wb")))
      { 
              printf("No se puede abrir el archivo: %s\n", nombreArchivo); 
              exit(EXIT_FAILURE); 
      }
        fwrite((archivoCodificado->informacion), 1, n, file);
 fclose(file);
}


// Esta funcion se encarga de codificar cada uno de los valores que se encuentran en
// la estructura llamada archivo y asignarlos a la estructura llamada archivocodificado.
// DESARROLLAR ESTA FUNCION EN SU TOTALIDAD.
int codificar(Archivo * archivo, Archivo * archivocodificado)
{
	
	File file1* file2*; //Archivos
	int tam = 0; //Tamanho en cantidad de caracteres
	int[] a = new int[100]; //Cadena de caracteres sin comprimir en binarios(estan en binarios por el fgetc que devuelve un int del unsigned char)
	int[] b = new int[100]; //Cadena de caracteres codificada
	if (!(file1 = fopen(archivo, "r"))) {
		printf("No se puede abrir el archivo: %s\n", archivo);
		exit(EXIT_FAILURE);
	}
	else{
		file1 = fopen(archivo, "r");
		while(feof(file1))
		{
			tam++; //Se guarda el tamanho
			a[tam] = fgetc(file1); //Se guarda el unsigned char en cada posicion
		}
	}
	int j = 1; //Contador para iterar sobre la cadena "a"
	b[0] = 0; //Inicia a codificar de la siguiente forma, el primer caracter empieza en 0, luego 1, luego 10, 11, 100, 101 y así sucesivamente
	while(j < tam)
	{
		int i = j-1; 
		int k = 1; //El valor en entero del numero de codificacion(Falta pasarlo a binario)
		while(i > 0)
		{
			if(a[j] != a[i]) //Si la letra a codificar no ha sido codificada aun, se le da el valor de la posicion(TOCA PASAR A BINARIO)
			{
				b[j] = k; 
				k++;
			} 
			else //Sino, se le da el valor del que ya tenía
			{
				b[j] = b[k];
			}
			i--;
		}
		k++;
		j++;
	}
	if (!(file2 = fopen(archivocodificado, "w"))) {
		printf("No se puede abrir el archivo codificado: %s\n", archivocodificado);
		exit(EXIT_FAILURE);
	}
	else{
		file2 = fopen(archivocodificado, "w"); //Finalmente se abre el nuevo archivo
		int i = 0;
		while(i<tam)
		{
			fputc(b[i], file2); //Pone caracter por caracter en el nuevo archivo
		}
	}
}

// Esta funcion recibe como parametros el vector de datos codificados,
// el codigo que se debe insertar, la longitud de este ultimo y la posicion donde
// debe insertar el codigo en el vector codificado (posicionBit).
// Se encarga de convertir los datos de codigo a bits e insertarlos en la posicion
// que les corresponde en datosCodificados. Longitud es la longitud de cada código, posicionBit es la posición del bit en el que se va
//a insertar, y nuevoTamanio es el tamanio del archivo comprimido.
// DESARROLLAR ESTA FUNCION EN SU TOTALIDAD.
//longitud = size_t s = sizeof(unsigned char  codigo);
//nuevoTamanio me dice el byte en que voy
void agregarAlArreglo(unsigned char datosCodificados[], unsigned char codigo, int longitud, int posicionBit, int nuevoTamanio)
{
    if((posicionBit+longitud) <= 8){
    //Este es el caso facil, lo puedo agregar en el mismo byte
        //
        unsigned char dondeFormatearCharacter = datosCodificados[nuevoTamanio];
        unsigned int shift = posicionBit - longitud;//El numero de 0 que tengo agregar
        unsigned char codigoConShift = codigo << shift;
        unsigned char charFormateado = dondeFormatearCharacter|codigoConShift;
        datosCodificados[nuevoTamanio] = charFormateado;
    
    }else{
     //Me toca hacerlo en diferentes bytes
        //Toca dividir el codigo en dos char's, uno lo muevo a la izquierda para perder los bits extras((posicionBit+longitud)-8)
        //El otro char lo muevo a la derecha para sacar los bit de la derecha
        int shiftIzquierda_PrimerChar = ((posicionBit+longitud)-8);
        unsigned char tempPrimerChar = codigo;
        tempPrimerChar = tempPrimerChar >> shiftIzquierda_PrimerChar;
        
        int shiftDerecha_SegundoChar = (8 - shiftIzquierda_PrimerChar);
        unsigned char tempSegundoChar = codigo;
        tempSegundoChar = tempSegundoChar << shiftDerecha_SegundoChar;
        
        //Ahora los agrego a sus respectivos bytes
        unsigned char primerCharParaFormatear = datosCodificados[nuevoTamanio];
        primerCharParaFormatear = primerCharParaFormatear|tempPrimerChar;
        datosCodificados[nuevoTamanio] = primerCharParaFormatear;
        
        unsigned char segundoCharParaFormatear = datosCodificados[(nuevoTamanio+1)];
        segundoCharParaFormatear = segundoCharParaFormatear|tempSegundoChar;
        datosCodificados[(nuevoTamanio+1)] = segundoCharParaFormatear;
    }
}

//-- Funcion main de la aplicacion
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int main()
{
	int tamanio;
	int tamanioCodificado;
	unsigned char nombreArchivo[] = "";
	unsigned char nombreCodigo[] = "";
	unsigned char nombreCodificado[] = "";
	Archivo * archivo = (Archivo *) malloc (sizeof (Archivo));
	Archivo * archivoCodificado = (Archivo *) malloc (sizeof (Archivo));

	printf("Ingrese el nombre del archivo a comprimir (incluya el .txt): \n");
	scanf("%s", &nombreArchivo);
	tamanio = readFile(archivo, nombreArchivo);
	printf("El tamanio del archivo es %d \n", tamanio);
	printf("Ingrese el nombre del archivo que contiene el codigo (sin la extension de archivo): \n");
	scanf("%s", &nombreCodigo);
	uploadCode(codigoCompresion, nombreCodigo);
	archivoCodificado -> informacion = (unsigned char *) calloc (tamanio, sizeof(unsigned char));
	printf("Ingrese el nombre del archivo para guardar el archivo codificado (incluya el .txt): \n");
	scanf("%s", &nombreCodificado);
	tamanioCodificado = codificar(archivo, archivoCodificado);
	writeFile((tamanioCodificado + 7) / 8, archivoCodificado, nombreCodificado);

	system("PAUSE");
}