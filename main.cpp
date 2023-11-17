#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
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
vector<int> memoria;
vector<PROCESO> Cola_lista;


const int TAMAÑO_MEMORIA = 1024; // Tamaño total de la memoria

void inicializarMemoria() {
    MEMORIA = new BLOQUE_DE_MEMORIA{ TAMAÑO_MEMORIA, true, 0, nullptr, nullptr};
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

bool dividirmemoria(BLOQUE_DE_MEMORIA* bloque, PROCESO* actual) {   
    if (actual->tamaño <= (bloque->tamaño/2)) {
        // Crear dos bloques hijos
        if(bloque->izq==nullptr && bloque->der==nullptr){
            bloque->izq = new BLOQUE_DE_MEMORIA{ bloque->tamaño / 2, true, 0, nullptr,nullptr };
            bloque->der = new BLOQUE_DE_MEMORIA{ bloque->tamaño / 2, true, 0, nullptr,nullptr };
            // Actualizar el bloque actual             
            bloque->libre = false;
        }
        // Es para decidir en cual de los bloques hijos se va a asignar la memoria
        if ((actual->tamaño <= bloque->izq->tamaño) && bloque->izq->libre==true) {
            return dividirmemoria(bloque->izq, actual);
        } else if((actual->tamaño <= bloque->izq->tamaño) && bloque->der->libre==true){
            return dividirmemoria(bloque->der, actual);
        } else{
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
    
    if(bloque->izq->idprocesoqueocupa == actual.idproceso){
        bloque->izq->libre = true;
        if(bloque->der->libre == true){
            bloque->izq = nullptr;
            bloque->der = nullptr;
            bloque->tamaño *= 2;
        }
    }else if(bloque->der->idprocesoqueocupa == actual.idproceso){
        bloque->der->libre = true;
        if(bloque->izq->libre == true){
            bloque->izq = nullptr;
            bloque->der = nullptr;
            bloque->tamaño *= 2;
        }
    }else if(bloque->izq!=nullptr && bloque->der!=nullptr){
        liberarmemoria(bloque->izq, actual);
        liberarmemoria(bloque->der, actual);
    }
}
void imprimir_memoria(){
   bool imprimir;
   int i,j;
   cout << endl;
   for (int i = 1; i <= 96; ++i) {
    bool imprimir = true; 
    for (int j = 0; j < memoria.size(); ++j) {
        if (memoria[j] == i) {
            imprimir = false;
            break;
        }
    }
    (imprimir) ? cout << "█" : cout << " ";
}
   cout <<endl;

}
// Funcion para imprimir los procesos (se cambiara mas tarde si queremos gui)
void imprimir_procesos(){
   for(auto i = Cola_procesos.begin(); i != Cola_procesos.end(); ++i) {
    PROCESO actual = *i;
    cout << "Proceso " << actual.idproceso << " (" << actual.tamaño << "," << actual.quantumproceso << ") " << endl;
   }
}
void llenarvector(BLOQUE_DE_MEMORIA* bloque,int tamañototal,bool izq,bool der){
   int tamaño;
   if(izq==true){
       if(bloque->izq!=nullptr && bloque->der!=nullptr){
          tamaño = tamañototal/2;
          memoria.push_back(tamaño);
          llenarvector(bloque->izq,tamaño,true,false);
          llenarvector(bloque->der,tamaño,false,true);
       }
   }
   if(der==true){
        if(bloque->izq!=nullptr && bloque->der!=nullptr){
          tamaño = tamaño + (tamañototal/2);
          memoria.push_back(tamaño);
          llenarvector(bloque->izq,tamaño,true,false);
          llenarvector(bloque->der,tamaño,false,true);
        }
   }
}
//Esta funcion genera un proceso aleatorio y cambia el id de los procesos para sumarlo en 1
PROCESO generar_proceso(int& id_procesos){

    //El tamaño limite de tamaño del proceso
    int TAMAÑOmin = 1;
    int TAMAÑOmax = 1000;
    //El tamaño limite del cuantum del proceso
    int CUANTUMmin = 1;
    int CUANTUMmax = 50;
    
    //Valor aleatorio del tamaño
    int tamaño = TAMAÑOmin + rand() %(TAMAÑOmax - TAMAÑOmin +1);
    
    //Valor aleatorio del cuantum
    int cuantum = CUANTUMmin + rand() %(CUANTUMmax - CUANTUMmin +1);
    
    //id incrementa
    id_procesos +=1;

    PROCESO proceso = {id_procesos,cuantum,tamaño};

    return proceso;
}
// ESTE ES EL PLANIFICADOR ROUND ROBIN
void PLANIFICADOR(){
    int id_procesos = 0;
    PROCESO begin;
    bool Proceso_esperando = false;
    while(true){

        system("clear");
        memoria.clear();
        llenarvector(MEMORIA,96,true,false);
        imprimir_memoria();
        imprimir_procesos();

        //Aqui inicializo el proceso actual, que sera el que esta al frente de la cola  

        //aqui deberia de ir un if para comprobar si hay espacio en memoria
        //esta seria la implementacion de buddy system en round robin :)
       // if(Proceso_esperando == false){
        PROCESO PorEntrar = generar_proceso(id_procesos);
      //  }
        
        if(dividirmemoria(MEMORIA,&PorEntrar) == true){
        
        Cola_procesos.push_back(PorEntrar);
        
        begin = Cola_procesos.front();
        
        begin.quantumproceso = begin.quantumproceso - QUANTUM_SYSTEM;
        
        //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
        if(begin.quantumproceso <= 0){

           // liberarmemoria(MEMORIA,Cola_procesos.front());
            Cola_procesos.erase(Cola_procesos.begin());

        }else{
        
        //Elimino ese proceso usando su iterador
        
        Cola_procesos.erase(Cola_procesos.begin());

        //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
        Cola_procesos.push_back(begin);
        }

        }else{
             

             // Proceso_esperando = true;
              //Declaro el proceso del inicio
              begin = Cola_procesos.front();
              //EJECUTAR
              begin.quantumproceso = begin.quantumproceso - QUANTUM_SYSTEM;

              Cola_procesos.erase(Cola_procesos.begin());
              //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
              Cola_procesos.push_back(begin);
        
        }
        sleep(5);

    }
}


int main(){
   srand(time(NULL));  
    //Generamos la memoria
   inicializarMemoria();

   //El id de los procesos
   int id_procesos;


   PLANIFICADOR();
   //SIMULACION
   //PLANIFICADOR();

   delete(MEMORIA);
   return 0;
}