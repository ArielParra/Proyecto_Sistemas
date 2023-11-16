#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct BLOQUE_DE_MEMORIA {
    int tamaño;
    bool libre;
};

struct NodoMemoria {
    int tamanio;
    bool libre;
    NodoMemoria* left;
    NodoMemoria* right;
};

NodoMemoria* CreaArbol(int tamanio) {
    NodoMemoria* nodo = new NodoMemoria;
    nodo->tamanio = tamanio;
    nodo->libre = true;
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
}

NodoMemoria* dividirmemoria(NodoMemoria* nodo, int tamanio_proceso) {
    if (nodo->tamanio == tamanio_proceso) {
        nodo->libre = false;
        return nodo;
    }

    if (nodo->tamanio > tamanio_proceso) {
        nodo->left = dividirmemoria(CreaArbol(nodo->tamanio / 2), tamanio_proceso);
        nodo->right = dividirmemoria(CreaArbol(nodo->tamanio / 2), tamanio_proceso);
        nodo->libre = false;
        return nodo;
    }

    return NULL;
}

struct PROCESO {
    int idproceso;
    int quantumproceso;
    BLOQUE_DE_MEMORIA* bloque_memoria;
};

vector<PROCESO> Cola_procesos;
vector<BLOQUE_DE_MEMORIA> MEMORIA;

const int QUANTUM_SYSTEM = 2;

void imprimir_procesos() {
    // Código para imprimir procesos
}

void imprimir_memoria() {
    // Código para imprimir la memoria
}

PROCESO generar_proceso(int& id_procesos) {
    int TAMANOmin = 50;
    int TAMANOmax = 200;
    int CUANTUMmin = 1;
    int CUANTUMmax = 10;

    int tamaño = TAMANOmin + rand() % (TAMANOmax - TAMANOmin + 1);
    int quantum = CUANTUMmin + rand() % (CUANTUMmax - CUANTUMmin + 1);

    BLOQUE_DE_MEMORIA* bloque_de_memoria = new BLOQUE_DE_MEMORIA;
    bloque_de_memoria->tamaño = tamaño;
    bloque_de_memoria->libre = true;

    id_procesos += 1;

    PROCESO proceso = {id_procesos, quantum, bloque_de_memoria};

    return proceso;
}

int main() {
    srand(time(NULL));
    MEMORIA.push_back({ 1024, true });
    NodoMemoria* MemoriaArbol = CreaArbol(1024);
    int id_procesos = 0;

    for (int i = 0; i < 20; i++) {
        PROCESO proceso = generar_proceso(id_procesos);
        Cola_procesos.push_back(proceso);
        MemoriaArbol = dividirmemoria(MemoriaArbol, proceso.bloque_memoria->tamaño);
    }

    // No se llama al planificador en esta instancia
    // PLANIFICADOR();

    return 0;
}
