#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

//Esto representara un bloque de memoria en BUDDY SYSTEM
struct BLOQUE_DE_MEMORIA {
    int tamaño;
    bool libre;
    int idprocesoqueocupa;
    BLOQUE_DE_MEMORIA* izq;
    BLOQUE_DE_MEMORIA* der;
};

//Esto representara un proceso
struct PROCESO{
    int idproceso; // su id
    int quantumproceso; // el tiempo en que tardara en terminarse
    int tamaño; // Un apuntador al bloque de memoria asignado
};

//VECTORES

//Esto representara la cola de procesos
vector<PROCESO> Cola_procesos;
BLOQUE_DE_MEMORIA* MEMORIA;



const int TAMAÑO_MEMORIA = 1024; // Tamaño total de la memoria

void inicializarMemoria() {
    MEMORIA = new BLOQUE_DE_MEMORIA{ TAMAÑO_MEMORIA, true, nullptr, nullptr, nullptr };
}
//Representara la memoria, cabe destacar, que la memoria NO ES DINAMICA
//La memoria TIENE TAMAÑO FIJO, pero se dividira en segmentos
//vector<BLOQUE_DE_MEMORIA> MEMORIA;

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
/*Esta funcion se encarga de dividir un bloque de memoria con Buddy System. Recibe un puntero al bloque actual bloque y el tamaño requerido TAMAÑO_PEDIDO
Luego, verifica si el bloque es lo suficientemente grande para dividirse en dos
Si si es, crea dos bloques hijos (izquierda y derecha) y actualiza el bloque actual
Luego, decide en cual de los bloques hijos asignar el espacio llamando recursivamente a dividirmemoria.
Si el bloque no es lo suficientemente grande, simplemente marca el bloque como no libre y lo devuelve*/

bool dividirmemoria(BLOQUE_DE_MEMORIA& bloque, PROCESO actual) {   
    if (bloque->tamaño >= actual->tamaño * 2) {
        // Crear dos bloques hijos
        if(bloque->izq==nullptr){
            bloque->izq = new BLOQUE_DE_MEMORIA{ bloque->tamaño / 2, true, nullptr, nullptr };}
        if(bloque->der==nullptr){
            bloque->der = new BLOQUE_DE_MEMORIA{ bloque->tamaño / 2, true, nullptr, nullptr };}

        // Actualizar el bloque actual
        bloque->tamaño /= 2;              
        bloque->libre = false;

        // Es para decidir en cual de los bloques hijos se va a asignar la memoria
        if (TAMAÑO_PEDIDO <= bloque->izq->tamaño && bloque->libre==true) {
            return dividirmemoria(bloque->izq, actual->tamaño);
        } else if(bloque->libre==true){
            return dividirmemoria(bloque->der, actual->tamaño);
        } else if{
            return false;
        }
    } else {
        bloque->libre = false;
        bloque->idprocesoqueocupa = actual->idproceso;
        return true;
    }
}

// LIBERAR MEMORIA
/*Esta funcion libera un bloque de memoria y fusiona bloques juntos que estan libres
Recibe un puntero al bloque que se va a liberar, luego marca el bloque como libre
Luego, entra en un bucle que fusiona bloques libres que estan juntos. Si ambos bloques hijos estan libres, fusiona el bloque actual con su hermano, duplica su tamaño 
y mueve el puntero al bloque padre
El bucle continua hasta que no se pueden fusionar mas bloques o hasta llegar a la raiz del arbol */
void liberarmemoria(BLOQUE_DE_MEMORIA* bloque, PROCESO actual) {
    
    if(bloque->izq->idprocesoqueocupa == actual->idproceso){
        bloque->izq->libre = true;
        if(bloque->der->libre == true){
            bloque->izq = nullptr;
            bloque->der = nullptr;
            bloque->tamaño *= 2;
        }
    }else if(bloque->der->idprocesoqueocupa == actual->idproceso){
        bloque->der->libre = true;
        if(bloque->izq->libre == true){
            bloque->izq = nullptr;
            bloque->der = nullptr;
            bloque->tamaño *= 2;
        }
    }else if{
        liberarmemoria(bloque->izq,actual);
        liberarmemoria(bloque->der,actual);
    }
}

// ESTE ES EL PLANIFICADOR ROUND ROBIN
void PLANIFICADOR(){
    //inicializo el begin de la cola de procesos
    int it = Cola_procesos.begin(); 
    while(!Cola_procesos.empty()){
        
        // sleep o usleep aqui, aunque quizas se maneje globalmente
        // ya que el usuario podra modificiar la velocidad de la simulacion
        // alomejor y ponemos que use las flechitas para aumentar o disminuir la velocidad, yo k se

        // system(cls) o system(clear) aqui
        // imprimir_procesos(); aqui
          
        //Aqui inicializo el proceso actual, que sera el que esta al frente de la cola
       

        //aqui deberia de ir un if para comprobar si hay espacio en memoria
        //esta seria la implementacion de buddy system en round robin :)
        PROCESO ACTUAL = Cola_procesos.front();

        if(dividirmemoria(MEMORIA,ACTUAL) == true){
         
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
             
        }else{
           //Avanza al siguiente en la lista de procesos
           if(++it == Cola_procesos.end()){
             it = Cola_procesos.begin();
           }
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
    //Generamos la memoria
   inicializarMemoria();

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