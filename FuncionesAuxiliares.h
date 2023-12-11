#pragma once //para que no de problemas de compilacion por inclusion multiple
#ifndef FuncionesAuxiliares_h
#define FuncionesAuxiliares_h

#include "compatibilidad.h"
int tamano_MEMORIA=1024;

#include <string>
using std::string; 
using std::getline;
using std::stoul;
using std::to_string;

#include <iostream>
using std::cout; 
using std::cin;


inline void PresioneTecla(){
    gotoxy(getmaxX()/2-18,getmaxY()-2);
    cout<<"Precione la tecla espacio para volver";
    fflush(stdout);
    getch();
}

void recuadro(){
#if defined(_WIN32)
gotoxy(0,0);cout<<"█";
#endif
unsigned int x=getmaxX(),j=0;
unsigned int y=getmaxY(),i=0;

//lineas verticales
  while(i<=getmaxY()){
    gotoxy(x,y);cout<<"█";
    gotoxy(0,y--);cout<<"█";
    fflush(stdout);
    i++;
  }
//lineas horizontales
  y=getmaxY();
  while(j<=getmaxX()){
    gotoxy(x,y);cout<<"█";
    gotoxy(x--,0);cout<<"█";
    fflush(stdout);
    j++;
  }
fflush(stdout);
cout<<RESET_COLOR;
}

void mensajeCentrado(const string mensaje){
  clrscr();
  gotoxy(getmaxX()/2 - mensaje.size()/2, getmaxY()/2 + 1);
  cout<<mensaje;fflush(stdout);
  recuadro();
  PresioneTecla();
  clrscr();
}

unsigned int validarEntradaInt(int opcion){
//funcion auxiliar para validar entrada de numeros
bool esValido = true; //bandera para validar entrada en el do while
string entrada; //entrada del usuario 
string mensaje;
  do {
    clrscr();
    switch(opcion){
      case 0: mensaje = "Ingrese el QUANTUM DEL SISTEMA(MAXIMO): ";break;
      case 1: mensaje = "Ingrese el QUANTUM DEL PROCESO(MAXIMO): ";break;
      case 2: mensaje = "Ingrese el TAMAÑO DEL PROCESO(MAXIMO) en KB: ";break;
      case 3: mensaje = "Ingrese el intervalo de tiempo de la simulación 2 en ms: ";break;
    }
    
    const string lineaDeCaptura = "-----";
    gotoxy(getmaxX()/2 - mensaje.size()/2, getmaxY()/2 - 1);
    cout<<mensaje;
    gotoxy(getmaxX()/2 + mensaje.size()/2 , getmaxY()/2);
    cout<<lineaDeCaptura;
    esValido = true;
    fflush(stdin);
    entrada.clear(); //se limpia la entrada
      //para capturar incluso espacios
      gotoxy(getmaxX()/2 + mensaje.size()/2 , getmaxY()/2 - 1);
      reset_shell_mode();//salirse de ncurses para usar getline
      cout<<CURSOR_ON;
      getline(cin,entrada);
      cout<<CURSOR_OFF;
      reset_prog_mode();//regresar a ncurses
      string error; 
      //solo se dio enter = error
      if(entrada.length()<1){
        esValido=false;
        error="Error: Ingrese algún número";
      }
      for (char c : entrada) {
        if (!isdigit(c)) {//si no es un numero
          esValido = false;
          error="Error: Ingrese unicamente números positivos";
          break; // Si se encuentra un carácter no numérico, salir del bucle
        }
      }
      if (esValido && opcion!=3){
        if(std::stoi(entrada)>tamano_MEMORIA){
          esValido = false;
          error="Error: Excede el tamaño de memoria de "+ to_string(tamano_MEMORIA) + "KB";
        }
      }
      if (esValido && opcion==3){
        if(std::stoi(entrada)>99999){
          esValido = false;
          error="Error: EL numero es más grande que 99999";
        }
      }
      
      if(error.size()>0){ // debido al stoi
      cout<<FG_RED;
      mensajeCentrado(error);
      }
      
    }while (!esValido);//mientras no sea valido se pide por mas numeros
    if(esValido==true){
       clrscr();
       const string cargado="Dato Modificado Correctamente!";
       cout<<FG_GREEN;
       mensajeCentrado(cargado);
    }
return std::stoi(entrada);//se convierte a int
}

bool continuar(){
while(true){    
    reset_prog_mode();
    int ch=getch();
    reset_shell_mode();
        switch(ch){
            case KEY_UP:
                return false;
                break;
            case KEY_ENTER:
                return true;
                break;
            default:
                break;
        }
    }
}

#endif //FuncionesAuxiliares_h