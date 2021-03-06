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
	int tamCharacteres = readFile(archivo, "");
	int i;
	int numeroDeBitsGuardados = 0;
	int tempPosicionBit = 0;
	unsigned char dondeGuardoLosBytesFormateados[TAMANIO_MAX];
	for(i  = 0; i <= tamCharacteres; i++){
	 //tengo que leer en archivo, el dato de 
	 unsigned char *direccionInformacion = archivo -> informacion;
	 unsigned char tempChar = *(direccionInformacion+i);//le sumo 1 porque cada byte vale 1
	 //Ahora necesito saber cual es el numero de 1 a 26, recuerda:esta en ascii
	 unsigned char dirreccionParaLosArreglos = tempChar - 'A';
	 unsigned char charACodificar;
	 charACodificar = codigoCompresion[dirreccionParaLosArreglos];
	 int tempLogitud = longitudCodigo[dirreccionParaLosArreglos];
	 //Ahora Teniendo Esa Informacion puedo llamar el siguiente metodo
	 //p.s el parametro nuevoTamanio va a decir en que numero de character va
	 agregarAlArreglo(dondeGuardoLosBytesFormateados, charACodificar, tempPosicionBit, tempLogitud, numeroDeBitsGuardados%8);
	 
	 //Alisto las variables para el sigueinte llamado
	 numeroDeBitsGuardados = numeroDeBitsGuardados + tempLogitud;
	 tempPosicionBit = tempPosicionBit + tempLogitud;
	    if(tempPosicionBit > 8){
	    tempPosicionBit = (tempPosicionBit - 8);
	    }
	}
	//Al final del ciclo tengo que escribir en el archivo los nuevos bytes que tengo en el arreglo dondeGuardoLosBytesFormateados[]
	uploadCode(dondeGuardoLosBytesFormateados, "");
	
	Archivo* formateado = new  Archivo();
	formateado->tamanio = (numeroDeBitsGuardados%8);
	formateado->informacion = dondeGuardoLosBytesFormateados;
	
	writeFile(numeroDeBitsGuardados%8, formateado, "");
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
        //Quiero convertir lo que tenemos escrito del byte en un arreglo de ints y terminar de llenarlo con 0. Para poder user bitwise operations
        int loQueLlevamosPasandoAInt;
        int tempPosicionBit = posicionBit;
        int tempByte[8];
        for(loQueLlevamosPasandoAInt = 0; loQueLlevamosPasandoAInt < 8; loQueLlevamosPasandoAInt++){
            if(tempPosicionBit > 0){
            unsigned char charAPasar = datosCodificados[(nuevoTamanio- longitud) - tempPosicionBit];
            tempByte[loQueLlevamosPasandoAInt] = ((int)charAPasar) -'0';
            tempPosicionBit = tempPosicionBit +1;
            }else{
             //Tengo que agregar 0
             tempByte[loQueLlevamosPasandoAInt] = 0;
            }
        }
        //Ahora tengo que crear el bit para el bitwise operation 
        int tempPosicionbitBit = posicionBit;
        int tempLogitud = 0;
        int byteConCodigo[8];
        int i;
        for(i = 0 ; i < 8; i++){
            //Tengo que poner tantos 0 como posicion bit, luego agregar el codigo y si faltan bits para completar 8 pongo más 0
            if(tempPosicionbitBit > 0){
            byteConCodigo[i] = 0;
            }else if(tempLogitud <= longitud) {
            //Saco el primer dato
             byteConCodigo[i] = (codigo >> tempLogitud) & 1;
            }else{
            byteConCodigo[i] = 0;
            }    
        }
        //No estoy seguro de esta parte
        int byteYaFormateado[8];
        int j;
        for(j = 0; j < 8; j++ ){
        byteYaFormateado[i] = byteYaFormateado[i]|tempByte[i];
        }
        //Ahora toca cojer el bit que ya esta formateado y agregarlo al array de caracteres
        
    
    }else{
     //Me toca hacerlo en diferentes bytes
     
     
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