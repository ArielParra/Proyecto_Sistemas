#pragma once //para que no de problemas de compilacion por inclusion multiple
#ifndef FuncionesBuddyRound_h
#define FuncionesBuddyRound_h

#include <iostream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include "compatibilidad.h"
using namespace std;

int tamano_MEMORIA = 1024; 
unsigned int QUANTUM_SYSTEM = 2;
unsigned int tamanoprocesomax = 1024;
unsigned int quantumprocesomax = 4;


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

inline void inicializarMemoria() {
    MEMORIA = new BLOQUE_DE_MEMORIA{tamano_MEMORIA, true,nullptr, nullptr, nullptr};
}
inline void imprimirMemoriaRecursiva(BLOQUE_DE_MEMORIA* bloque) {
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
        if (bloque->der==nullptr && bloque->izq==nullptr) {// atencion la segunda vez
        cout<<FG_GREEN << "[" << bloque->procesoqueocupa->idproceso << "," << bloque->tamano
        << "(" << (bloque->procesoqueocupa->tamano) << ")," << bloque->procesoqueocupa->quantumproceso << "]"<<RESET_COLOR;
        }
    }
}
// Función para buscar un proceso en la memoria y reducir su quantum
inline void reducirQuantumProceso(BLOQUE_DE_MEMORIA* bloque, int idProceso) {
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
inline void dividirBloque(BLOQUE_DE_MEMORIA* bloque) {
      if(bloque->izq==nullptr && bloque->der==nullptr && bloque->tamano > 32){
        bloque->libre=false;
        bloque->izq = new BLOQUE_DE_MEMORIA{bloque->tamano / 2, true, nullptr, nullptr, nullptr};
        bloque->der = new BLOQUE_DE_MEMORIA{bloque->tamano / 2, true, nullptr, nullptr, nullptr};
      }
}

inline bool asignarMemoria(PROCESO proceso, BLOQUE_DE_MEMORIA* bloque) {
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
inline void fusionarBloquesContiguos(BLOQUE_DE_MEMORIA* bloque,bool &salida) {
    int ch = 0;
    if(bloque->izq!=nullptr){
        fusionarBloquesContiguos(bloque->izq,salida);
    }
    if(bloque->der!=nullptr){
        fusionarBloquesContiguos(bloque->der,salida);
    }
    
    if (bloque != nullptr && bloque->izq != nullptr && bloque->der != nullptr &&
        bloque->izq->procesoqueocupa==nullptr && bloque->der->procesoqueocupa==nullptr && bloque->der->libre && bloque->izq->libre) {
        cout <<endl<< "Condensando memoria.." <<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout <<endl;
        bloque->libre = true;
        delete bloque->izq;
        delete bloque->der;
        bloque->izq = nullptr;
        bloque->der = nullptr;
    }
}

inline void liberarmemoria(BLOQUE_DE_MEMORIA* bloque, PROCESO actual) {
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
inline void imprimir_procesos(){
   for (const auto& actual : Cola_procesos) {
        cout << "Proceso " << actual.idproceso << " [" << actual.idproceso << "," << actual.tamano << "," << actual.quantumproceso << "] ";
        if (&actual == &Cola_procesos.front()) {
            cout << FG_YELLOW << "-> Principio de la Lista" << RESET_COLOR;
        }
        if (&actual == &Cola_procesos.back() && &actual!=&Cola_procesos.front()) {
            cout << FG_YELLOW << "-> Final de la Lista" << RESET_COLOR;
        }
        cout << endl;
    }
}

//Esta funcion genera un proceso aleatorio y cambia el id de los procesos para sumarlo en 1
inline PROCESO generar_proceso(int& id_procesos){

    
    //Valor aleatorio del tamano
    int tamano = 1 + rand() %(tamanoprocesomax-1 +1);
    
    //Valor aleatorio del cuantum
    int cuantum =  1 + rand() %(quantumprocesomax - 1 +1);
    
    //id incrementa
    id_procesos +=1;

    PROCESO proceso = {id_procesos,cuantum,tamano};

    return proceso;
}
void generarprocesos(){
  int id_procesos = 0;
  for(int i=0;i<1000;i++){
     PROCESO proceso = generar_proceso(id_procesos);
     Cola_lista.push_back(proceso);
   }
}
// ESTE ES EL PLANIFICADOR ROUND ROBIN
inline void PLANIFICADOR(){
    const int ancho_pantalla=getmaxX()/2 - 40;    
    clrscr();
    unsigned int id_procesos = 0;
    inicializarMemoria();
    generarprocesos();
    reset_shell_mode();
    int i=0;
    int ch = 0;
    bool salida = true;
    while(salida){
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<<"Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();

        PROCESO PorEntrar = Cola_lista[i];

        cout <<endl<<FG_BLUE<< "Proceso por entrar: "<<"("<<PorEntrar.idproceso<<","<<PorEntrar.tamano<<","<<PorEntrar.quantumproceso<<")"<<RESET_COLOR<<endl;
            cout << endl << "Presione enter para continuar" << endl;
        do{
            reset_prog_mode();
            ch = getch();
            reset_shell_mode();
            if(ch ==KEY_UP){salida = false;break;}
        }while(ch!=KEY_ENTER);
        ch = 0;
        if(asignarMemoria(PorEntrar,MEMORIA)){
            cout <<endl<<FG_YELLOW<< "El proceso logro ser asignado en memoria!!"<<RESET_COLOR<<endl;
            Cola_procesos.push_back(PorEntrar);
            PROCESO begin = Cola_procesos.front();
            cout <<endl<<FG_MAGENTA<< ".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
            imprimirMemoriaRecursiva(MEMORIA);
            cout << endl;
            cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
            imprimir_procesos();
            cout << endl<<FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
            cout << endl << "Presione enter para continuar" << endl;
              do{
            reset_prog_mode();
            ch = getch();
            reset_shell_mode();
            if(ch ==KEY_UP){salida = false;break;}
        }while(ch!=KEY_ENTER);
             ch = 0;
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;

            cout <<endl<<FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<RESET_COLOR<<endl<<endl;
            //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
            if(begin.quantumproceso <= 0){
                Cola_procesos.erase(Cola_procesos.begin());
                liberarmemoria(MEMORIA,begin);
                fusionarBloquesContiguos(MEMORIA,salida);
                cout << endl;
                imprimirMemoriaRecursiva(MEMORIA);
                cout <<endl<<endl;
            }else{               
                Cola_procesos.erase(Cola_procesos.begin());
                //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
                Cola_procesos.push_back(begin); 
            }
            i+=1;

        }else{

              cout << FG_RED<<endl<< "El proceso NO logro ser asignado en memoria!!"<<RESET_COLOR<<endl;
              PROCESO begin = Cola_procesos.front();
              cout <<endl<<  FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
              imprimirMemoriaRecursiva(MEMORIA);
              cout << endl;
              cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
              imprimir_procesos();
              cout << endl<< FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
            cout << endl << "Presione enter para continuar" << endl;
              do{
            reset_prog_mode();
            ch = getch();
            reset_shell_mode();
            if(ch ==KEY_UP){salida = false;break;}
        }while(ch!=KEY_ENTER);
              ch = 0;
              reducirQuantumProceso(MEMORIA,begin.idproceso);
              begin.quantumproceso -= QUANTUM_SYSTEM;
              cout <<endl<< FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<RESET_COLOR<<endl<<endl;
              if(begin.quantumproceso <= 0){
                  Cola_procesos.erase(Cola_procesos.begin());
                  liberarmemoria(MEMORIA,begin);
                  fusionarBloquesContiguos(MEMORIA,salida);
                  cout <<endl;
                  imprimirMemoriaRecursiva(MEMORIA);
                  cout <<endl<<endl;
              }else{

              Cola_procesos.erase(Cola_procesos.begin());
              //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
              Cola_procesos.push_back(begin);
              }
        }
    }
    Cola_procesos.clear();
    Cola_lista.clear();
    delete(MEMORIA);
    reset_prog_mode();
}

#endif
