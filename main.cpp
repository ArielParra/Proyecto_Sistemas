/* 
Equipo 1 conformado por:
Miguel Ángel Batres Luna     ID: 350553 
Alan Gael Gallardo Jiménez   ID: 351914
Luis Ernesto López Cárdenas  ID: 350346 
Ariel Emilio Parra Martínez  ID: 280862 
*/

#include <iostream>
#include <vector>
using namespace std;

//Esto representara un bloque de memoria en BUDDY SYSTEM
struct BLOQUE_DE_MEMORIA{
    int tamaño;
    bool libre;
};

//Esto representara un proceso
struct PROCESO{
    int idproceso; // su id
    int quantumproceso; // el tiempo en que tardara en terminarse
    BLOQUE_DE_MEMORIA* bloque_memoria; // Un apuntador al bloque de memoria asignado
};

//VECTORES

//Esto representara la cola de procesos
vector<PROCESO> Cola_procesos;

//Representara la memoria, cabe destacar, que la memoria NO ES DINAMICA
//La memoria TIENE TAMAÑO FIJO, pero se dividira en segmentos
vector<BLOQUE_DE_MEMORIA> MEMORIA;

//TAMAÑO DEL QUANTUM DEL SISTEMA 
//Esto representa el quantum que se le restara a los procesos equitativamente 
//Es decir A TODOS
const int QUANTUM_SYSTEM = 2;


// LAS FUNCIONES DE BUDDY SYSTEM
// HABRAN DE MOMENTO 2, una sera dividir la memoria cuando un proceso se vaya a 
// ejecutar, y la otra funcion servira para liberar la memoria y/o juntarla nuevamente

// Un Ejemplo rapido es que si tenemos que ejecutar un proceso de 490K
// y tenemos una memoria de 1M, entonces la memoria se dividira en 2, y el proceso
// tomara el bloque de memoria necesaria

// DIVIDIR MEMORIA
BLOQUE_DE_MEMORIA* dividirmemoria(int TAMAÑO_PEDIDO){

    //rifense
    return nullptr;
}

// JUNTAR MEMORIA
void juntarmemoria(BLOQUE_DE_MEMORIA* bloque_de_memoria){

   //rifense
}

void PLANIFICADOR(){

    while(!Cola_procesos.empty()){
        //Aqui inicializo el proceso actual, que sera el que esta al frente de la cola
        PROCESO ACTUAL = Cola_procesos.front();
        //Elimino ese proceso usando su iterador
        Cola_procesos.erase(Cola_procesos.begin());

        ACTUAL.quantumproceso = ACTUAL.quantumproceso - QUANTUM_SYSTEM;
        //Front sirve para referirte a lo que esta dentro de esa casilla
        //Begin sirve para referirte a la casilla en si

        //Si el proceso es mayor que es 0 quiere decir que todavia no ha acabado
        if(ACTUAL.quantumproceso > 0){
            //Mando el proceso al final de la cola
            Cola_procesos.push_back(ACTUAL);
        }else{
            //Aqui ya acabo el proceso
            juntarmemoria(ACTUAL.bloque_memoria);
        }
    }
}

int main(){
   return 0;
}