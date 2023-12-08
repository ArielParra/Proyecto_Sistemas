#include "herenciaPoli.cpp"
#include "FuncionesAuxiliares.h"
#include "FuncionesBuddyRound.h"

extern int tamano_MEMORIA; 
extern unsigned int QUANTUM_SYSTEM;
extern unsigned int tamanoprocesomax;
extern unsigned int quantumprocesomax;



inline void logouaa(){
const int altura_grafico=16,ancho_grafico=43;
int x=(getmaxX()/4)-(ancho_grafico/2),y=(getmaxY()/2)-(altura_grafico/2);
gotoxy(x, y);  cout<<FG_BLUE<<"█▒  "<< FG_CYAN<<"██████       "<<FG_CYAN<<"██████ "<<FG_BLUE<<"▒▒█████▒▒▒        "; 
gotoxy(x, y++);cout<<FG_BLUE<<"███  "<<FG_CYAN<<"███████   "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒█████████████     "; 
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████                      "<<FG_BLUE<<"█████   "; 
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████               "<<FG_CYAN<<"███████  "<<FG_BLUE<<"▒███▒ ";
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████        "<<FG_YELLOW<<"██      "<<FG_CYAN<<"████████ "<<FG_BLUE<<"▒███▒"; 
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████       "<<FG_YELLOW<<"████ "<<FG_RED<<"█    "<<FG_CYAN<<"████████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████      "<<FG_YELLOW<<"████ "<<FG_RED<<"███    "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████     "<<FG_YELLOW<<"████ "<<FG_RED<<"█████   "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███"; 
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████    "<<FG_YELLOW<<"████ "<<FG_RED<<"██████   "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████    "<<FG_YELLOW<<"███ "<<FG_RED<<"██████    "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"███▒ "<<FG_CYAN<<"███████    "<<FG_YELLOW<<"██ "<<FG_RED<<"█████      "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"████ "<<FG_CYAN<<"████████      "<<FG_RED<<"████       "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███"; 
gotoxy(x, y++);cout<<FG_BLUE<<" ███▒ "<<FG_CYAN<<"████████      "<<FG_RED<<"██        "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<" ▒████  "<<FG_CYAN<<"███████               ███████ "<<FG_BLUE<<"▒███"; 
gotoxy(x, y++);cout<<FG_BLUE<<"   █████                      "<<FG_CYAN<<"███████ "<<FG_BLUE<<"▒███";
gotoxy(x, y++);cout<<FG_BLUE<<"     ▒████████████▒ "<<FG_CYAN<<"████████   ███████ "<<FG_BLUE<<"███"<<RESET_COLOR;
fflush(stdout);
}


inline void flecha_derecha() {
  const int altura_grafico=9, ancho_grafico=14;
  int x=getmaxX()-ancho_grafico,y=getmaxY()-altura_grafico;
  cout<<FG_BLUE;
  gotoxy(x, y++); cout << "   ▒▒▒▒▒▒▒▒    ";
  gotoxy(x, y++); cout << "  ▒        ▒   "; 
  gotoxy(x, y++); cout << " ▒      ▒   ▒  "; 
  gotoxy(x, y++); cout << "▒        ▒   ▒ "; 
  gotoxy(x, y++); cout << "▒  ▒▒▒▒▒▒▒▒  ▒ "; 
  gotoxy(x, y++); cout << "▒        ▒   ▒ ";  
  gotoxy(x, y++); cout << " ▒      ▒   ▒  ";  
  gotoxy(x, y++); cout << "  ▒        ▒   ";  
  gotoxy(x, y++); cout << "   ▒▒▒▒▒▒▒▒    "<<RESET_COLOR;
  fflush(stdout);

}

inline void flecha_izquierda() {
  const int altura_grafico=9;
  int x=1,y=getmaxY()-altura_grafico;
  cout<<FG_BLUE;
  gotoxy(x, y++); cout << "   ▒▒▒▒▒▒▒▒    ";
  gotoxy(x, y++); cout << "  ▒        ▒   ";  
  gotoxy(x, y++); cout << " ▒   ▒      ▒  ";  
  gotoxy(x, y++); cout << "▒   ▒        ▒ ";  
  gotoxy(x, y++); cout << "▒  ▒▒▒▒▒▒▒▒  ▒ ";  
  gotoxy(x, y++); cout << "▒   ▒        ▒ "; 
  gotoxy(x, y++); cout << " ▒   ▒      ▒  "; 
  gotoxy(x, y++); cout << "  ▒        ▒   "; 
  gotoxy(x, y++); cout << "   ▒▒▒▒▒▒▒▒    "<<RESET_COLOR;
  fflush(stdout);
}



void primer_pantalla() {
  /*Primer Pantalla*/
 Maestro santiago("J. Santiago Cortés López", "Sistemas Operativos");
  Alumno miguel("Miguel Ángel Batres Luna   ", 350553, 3, 'A');
    Alumno alan("Juan Damián Ortega De Luna ", 351914, 3, 'A');
   Alumno ariel("Ariel Emilio Parra Martínez", 280862, 3, 'A');

  clrscr();
  flecha_derecha();
  const unsigned int altura_grafico = 16, ancho_grafico = 43;
  unsigned int x = (getmaxX() / 3) + (ancho_grafico / 3);
  unsigned int y = (getmaxY() / 2) - (altura_grafico / 3);
  logouaa();
  x = (getmaxX()/3) +20;
  y = (getmaxY() / 2) - (altura_grafico / 3);
  gotoxy(x,y++);
  santiago.imprimirInformacion();

  gotoxy(x,y++);
  gotoxy(x,y++);
  miguel.imprimirInformacion();
  gotoxy(x,y++);
  alan.imprimirInformacion();
  gotoxy(x,y++);
  ariel.imprimirInformacion();
  gotoxy(x,y++);
  fflush(stdout);
}
void segunda_pantalla() {
  /*segunda Pantalla*/
  clrscr();
  flecha_izquierda();
  flecha_derecha();
  const int ancho_grafico = 93,altura_grafico=12;

  int x = (getmaxX() / 2) - (ancho_grafico / 2),y = (getmaxY() / 2) - (altura_grafico/2);
  
  cout << FG_BLUE;
  gotoxy(x,y++); cout <<"██████╗ ██╗   ██╗██████╗ ██████╗ ██╗   ██╗   ██████╗██╗   ██╗ ██████╗████████╗███████╗███╗   ███╗";
  gotoxy(x,y++); cout <<"██╔══██╗██║   ██║██╔══██╗██╔══██╗╚██╗ ██╔╝  ██╔════╝╚██╗ ██╔╝██╔════╝╚══██╔══╝██╔════╝████╗ ████║";
  gotoxy(x,y++); cout <<"██████╦╝██║   ██║██║  ██║██║  ██║ ╚████╔╝   ╚█████╗  ╚████╔╝ ╚█████╗    ██║   █████╗  ██╔████╔██║";
  gotoxy(x,y++); cout <<"██╔══██╗██║   ██║██║  ██║██║  ██║  ╚██╔╝     ╚═══██╗  ╚██╔╝   ╚═══██╗   ██║   ██╔══╝  ██║╚██╔╝██║";
  gotoxy(x,y++); cout <<"██████╦╝╚██████╔╝██████╔╝██████╔╝   ██║     ██████╔╝   ██║   ██████╔╝   ██║   ███████╗██║ ╚═╝ ██║";
  gotoxy(x,y++); cout <<"╚═════╝  ╚═════╝ ╚═════╝ ╚═════╝    ╚═╝     ╚═════╝    ╚═╝   ╚═════╝    ╚═╝   ╚══════╝╚═╝     ╚═╝"<<RESET_COLOR;
  
  cout << FG_CYAN;
  gotoxy(x,y++); cout <<"   ██████╗  █████╗ ██╗   ██╗███╗  ██╗██████╗   ██████╗  █████╗ ██████╗ ██████╗ ██╗███╗  ██╗";
  gotoxy(x,y++); cout <<"   ██╔══██╗██╔══██╗██║   ██║████╗ ██║██╔══██╗  ██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║████╗ ██║";
  gotoxy(x,y++); cout <<"   ██████╔╝██║  ██║██║   ██║██╔██╗██║██║  ██║  ██████╔╝██║  ██║██████╦╝██████╦╝██║██╔██╗██║";
  gotoxy(x,y++); cout <<"   ██╔══██╗██║  ██║██║   ██║██║╚████║██║  ██║  ██╔══██╗██║  ██║██╔══██╗██╔══██╗██║██║╚████║";
  gotoxy(x,y++); cout <<"   ██║  ██║╚█████╔╝╚██████╔╝██║ ╚███║██████╔╝  ██║  ██║╚█████╔╝██████╦╝██████╦╝██║██║ ╚███║";
  gotoxy(x,y++); cout <<"   ╚═╝  ╚═╝ ╚════╝  ╚═════╝ ╚═╝  ╚══╝╚═════╝   ╚═╝  ╚═╝ ╚════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚══╝"<<RESET_COLOR;
  fflush(stdout);
}

void tercer_pantalla(int seleccion) {
  /*tercer Pantalla*/
  clrscr();
  flecha_izquierda();
  fflush(stdout);
  const char *colores[] = {FG_CYAN, FG_MAGENTA, FG_BLUE,FG_GREEN, FG_YELLOW};
  const char *opciones1[] = {"█▀█ █ █ ▄▀█ █▄ █ ▀█▀ █ █ █▀▄▀█   █▀ █▄█ █▀ ▀█▀ █▀▀ █▀▄▀█",  "█▀█ █ █ ▄▀█ █▄ █ ▀█▀ █ █ █▀▄▀█  █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀","▀█▀ ▄▀█ █▀▄▀█ ▄▀█ █▄ █ █▀█  █▀█ █▀█ █▀█ █▀▀ █▀▀ █▀ █▀","█▀ █ █▀▄▀█ █ █ █   ▄▀█ █▀▀ █ █▀█ █▄ █","█▀ ▄▀█ █   █ █▀█"};
  const char *opciones2[] = {"▀▀█ █▄█ █▀█ █ ▀█  █  █▄█ █ ▀ █   ▄█  █  ▄█  █  ██▄ █ ▀ █",  "▀▀█ █▄█ █▀█ █ ▀█  █  █▄█ █ ▀ █  █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█"," █  █▀█ █ ▀ █ █▀█ █ ▀█ █▄█  █▀▀ █▀▄ █▄█ █▄▄ ██▄ ▄█ ▄█","▄█ █ █ ▀ █ █▄█ █▄▄ █▀█ █▄▄ █ █▄█ █ ▀█","▄█ █▀█ █▄▄ █ █▀▄"};

  const int altura_grafico = 12, ancho_grafico = 66;
  int x = (getmaxX() / 2) - (ancho_grafico / 2),y = (getmaxY() / 2) - (altura_grafico/2);
 
  for (int i = 0; i < 5; i++) {
    cout<<colores[i];
    if (i == seleccion) {
      gotoxy(x,y++);cout << "    ▀▄  " << opciones1[i];
      gotoxy(x,y++);cout << "▀▀▀▀▀█▀ " << opciones2[i];
      gotoxy(x,y++);cout << "    ▀   " <<RESET_COLOR;
      gotoxy(x,y++);
    } else {
      gotoxy(x,y++);cout << "        " << opciones1[i]; 
      gotoxy(x,y++);cout << "        " <<opciones2[i];
      gotoxy(x,y++);cout << "        " <<RESET_COLOR;
      gotoxy(x,y++);
    }
  }
  fflush(stdout);

}

void consultardato(int opcion) {
  /*tercer Pantalla*/
  clrscr();
  fflush(stdout);
  string dato;
  string mensaje;
  
  switch(opcion){
    case 0:
      dato = to_string(QUANTUM_SYSTEM);
      mensaje = "El Quantum maximo del sistema es: "+dato;
    break;
    case 1:
    dato = to_string(quantumprocesomax);
      mensaje = "El Quantum maximo para los procesos es: "+dato;
    break;
    case 2:
    dato = to_string(tamanoprocesomax);
      mensaje = "El tamaño maximo para los procesos es: "+dato;
    break;
  }

  const int altura_grafico = 1, ancho_grafico = 42;
  int x = (getmaxX() / 2) - (ancho_grafico / 2),y = (getmaxY() / 2) - (altura_grafico/2);
   
  gotoxy(x,y++);
  cout << mensaje;
  fflush(stdout);
  PresioneTecla();

}
void MenuParametros(int seleccion) {
  /*tercer Pantalla*/
  clrscr();
  fflush(stdout);
  const char *colores[] = {FG_CYAN, FG_MAGENTA, FG_BLUE};
  const char *opciones1[] = {"█▀▀ █▀█ █▄ █ █▀ █ █ █   ▀█▀ ▄▀█ █▀█","█▀▄▀█ █▀█ █▀▄ █ █▀▀ █ █▀▀ ▄▀█ █▀█","█▀ ▄▀█ █   █ █▀█"};
  const char *opciones2[] = {"█▄▄ █▄█ █ ▀█ ▄█ █▄█ █▄▄  █  █▀█ █▀▄","█ ▀ █ █▄█ █▄▀ █ █▀  █ █▄▄ █▀█ █▀▄","▄█ █▀█ █▄▄ █ █▀▄"};

  const int altura_grafico = 8, ancho_grafico = 44;
  int x = (getmaxX() / 2) - (ancho_grafico / 2),y = (getmaxY() / 2) - (altura_grafico/2);
 
  for (int i = 0; i < 3; i++) {
    cout<<colores[i];
    if (i == seleccion) {
      gotoxy(x,y++);cout << "    ▀▄  " << opciones1[i];
      gotoxy(x,y++);cout << "▀▀▀▀▀█▀ " << opciones2[i];
      gotoxy(x,y++);cout << "    ▀   " <<RESET_COLOR;
      gotoxy(x,y++);
    } else {
      gotoxy(x,y++);cout << "        " << opciones1[i]; 
      gotoxy(x,y++);cout << "        " <<opciones2[i];
      gotoxy(x,y++);cout << "        " <<RESET_COLOR;
      gotoxy(x,y++);
    }
  }
  fflush(stdout);

}
void Parametros(int opcion){
  int seleccion = 0;

  bool salir = false;
  while (!salir) {
    MenuParametros(seleccion);
    switch (getch()) {
      case KEY_UP:
          if (seleccion > 0) {
            seleccion--;
          }

          break;
      case KEY_DOWN:
          if (seleccion < 3) {
            seleccion++;
          }
          break;
      case KEY_ENTER:
          switch (seleccion) {
          case 0: 
             switch(opcion){
               case 0: consultardato(0);break;
               
               case 1: consultardato(1);break;

               case 2: consultardato(2);break;
             }
            break;
          case 1:
            switch(opcion){
               case 0:  QUANTUM_SYSTEM = validarEntradaInt(0); break;
               
               case 1:  quantumprocesomax = validarEntradaInt(1);break;

               case 2:  tamanoprocesomax = validarEntradaInt(2);break;
             }
          break;
          case 2: //salir
            salir = true;
            break;
          }
      break;
      }// getch
    }//while
}
void generarprocesos(){
  int id_procesos = 0;
  for(int i=0;i<1000;i++){
     PROCESO proceso = generar_proceso(id_procesos);
     Cola_lista.push_back(proceso);
   }
}


void menus(){
 int pantalla = 1, seleccion = 0;
 int id_procesos = 0;

  bool salir = false;
  while (!salir) {

    switch (pantalla) {
    case 1:
      primer_pantalla();
      break;
    case 2:
      segunda_pantalla();
      break;
    case 3:
      tercer_pantalla(seleccion);
      break;
    }

    switch (getch()) {
      case KEY_LEFT:
        if (pantalla > 1) {
          pantalla--;
        }
        break;
      case KEY_RIGHT:
        if (pantalla < 3) {
          pantalla++;
        }
        break;
      case KEY_UP:
          if (pantalla == 3 && seleccion > 0) {
            seleccion--;
          }

          break;
      case KEY_DOWN:
          if (pantalla == 3 && seleccion < 4) {
            seleccion++;
          }
          break;
      case KEY_ENTER:
        if (pantalla == 3) {
          switch (seleccion) {
          case 0: //niveles
               Parametros(0);
            break;
          case 1:
               Parametros(1);
              break;
          case 2:
               Parametros(2);
          break;
          case 3:
               
               generarprocesos();
               PLANIFICADOR();
               delete(MEMORIA);
          break;
          case 4: //salir
            endCompat();
            exit(0);
            break;
        }
      break;
      }//if
    }// getch
  }//while
}



