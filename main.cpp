#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include "compatibilidad.h"
using namespace std;
/*Funcinoes Auxiliares*/
#if defined(_WIN32)
void clear() { system("cls"); }
#else
void clear() { system("clear"); }
#endif
//Esto representara un proceso
struct PROCESO{
    int idproceso; // su id
    int quantumproceso; // el tiempo en que tardara en terminarse
    int tamano; 
};

//Esto representara un bloque de memoria en BUDDY SYSTEM
struct BLOQUE_DE_MEMORIA {
    int tamano;
    bool libre;
    PROCESO* procesoqueocupa;
    BLOQUE_DE_MEMORIA* izq;
    BLOQUE_DE_MEMORIA* der;
};


//VECTORES

//Esto representara la cola de procesos
BLOQUE_DE_MEMORIA* MEMORIA;
vector<PROCESO> Cola_procesos;
vector<int> memoria;
vector<PROCESO> Cola_lista;

const int tamano_MEMORIA = 1024; // tamano total de la memoria

void inicializarMemoria() {
    MEMORIA = new BLOQUE_DE_MEMORIA{tamano_MEMORIA, true,nullptr, nullptr, nullptr};
}
//Representara la memoria, cabe destacar, que la memoria NO ES DINAMICA
//La memoria TIENE tamano FIJO, pero se dividira en segmentos
//vector<BLOQUE_DE_MEMORIA> MEMORIA;

//tamano DEL QUANTUM DEL SISTEMA 
//Esto representa el quantum que se le restara a los procesos equitativamente 
//Es decir A TODOS
const int QUANTUM_SYSTEM = 2;
 

// LAS FUNCIONES DE BUDDY SYSTEM
// HABRAN DE MOMENTO 2, una sera dividir la memoria cuando un proceso se vaya a 
// ejecutar, y la otra funcion servira para liberar la memoria y/o juntarla/liberarla nuevamente

// Un Ejemplo rapido es que si tenemos que ejecutar un proceso de 490K
// y tenemos una memoria de 1M, entonces la memoria se dividira en 2, y el proceso
// tomara el bloque de memoria necesaria

// A mi parecer deberan ser recursivas, ya que si el tamano no es adecuado
// se tendra que dividir mas y mas y mas, hasta el infinito y mas alla, mentira nomas hasta 32kb 

void imprimirMemoriaRecursiva(BLOQUE_DE_MEMORIA* bloque) {
    if(bloque->izq!=nullptr){
        imprimirMemoriaRecursiva(bloque->izq);
    }
    if(bloque->der!=nullptr){
        imprimirMemoriaRecursiva(bloque->der);
    }
    
    if(bloque->procesoqueocupa==nullptr){
    if (bloque->der==nullptr && bloque->izq==nullptr) {
        cout << "[0"<< "," << bloque->tamano
        << "(0),0]";}
    }else{
        if (bloque->der==nullptr && bloque->izq==nullptr) {
        cout<<FG_GREEN << "[" << bloque->procesoqueocupa->idproceso << "," << bloque->tamano
        << "(" << (bloque->procesoqueocupa->tamano) << ")," << bloque->procesoqueocupa->quantumproceso << "]"<<RESET_COLOR;
    }
    }
}
// Función para buscar un proceso en la memoria y reducir su quantum
void reducirQuantumProceso(BLOQUE_DE_MEMORIA* bloque, int idProceso) {
    if (bloque != nullptr) {
        if (bloque->procesoqueocupa != nullptr && bloque->procesoqueocupa->idproceso == idProceso) {
            // Reducir el quantum del proceso
            bloque->procesoqueocupa->quantumproceso -= QUANTUM_SYSTEM;
        } else {
            if (bloque->izq != nullptr) {
                reducirQuantumProceso(bloque->izq, idProceso);
            }
            if (bloque->der != nullptr) {
                reducirQuantumProceso(bloque->der, idProceso);
            }
        }
    }
}
// DIVIDIR MEMORIA
/*Esta funcion se encarga de dividir un bloque de memoria con Buddy System. Recibe un puntero al bloque actual bloque y el tamano requerido tamano_PEDIDO
Luego, verifica si el bloque es lo suficientemente grande para dividirse en dos
Si si es, crea dos bloques hijos (izquierda y derecha) y actualiza el bloque actual
Luego, decide en cual de los bloques hijos asignar el espacio llamando recursivamente a dividirmemoria.
Si el bloque no es lo suficientemente grande, simplemente marca el bloque como no libre y lo devuelve*/

// Función auxiliar para dividir un bloque en dos bloques más pequeños
void dividirBloque(BLOQUE_DE_MEMORIA* bloque) {
      if(bloque->izq==nullptr && bloque->der==nullptr && bloque->tamano > 32){
        bloque->libre=false;
        bloque->izq = new BLOQUE_DE_MEMORIA{bloque->tamano / 2, true, nullptr, nullptr, nullptr};
        bloque->der = new BLOQUE_DE_MEMORIA{bloque->tamano / 2, true, nullptr, nullptr, nullptr};
      }
}

bool asignarMemoria(PROCESO proceso, BLOQUE_DE_MEMORIA* bloque) {
    // Caso base: si el bloque actual no es lo suficientemente grande, retorna false
    if (bloque->tamano < proceso.tamano) {
        return false;
    }

    // Si el bloque actual es lo suficientemente grande, intenta asignar memoria
    if ((bloque->tamano >= proceso.tamano && proceso.tamano>(bloque->tamano/2) && bloque->libre==true) || (bloque->tamano >= proceso.tamano && bloque->libre==true && (bloque->tamano/2)<32)) {
        bloque->procesoqueocupa = new PROCESO{proceso.idproceso, proceso.quantumproceso, proceso.tamano};
        bloque->libre = false;
        return true;
    }

    // Si el bloque es lo suficientemente grande, pero no se pudo asignar, intenta en los bloques hijos
    if(bloque->libre==true){
       dividirBloque(bloque);
    }
    // Intenta asignar memoria en el bloque izquierdo
    if(bloque->izq!=nullptr){
    if (asignarMemoria(proceso, bloque->izq)) {
        return true;
    }}

    // Si no se pudo asignar en el izquierdo, intenta en el bloque derecho
    if(bloque->der!=nullptr){
    if (asignarMemoria(proceso, bloque->der)) {
        return true;
    }}

    // Si no se pudo asignar en ninguno de los bloques hijos, retorna false
    return false;
}

// LIBERAR MEMORIA
/*Esta funcion libera un bloque de memoria y fusiona bloques juntos que estan libres
Recibe un puntero al bloque que se va a liberar, luego marca el bloque como libre
Luego, entra en un bucle que fusiona bloques libres que estan juntos. Si ambos bloques hijos estan libres, fusiona el bloque actual con su hermano, duplica su tamano 
y mueve el puntero al bloque padre
El bucle continua hasta que no se pueden fusionar mas bloques o hasta llegar a la raiz del arbol */


// Función auxiliar para fusionar bloques contiguos libres
void fusionarBloquesContiguos(BLOQUE_DE_MEMORIA* bloque) {
    
    if(bloque->izq!=nullptr){
        fusionarBloquesContiguos(bloque->izq);
    }
    if(bloque->der!=nullptr){
        fusionarBloquesContiguos(bloque->der);
    }
    
    if (bloque != nullptr && bloque->izq != nullptr && bloque->der != nullptr &&
        bloque->izq->procesoqueocupa==nullptr && bloque->der->procesoqueocupa==nullptr && bloque->der->libre && bloque->izq->libre) {
        clear();
        cout << "Condensando memoria.." <<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        pausa();
        bloque->libre = true;
        delete bloque->izq;
        delete bloque->der;
        bloque->izq = nullptr;
        bloque->der = nullptr;
        clear();
        cout << "Condensando memoria.." <<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        pausa();
    }
}

void liberarmemoria(BLOQUE_DE_MEMORIA* bloque, PROCESO actual) {
    if (bloque != nullptr) {
        if (bloque->procesoqueocupa != nullptr && bloque->procesoqueocupa->idproceso == actual.idproceso) {
            delete bloque->procesoqueocupa;  // Liberar la memoria del objeto PROCESO
            bloque->procesoqueocupa = nullptr;
            bloque->libre = true;

        } else {
            if (bloque->izq != nullptr) {
                liberarmemoria(bloque->izq, actual);
            }
            if (bloque->der != nullptr) {
                liberarmemoria(bloque->der, actual);
            }
        }
    }
}
// Funcion para imprimir los procesos (se cambiara mas tarde si queremos gui)
void imprimir_procesos(){
   for(auto i = Cola_procesos.begin(); i != Cola_procesos.end(); ++i) {
    PROCESO actual = *i;
    cout << "Proceso " << actual.idproceso << " (" << actual.tamano << "," << actual.quantumproceso << ") " << endl;
   }
}

//Esta funcion genera un proceso aleatorio y cambia el id de los procesos para sumarlo en 1
PROCESO generar_proceso(int& id_procesos){

    //El tamano limite de tamano del proceso
    int tamanomin = 1;
    int tamanomax = 1024;
    //El tamano limite del cuantum del proceso
    int CUANTUMmin = 1;
    int CUANTUMmax = 4;
    
    //Valor aleatorio del tamano
    int tamano = tamanomin + rand() %(tamanomax - tamanomin +1);
    
    //Valor aleatorio del cuantum
    int cuantum = CUANTUMmin + rand() %(CUANTUMmax - CUANTUMmin +1);
    
    //id incrementa
    id_procesos +=1;

    PROCESO proceso = {id_procesos,cuantum,tamano};

    return proceso;
}
// ESTE ES EL PLANIFICADOR ROUND ROBIN
void PLANIFICADOR(){
    int i=0;
    while(true){

        clear();
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();
        //llenarvector(MEMORIA,96,true,false);
        //imprimir_memoria();

        PROCESO PorEntrar = Cola_lista[i];

        cout <<endl<< "Proceso por entrar: "<<FG_BLUE<<"("<<PorEntrar.idproceso<<","<<PorEntrar.tamano<<","<<PorEntrar.quantumproceso<<")"<<RESET_COLOR<<endl;
        pausa();
        if(asignarMemoria(PorEntrar,MEMORIA)){
            cout << FG_YELLOW<<endl<< "El proceso logro ser asignado en memoria!!"<<RESET_COLOR<<endl;
            Cola_procesos.push_back(PorEntrar);
            PROCESO begin = Cola_procesos.front();
            cout <<endl<<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
            imprimirMemoriaRecursiva(MEMORIA);
            cout << endl;
            cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
            imprimir_procesos();
            cout << endl<<FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
              pausa();

            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;

            cout <<endl<<FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<RESET_COLOR<<endl;
            //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
            if(begin.quantumproceso <= 0){
                Cola_procesos.erase(Cola_procesos.begin());
                liberarmemoria(MEMORIA,begin);
                cout <<endl<<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
                imprimirMemoriaRecursiva(MEMORIA);
                cout << endl;
                fusionarBloquesContiguos(MEMORIA);
            }else{               
                Cola_procesos.erase(Cola_procesos.begin());
                //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
                Cola_procesos.push_back(begin); 
            }
            i+=1;

        }else{
              cout << FG_RED<<endl<< "El proceso NO logro ser asignado en memoria!!"<<RESET_COLOR<<endl;
              cout <<endl<< "Proceso por entrar: "<<"("<<PorEntrar.idproceso<<","<<PorEntrar.tamano<<","<<PorEntrar.quantumproceso<<")"<<endl;
              PROCESO begin = Cola_procesos.front();
              cout <<endl<<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
              imprimirMemoriaRecursiva(MEMORIA);
              cout << endl;
              cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
              imprimir_procesos();
              cout << endl<<FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
              pausa();
              reducirQuantumProceso(MEMORIA,begin.idproceso);
              begin.quantumproceso -= QUANTUM_SYSTEM;
              cout <<endl<<FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<RESET_COLOR<<endl;
              if(begin.quantumproceso <= 0){
                  Cola_procesos.erase(Cola_procesos.begin());
                  liberarmemoria(MEMORIA,begin);
                  cout <<endl<<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
                  imprimirMemoriaRecursiva(MEMORIA);
                  cout << endl;
                  pausa();
                  fusionarBloquesContiguos(MEMORIA);
              }else{

              Cola_procesos.erase(Cola_procesos.begin());
              //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
              Cola_procesos.push_back(begin);
              }
        }
    }
}


int main(){
   srand(time(NULL));  
    //Generamos la memoria
   inicializarMemoria();

   //El id de los procesos
   int id_procesos = 0;
   
   for(int i=0;i<1000;i++){
     PROCESO proceso = generar_proceso(id_procesos);
     Cola_lista.push_back(proceso);
   }

   PLANIFICADOR();
   //SIMULACION
   //PLANIFICADOR();

   delete(MEMORIA);
   return 0;
}