
/* 
Equipo 1 conformado por:
Miguel Ángel Batres Luna     ID: 350553 
Alan Gael Gallardo Jiménez   ID: 351914
Luis Ernesto López Cárdenas  ID: 350346 
Ariel Emilio Parra Martínez  ID: 280862 
*/

#include <iostream>
#include <vector>
#include <time.h>

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
// ejecutar, y la otra funcion servira para liberar la memoria y/o juntarla/liberarla nuevamente

// Un Ejemplo rapido es que si tenemos que ejecutar un proceso de 490K
// y tenemos una memoria de 1M, entonces la memoria se dividira en 2, y el proceso
// tomara el bloque de memoria necesaria

// A mi parecer deberan ser recursivas, ya que si el tamaño no es adecuado
// se tendra que dividir mas y mas y mas, hasta el infinito y mas alla, mentira nomas hasta 32kb uwu


// DIVIDIR MEMORIA
BLOQUE_DE_MEMORIA* dividirmemoria(int TAMAÑO_PEDIDO){

    //rifense
    return nullptr;
}

// LIBERAR MEMORIA
void liberarmemoria(BLOQUE_DE_MEMORIA* bloque_de_memoria){

   //rifense
}

// ESTE ES EL PLANIFICADOR ROUND ROBIN
void PLANIFICADOR(){

    while(!Cola_procesos.empty()){
        
        // sleep o usleep aqui, aunque quizas se maneje globalmente
        // ya que el usuario podra modificiar la velocidad de la simulacion
        // alomejor y ponemos que use las flechitas para aumentar o disminuir la velocidad, yo k se

        // system(cls) o system(clear) aqui
        // imprimir_procesos(); aqui
          
        //Aqui inicializo el proceso actual, que sera el que esta al frente de la cola
        PROCESO ACTUAL = Cola_procesos.front();

        //aqui deberia de ir un if para comprobar si hay espacio en memoria
        //esta seria la implementacion de buddy system en round robin :)

        //Elimino ese proceso usando su iterador
        Cola_procesos.erase(Cola_procesos.begin());

        //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
        ACTUAL.quantumproceso = ACTUAL.quantumproceso - QUANTUM_SYSTEM;

        //Front sirve para referirte a lo que esta dentro de esa casilla
        //Begin sirve para referirte a la casilla en si

        //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
        if(ACTUAL.quantumproceso > 0){
            //Mando el proceso al final de la cola
            Cola_procesos.push_back(ACTUAL);
        }else{
            //Aqui ya acabo el proceso
            //liberarmemoria(ACTUAL.bloque_memoria);
             Cola_procesos.erase(Cola_procesos.begin());
        }
    }
}
//Esta funcion genera un proceso aleatorio y cambia el id de los procesos para sumarlo en 1
PROCESO generar_proceso(int& id_procesos){

    //El tamaño limite de tamaño del proceso
    int TAMAÑOmin = 50;
    int TAMAÑOmax = 200;
    //El tamaño limite del cuantum del proceso
    int CUANTUMmin = 1;
    int CUANTUMmax = 10;
    
    //Valor aleatorio del tamaño
    int tamaño = TAMAÑOmin + rand() %(TAMAÑOmax - TAMAÑOmin +1);
    
    //Valor aleatorio del cuantum
    int cuantum = CUANTUMmin + rand() %(CUANTUMmax - CUANTUMmin +1);

    //Se genera el tamaño de bloque de memoria
    BLOQUE_DE_MEMORIA* bloque_de_memoria;
    bloque_de_memoria->tamaño = tamaño;
    bloque_de_memoria->libre = true;
    
    //id incrementa
    id_procesos +=1;

    PROCESO proceso = {id_procesos,cuantum,bloque_de_memoria};

    return proceso;
}

// Funcion para imprimir los procesos (se cambiara mas tarde si queremos gui)
void imprimir_procesos(){
    
}

// Aqui Imprimiremos como se vera la memoria, como ira dividiendo sus segmentos o juntandolos,etc
void imprimir_memoria(){

}

int main(){
   srand(time(NULL));  
   //El id de los procesos
   int id_procesos;

   //Aqui un for rapidito donde genero 20 procesos aleatoriamente
   for(int i=0;i<20;i++){
     PROCESO proceso = generar_proceso(id_procesos);
     Cola_procesos.push_back(proceso);
   }
   
   //SIMULACION
   //PLANIFICADOR();


   return 0;
}