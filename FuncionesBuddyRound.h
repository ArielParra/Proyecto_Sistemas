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
#include "FuncionesAuxiliares.h"
using namespace std;

unsigned int QUANTUM_SYSTEM = 2;
unsigned int tamanoprocesomax = 50;
unsigned int quantumprocesomax = 10;


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
            if(bloque->procesoqueocupa->quantumproceso<0){bloque->procesoqueocupa->quantumproceso=0;}
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
inline void fusionarBloquesContiguos(BLOQUE_DE_MEMORIA* bloque,bool imprimir) {
    int ch = 0;
    if(bloque->izq!=nullptr){
        fusionarBloquesContiguos(bloque->izq,imprimir);
    }
    if(bloque->der!=nullptr){
        fusionarBloquesContiguos(bloque->der,imprimir);
    }
    
    if (bloque != nullptr && bloque->izq != nullptr && bloque->der != nullptr &&
        bloque->izq->procesoqueocupa==nullptr && bloque->der->procesoqueocupa==nullptr && bloque->der->libre && bloque->izq->libre) {
        if(imprimir){cout <<endl<< "Condensando memoria.." <<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout <<endl;}
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
inline void PLANIFICADOR(int ms,bool tiempo){
    clrscr();
    unsigned int id_procesos = 0;
    unsigned int atendidos = 0;
    unsigned int memoriausada= 0;
    bool entradasdedatos = true;
    inicializarMemoria();
    generarprocesos();
    reset_shell_mode();
    int i=0;
    int ch = 0;
    bool salida = true;
    while(salida){

        if(i>=Cola_lista.size()){
            salida = false;
        }
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<<"Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();

       
        PROCESO PorEntrar = Cola_lista[i];

        cout <<endl<<FG_BLUE<< "Proceso por entrar: "<<"("<<PorEntrar.idproceso<<","<<PorEntrar.tamano<<","<<PorEntrar.quantumproceso<<")"<<RESET_COLOR<<endl;
        
        if(tiempo){
            delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
        }else{
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
        }
        
        if(asignarMemoria(PorEntrar,MEMORIA)){
            cout <<endl<<FG_YELLOW<< "El proceso logro ser asignado en memoria!!"<<RESET_COLOR<<endl;
            atendidos+=1;
            Cola_procesos.push_back(PorEntrar);
            memoriausada += PorEntrar.tamano;
            PROCESO begin = Cola_procesos.front();
            cout <<endl<<FG_MAGENTA<< ".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
            imprimirMemoriaRecursiva(MEMORIA);
            cout << endl;
            cout <<endl<<FG_MAGENTA<< "Lista de procesos "<<RESET_COLOR<<endl<<endl;
            imprimir_procesos();
            cout << endl<<FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
            
            if(tiempo){
            delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
        }else{
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
        }
            cout <<endl<<FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<endl<<endl;
            cout <<"Proceso "<<begin.idproceso<<"["<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<"]"<<endl;
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;
            if(begin.quantumproceso<0){begin.quantumproceso=0;}
            cout <<"Proceso "<<begin.idproceso<<"["<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<"]"<<RESET_COLOR<<endl<<endl;
        
            //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
            if(begin.quantumproceso <= 0){
                memoriausada -= begin.tamano;
                Cola_procesos.erase(Cola_procesos.begin());
                liberarmemoria(MEMORIA,begin);
                fusionarBloquesContiguos(MEMORIA,true);
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
              
               if(tiempo){
            delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
        }else{
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
        }
               cout <<endl<<FG_CYAN<< "Proceso "<<begin.idproceso<<" EJECUTADO!!"<<endl;
            cout <<"Proceso "<<begin.idproceso<<"["<<begin.idproceso<<","<<begin.tamano<<"]"<<begin.quantumproceso<<endl;
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;
            if(begin.quantumproceso<0){begin.quantumproceso=0;}
            cout <<"Proceso "<<begin.idproceso<<"["<<begin.idproceso<<","<<begin.tamano<<"]"<<begin.quantumproceso<<RESET_COLOR<<endl<<endl;
              if(begin.quantumproceso <= 0){
                memoriausada -= begin.tamano;
                  Cola_procesos.erase(Cola_procesos.begin());
                  liberarmemoria(MEMORIA,begin);
                  fusionarBloquesContiguos(MEMORIA,true);
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
    float memoriaporcentajeusado = (memoriausada*100)/tamano_MEMORIA;
    cout << FG_BLUE;
    if(atendidos>1000){atendidos=1000;}
    estadisticas(atendidos,memoriaporcentajeusado);

    Cola_procesos.clear();
    Cola_lista.clear();
    delete(MEMORIA);
    reset_prog_mode();
}
// ESTE ES EL PLANIFICADOR ROUND ROBIN
inline void PLANIFICADOR2(int ms){
    clrscr();
   unsigned int id_procesos = 0;
    unsigned int atendidos = 0;
    unsigned int memoriausada= 0;
    inicializarMemoria();
    generarprocesos();
    reset_shell_mode();
    int i=0;
    int ch = 0;
    bool salida = true;
    bool entradasdedatos=true;
    while(salida){

        if(i>=Cola_lista.size()){
                 salida = false;
        }
        clrscr();
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<<"Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();
        
        PROCESO PorEntrar = Cola_lista[i];


        if(entradasdedatos){
        cout <<endl<<FG_BLUE<< "Proceso por entrar: "<<"("<<PorEntrar.idproceso<<","<<PorEntrar.tamano<<","<<PorEntrar.quantumproceso<<")"<<RESET_COLOR<<endl;
        }

        delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
        if(entradasdedatos){
        if(asignarMemoria(PorEntrar,MEMORIA)){
            memoriausada += PorEntrar.tamano;
            Cola_procesos.push_back(PorEntrar);
            PROCESO begin = Cola_procesos.front();
             clrscr();
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<<"Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();
        
            cout << endl<<FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl<<endl;

             delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
            
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;
           
            //Si el proceso es mayor que 0 quiere decir que todavia no ha acabado
            if(begin.quantumproceso <= 0){
                memoriausada -= begin.tamano;
                atendidos+=1;
                Cola_procesos.erase(Cola_procesos.begin());
                liberarmemoria(MEMORIA,begin);
                fusionarBloquesContiguos(MEMORIA,false);
            }else{               
                Cola_procesos.erase(Cola_procesos.begin());
                //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
                Cola_procesos.push_back(begin); 
            }
            i+=1;

        }else{
         
              PROCESO begin = Cola_procesos.front();
              
              clrscr();
        cout <<FG_MAGENTA<<".-------MEMORIA ACTUAL---------."<<RESET_COLOR<<endl<<endl;
        imprimirMemoriaRecursiva(MEMORIA);
        cout << endl;
        cout <<endl<<FG_MAGENTA<<"Lista de procesos "<<RESET_COLOR<<endl<<endl;
        imprimir_procesos();
        
              cout << endl<< FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
              
            delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
           
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;
              if(begin.quantumproceso <= 0){
                  memoriausada-= begin.tamano;
                  atendidos+=1;
                  Cola_procesos.erase(Cola_procesos.begin());
                  liberarmemoria(MEMORIA,begin);
                  fusionarBloquesContiguos(MEMORIA,false);
              }else{

              Cola_procesos.erase(Cola_procesos.begin());
              //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
              Cola_procesos.push_back(begin);
              }
        }
        }else{

             if(Cola_procesos.empty()){
                 salida = false;
              }else{
              PROCESO begin = Cola_procesos.front();
             
              cout << endl<< FG_RED<<"Proceso a ejecutar: "<<FG_YELLOW<<"("<<begin.idproceso<<","<<begin.tamano<<","<<begin.quantumproceso<<")"<<RESET_COLOR<<endl;
              
            delay(ms);
            if(kbhit()){
            cout << endl << "Presione enter para continuar o tecla 'p' para salir" << endl;
            if(!continuar(entradasdedatos)){salida = false; break;}
            }
           
            reducirQuantumProceso(MEMORIA,begin.idproceso);
            begin.quantumproceso -= QUANTUM_SYSTEM;
              if(begin.quantumproceso <= 0){
                  memoriausada -= begin.tamano;
                  atendidos+=1;
                  Cola_procesos.erase(Cola_procesos.begin());
                  liberarmemoria(MEMORIA,begin);
                  fusionarBloquesContiguos(MEMORIA,false);
              }else{

              Cola_procesos.erase(Cola_procesos.begin());
              //ESTA LINEA REPRESENTA QUE EL PROCESO SE ESTA EJECUTANDO
              Cola_procesos.push_back(begin);
              }
              }
        }
    }
    float memoriaporcentajeusado = (memoriausada*100)/tamano_MEMORIA;
    cout << FG_BLUE;
      if(atendidos>1000){atendidos=1000;}
    estadisticas(atendidos,memoriaporcentajeusado);
    Cola_procesos.clear();
    Cola_lista.clear();
    delete(MEMORIA);
    reset_prog_mode();
}

#endif
