#include<iostream>
#include<string.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
using namespace std;
/*mapa de juego*/
int matriz[8][8];

/*struct curso*/ /*Cuadro amarillo*/
struct Cursor {
	//actual del cursor
	int X;
	int Y;
	//ultimo enter
	int ultX;
	int ultY;
	
	int turno;
	//color 
	int colorJ1;
	int colorJ2;
	int colorT;
};
/*struct movimiento ficha*/ /*Lógica de movimientos*/ /*Eliminaciones*/
struct Mov {
	//peon
	int pos[2][2];
	int elim[2][2];
	int tipoM[2];
	//reina
	int movR[30][2];
	int contR;
	//contador de mov j
	int car;
	int con;
	//puntos del ultimo mov j
	int x;
	int y;
};
/*struct jugado*/
struct Jugador {
	char nombre[10];
	int puntos;
};

struct Mov MovJ;
struct Cursor C;
struct Jugador jugadores[2];

void gotoxy(const int x, const int y) {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}
void Color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//espacios x->" "
void espacio(int e) {
	for (int i = 0;i < e;i++) {
		cout << " ";
	}
}
//mover el curso 
void salto(int s) {
	for (int i = 0;i < s;i++) {
		cout << endl;
	}
}
//marco del juego
void dibujarMarco() {
	Color(3);
	for (int i = 0;i < 18;i++) {
		for (int j = 0;j < 72;j++) {
			/*columas y filas*/
			if (i == 0 && j != 0) cout << (char)205;
			if (j == 0 && i != 0 && i != 17) cout << (char)186;
			if (j == 71 && i != 0 && i != 17) {
				switch (i) {
				case 3: {
					if (C.turno == 2) {
						espacio(30);cout << "Turno: J2";espacio(32);cout << (char)186;
					}else{
						espacio(30);cout << "Turno: J1";espacio(32);cout << (char)186;
					}
				}break;
				case 5: {
					espacio(3);Color(2);cout << "Jugador-1";espacio(24);espacio(18);Color(11);cout << "Jugador-2";Color(3);espacio(8);cout << (char)186;
				}break;
				case 6: {
					espacio(3);Color(2);
					if (jugadores[0].puntos <= 9) {
						cout << "Puntos:" << jugadores[0].puntos;espacio(43);
					}
					else {
						cout << "Puntos:" << jugadores[0].puntos;espacio(42);
					}
					Color(11);
					if (jugadores[1].puntos <= 9) {
						cout << "Puntos:" << jugadores[1].puntos;espacio(9);
					}
					else {
						cout << "Puntos:" << jugadores[1].puntos;espacio(8);
					}
					Color(3);cout << (char)186;
				}break;

				default: {
					espacio(71);cout << (char)186;
				}break;
				}
			}
			if (i == 17 && j != 0) cout << (char)205;
			/*esquinas*/
			if (i == 0 && j == 0) cout << (char)201;
			if (i == 17 && j == 0) cout << (char)200;
			if (i == 0 && j == 71) cout << (char)187;
			if (i == 17 && j == 71) cout << (char)188;
		}
		cout << endl;
	}
	Color(7);
	cout<<"Turno de:";
	if(C.turno==2){
		cout<<jugadores[1].nombre<<endl;
	}else{
		cout<<jugadores[0].nombre<<endl;
	}
}
int ExisteMovimientoJugador(int x, int y) {
	int res = 0;
	if (MovJ.pos[0][0] == x && MovJ.pos[0][1] == y || MovJ.pos[1][0] == x && MovJ.pos[1][1] == y) {
		res = 1;
	}
	return res;
}
int existeMJR(int x, int y) {
	int res = 0;
	for (int i = 0;i < MovJ.contR;i++) {
		if (MovJ.movR[i][0] == x && MovJ.movR[i][1] == y) {
			res = 1;
		}
	}
	return res;
}

void mostrar(int m[8][8]) {
	int i, j;
	char f;
	Color(7);
	for (i = 0;i < 8;i++) {
		for (j = 0;j < 8;j++) {
			
			gotoxy(24 + (j * 3), 5 + (i)); //Pocisionar matriz. /*Validar si el jugador tocó una ficha*/
			if (m[i][j] == 0) {
				//mostrar color amarillo
				if (i == C.X && j == C.Y)Color(6);
				else Color(6400+C.colorT);

				if (MovJ.car > 0) {
					if (ExisteMovimientoJugador(i, j)) {
						Color(6);
					}
				}
				if (MovJ.contR > 0) {
					if (existeMJR(i, j)) {
						Color(6);
					}
				}
				if ((i + j + 1) % 2 == 0) {//intercalar forma en el tablero
					f = 178;
					cout << "[" << f << "]";
				}
				else {
					f = 177;
					cout << "[" << f << "]";
				}
				
			}

			switch (m[i][j]) {
			case 1: {

				if (i == C.X && j == C.Y)Color(6);
				else Color(15);

				char f = 157;
				cout << "[" << f << "]";
				Color(7);
			}break;
			case 2: {
				char g = 184;

				if (i == C.X && j == C.Y)Color(6);
				else Color(8);
				cout << "[" << g << "]";

				Color(7);
			}break;
			case 3: {
				if (i == C.X && j == C.Y)Color(6);
				else Color(14);

				cout << "[" << "R" << "]";
				Color(7);
			}break;
			case 4: {

				if (i == C.X && j == C.Y)Color(6);
				else Color(8);
				cout << "[" << "R" << "]";

				Color(7);
			}break;
			}
		}
		cout << endl;
	}
	Color(7);
}
//consulta
int limite(int x, int y) {
	if (x >= 0 && y >= 0 && x < 8 && y < 8) return 1;
	return 0;
}
void posibleM(int tipo) {
	int x = 0, y = 0;
	switch (tipo) {
	case 1:x = -1;break;
	case 2:x = 1;break;
	case 3:y = 1;break;
	case 4:y = -1;break;
	}
	if (limite(C.X + x, C.Y + y)) {
		C.X += x;C.Y += y;
	}
}
void vaciarJugadas() {
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 2;j++) {
			MovJ.pos[i][j] = -1;
		}
		MovJ.tipoM[i] = -1;
	}
	for (int i = 0;i < 30;i++) {
		for (int j = 0;j < 2;j++) {
			MovJ.movR[i][j] = -1;
		}
	}
	MovJ.contR = 0;
	MovJ.car = 0;
	MovJ.con = 0;
	MovJ.x = -1;
	MovJ.y = -1;

}
void limpiar(int m[8][8]) {
	//limpio cursor
	C.X = 7;
	C.Y = 4;
	C.ultX = 7;
	C.ultY = 4;
	C.turno = 2;

	//limpios datos jugadores
	strcpy(jugadores[0].nombre, " ");
	jugadores[0].puntos = 0;
	strcpy(jugadores[1].nombre, " ");
	jugadores[0].puntos = 0;
    /*
	int m1[8][8] = { 2,0,2,0,2,0,2,0,
					 0,2,0,2,0,2,0,2,
					 2,0,2,0,2,0,2,0,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,
					 0,1,0,1,0,1,0,1,
					 1,0,1,0,1,0,1,0,
					 0,1,0,1,0,1,0,1, };
*/
	int m1[8][8] = { 0,0,0,0,0,0,0,2,
					 0,0,1,0,0,0,0,2,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,2,0,0,0,
					 0,1,0,1,0,1,0,1,
					 1,0,1,0,2,0,1,0,
					 0,1,0,0,0,0,0,1, };	
	int i, j;
	for (i = 0;i < 8;i++) {
		for (j = 0;j < 8;j++) {
			m[i][j]=0;
			m[i][j] = m1[i][j];
		}
	}

	//vacio jugadas
	vaciarJugadas();
}
//peones jugador 1= -1 nor jugador 2=1 sur 
void cargarJugada(int aux) {
	vaciarJugadas();
	int oponente = (C.turno == 2) ? 1 : 2;
	int reinaop = (C.turno == 2) ? 3 : 4;
	if (limite(C.X + aux, C.Y + 1)) {
		if (matriz[C.X + aux][C.Y + 1] == 0) {
			MovJ.pos[0][0] = C.X + aux;
			MovJ.pos[0][1] = C.Y + 1;
			MovJ.tipoM[0] = 1;
		}
		else if (matriz[C.X + aux][C.Y + 1] == oponente||matriz[C.X + aux][C.Y + 1]==reinaop) {
			if (limite(C.X + aux * 2, C.Y + 2)) {
				if (matriz[C.X + aux * 2][C.Y + 2] == 0) {
					MovJ.pos[0][0] = C.X + aux * 2;
					MovJ.pos[0][1] = C.Y + 2;
					MovJ.tipoM[0] = 2;
					MovJ.elim[0][0] = C.X + aux;
					MovJ.elim[0][1] = C.Y + 1;
				}
				else {
					MovJ.tipoM[0] = -1;
				}
			}
		}
		else {
			MovJ.tipoM[0] = -1;
		}
		MovJ.car = 1;
		MovJ.con = 9;
	}
	else {
		MovJ.tipoM[0] = -1;
	}
	if (limite(C.X + aux, C.Y - 1)) {
		if (matriz[C.X + aux][C.Y - 1] == 0) {
			MovJ.pos[1][0] = C.X + aux;
			MovJ.pos[1][1] = C.Y - 1;
			MovJ.tipoM[1] = 1;
		}
		else if (matriz[C.X + aux][C.Y - 1] == oponente||matriz[C.X + aux][C.Y - 1]==reinaop) {
			if (limite(C.X + aux * 2, C.Y - 2)) {
				if (matriz[C.X + aux * 2][C.Y - 2] == 0) {
					MovJ.pos[1][0] = C.X + aux * 2;
					MovJ.pos[1][1] = C.Y - 2;
					MovJ.tipoM[1] = 2;
					MovJ.elim[1][0] = C.X + aux;
					MovJ.elim[1][1] = C.Y - 1;
				}
				else {
					MovJ.tipoM[1] = -1;
				}
			}
		}
		else {
			MovJ.tipoM[1] = -1;
		}
		MovJ.car = 1;
		MovJ.con = 9;
	}
	else {
		MovJ.tipoM[1] = -1;
	}
}
void limiteR(int x, int y, int i, int j, int mx, int my) {
	int aux1 = 0, aux2 = 0;
	int aux3 = 0, aux4 = 0;
	int op1,ju1;

	if (C.turno == 2) {op1 = 1;ju1=2;}
	else {op1 = 2;ju1=1;}
	int op2,ju2;
	if (C.turno == 2) {op2 = 3;ju2=4;}
	else {op2 = 4;ju2=3;}
	do {
		i += 1;j += 1;
		aux1 = i;aux2 = j;
		aux1 *= mx;aux2 *= my;
		if (limite(x + aux1, y + aux2)) {
			aux3 = ((i + 1) * mx);aux4 = ((j + 1) * my);
			if (matriz[x + aux1][y + aux2] == 0) {
				MovJ.movR[MovJ.contR][0] = x + aux1;
				MovJ.movR[MovJ.contR][1] = y + aux2;
				MovJ.contR++;
			}
			else if ((matriz[x + aux1][y + aux2] == op1 || matriz[x + aux1][y + aux2] == op2) && limite(x + aux3, y + aux4)) {
				if (matriz[x + aux1][y + aux2] == op1 && matriz[x + aux3][y + aux4] == op1 || matriz[x + aux1][y + aux2] == op1 && matriz[x + aux3][y + aux4] == op2 || matriz[x + aux1][y + aux2] == op2 && matriz[x + aux3][y + aux4] == op1 || matriz[x + aux1][y + aux2] == op2 && matriz[x + aux3][y + aux4] == op2) {
					break;
				}
			}else if((matriz[x + aux1][y + aux2] == ju1 || matriz[x + aux1][y + aux2] == ju2) && limite(x + aux3, y + aux4)){
				break;
			}
		}
	} while (limite(x + aux1, y + aux2) != 0);
}
void cargarJugadaReina() {
	vaciarJugadas();
	for (int i = 0;i < 4;i++) {
		switch (i + 1) {
		case 1: {              //+i +j 5 5 6 6
			limiteR(C.X, C.Y, 0, 0, 1, 1);//diagonal bajando
		}break;
		case 2: {               //-i -j 5 5 4 4
			limiteR(C.X, C.Y, 0, 0, -1, -1);//diagonal subiendo
		}break;
		case 3: {
			limiteR(C.X, C.Y, 0, 0, -1, 1);//diagonal-inversa bajando
		}break;
		case 4: {
			limiteR(C.X, C.Y, 0, 0, 1, -1);//diagonal-inversas subiendo
		}break;
		}
	}

}
void mostraMovi() {
	if (matriz[C.X][C.Y] == C.turno) {
		if (C.turno == 2) {
			cargarJugada(1);//jugador verde
		}
		else {
			cargarJugada(-1);//jugador lightblue
		}
	}else if (matriz[C.X][C.Y] == 3 || matriz[C.X][C.Y] == 4) {
		cargarJugadaReina();
		if (MovJ.contR > 0) {
			MovJ.con = 14;
		}
	}
}
int eliminarR(int tipo) {
	int i = 0, aux1 = 0, j = 0, aux2 = 0;
	int op1;
	if (C.turno == 2) { op1 = 1; }
	else { op1 = 2; }
	int op2;
	if (C.turno == 2) { op2 = 3; }
	else { op2 = 4; }
	int cont = 0;
	do {
		i += 1;j += 1;
		aux1 = i;aux2 = j;
		switch (tipo) {
		case 1:aux1 *= -1;break;
		case 3:aux2 *= -1;break;
		case 4: {
			aux1 *= -1;aux2 *= -1;
		}break;
		}
		if (matriz[C.ultX + aux1][C.ultY + aux2] == op1 || matriz[C.ultX + aux1][C.ultY + aux2] == op2) {
			matriz[C.ultX + aux1][C.ultY + aux2] = 0;
			cont++;
		}
	} while ((C.ultX + aux1) != C.X && (C.ultY + aux2) != C.Y);
	return cont;
}
//moverficha 
int moverFicha() {
	if (ExisteMovimientoJugador(C.X, C.Y)) { /*Jugador de la parte Sur*/
		matriz[MovJ.x][MovJ.y] = 0;//Colocar un 0 en la matriz en donde estaba anteriormente el cursor.
		if (MovJ.pos[0][0] == C.X && MovJ.pos[0][1] == C.Y) {
			//valido si corono
			if (C.X == 0 && C.turno == 1 || C.X == 7 && C.turno == 2) {
				if (C.turno == 2) { 
					matriz[MovJ.pos[0][0]][MovJ.pos[0][1]] = 4;
				}
				else { 
					matriz[MovJ.pos[0][0]][MovJ.pos[0][1]] = 3;
				}
			}else {
				matriz[MovJ.pos[0][0]][MovJ.pos[0][1]] = C.turno;
			}
			//valido si elimino ficha
			if (MovJ.tipoM[0] == 2) { /*Validando la nueva ficha que se va a colocar*/
				matriz[MovJ.elim[0][0]][MovJ.elim[0][1]] = 0;
				if (C.turno == 2) {
					jugadores[0].puntos += 1;
				}
				else {
					jugadores[1].puntos += 1;
				}
				return 2;
			}
			return 1;
		}
		else if (MovJ.pos[1][0] == C.X && MovJ.pos[1][1] == C.Y) { /*Jugador de la parte Norte*/
			//valido si corono
			if (C.X == 0 && C.turno == 1 || C.X == 7 && C.turno == 2) {
				if (C.turno == 2) {
					matriz[MovJ.pos[1][0]][MovJ.pos[1][1]] = 4;
				}
				else { 
					matriz[MovJ.pos[1][0]][MovJ.pos[1][1]] = 3;
				}
			}else {
				matriz[MovJ.pos[1][0]][MovJ.pos[1][1]] = C.turno;
			}
			//valido si elimino ficha
			if (MovJ.tipoM[1] == 2) {
				matriz[MovJ.elim[1][0]][MovJ.elim[1][1]] = 0;
				if (C.turno == 2) {
					jugadores[0].puntos += 1;
				}
				else {
					jugadores[1].puntos += 1;
				}
				return 2;
			}
			return 1;
		}
	}
	else if (existeMJR(C.X, C.Y)) {
		matriz[C.ultX][C.ultY] = 0;
		int movRX = (C.ultX - C.X), movRY = (C.ultY - C.Y);
		cout << endl << movRX << " " << movRY << endl;
		int tipoE = 0;
		if (movRX > 0 && movRY < 0) tipoE = 1;
		if (movRX < 0 && movRY < 0) tipoE = 2;
		if (movRX < 0 && movRY>0) tipoE = 3;
		if (movRX > 0 && movRY > 0) tipoE = 4;
		if (eliminarR(tipoE) > 0) {
			if (C.turno == 2) {
				jugadores[0].puntos += 1;
			}
			else {
				jugadores[1].puntos += 1;
			}
		}
		matriz[C.X][C.Y] = (C.turno == 2) ? 4 : 3;
		return 3;
	}
	return 0;
}
void marcarMovimientos() {
	mostraMovi();
	MovJ.x = C.X;MovJ.y = C.Y;//guardo posicion curso	
	C.ultX = C.X;C.ultY = C.Y;//guardo ultimo estado del cursor
}
int victoria(){
    int jugador,reina,valor;
	if(C.turno==2){
		jugador=1;reina=3;valor=-1;
	}else{
		jugador=2;reina=4;valor=1;
	}	
	int movimiento=0,ficha=0;
	int auxX=0,auxY=0;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			auxX=C.X;auxY=C.Y;
			C.X=i;C.Y=j;
            if(matriz[i][j]!=0){
            	if(matriz[i][j]==jugador||matriz[i][j]==reina){
					ficha++;
					if(matriz[i][j]==jugador){
						cargarJugada(valor);
						if(MovJ.tipoM[0]!=-1||MovJ.tipoM[1]!=-1){
							movimiento++;
						}
					}else{
						cargarJugadaReina();
						if(MovJ.contR>0){
							movimiento++;
						}
					} 
				}
			}
			C.X=auxX;C.Y=auxY;
			vaciarJugadas();
		}
	}
	if(ficha==0) return 1;
	if(movimiento==0) return 1;
	return 0;
}

void mover(int m[8][8]) {
	int ch;

	while (1) {

		if (_kbhit()) {
			// cargo ch
			ch = _getch();
			//salto(10);
			if (MovJ.con > 0) MovJ.con--;
			if (MovJ.con == 0) {
				MovJ.con--;
				vaciarJugadas();
			}
			switch ((ch + 32)) {
				//arriba
			case 119:case 104:case 151: {
				posibleM(1);
			}
					break;
					//abajo
			case 115:case 147:case 112: {
				posibleM(2);
			}
					break;
					//derecha
			case 132:case 100:case 109: {
				posibleM(3);
			}
					break;
					//izquierda
			case 97:case 107:case 129: {
				posibleM(4);
			}
				   break;
			case 45: {

				if (MovJ.car > 0 || MovJ.contR > 0) {
					int respM = moverFicha();
					if(victoria()==1){
						cout<<"Victoria:";
						if(C.turno==2){
							cout<<jugadores[1].nombre<<endl;
						}else{
							cout<<jugadores[0].nombre<<endl;
						}
						system("pause");
						return;
					}
					switch (respM) {
					case 1: {
						C.turno = (C.turno == 2) ? 1 : 2;
						vaciarJugadas();
					}break;
					case 2: {
						//seguir comiendo 
						marcarMovimientos();
						//cambiar de turno
						//cout << "p0:" << MovJ.tipoM[0] << "p1:" << MovJ.tipoM[1] << endl;
						//system("pause");
						if (MovJ.tipoM[0] < 0 && MovJ.tipoM[1] < 0 || MovJ.tipoM[0] == 1 && MovJ.tipoM[1] < 0 || MovJ.tipoM[0] < 0 && MovJ.tipoM[1] == 1 || MovJ.tipoM[0] == 1 && MovJ.tipoM[1] == 1) {
							if (C.turno == 2) { C.turno = 1; }
							else { C.turno = 2; }
							vaciarJugadas();
						}
					}break;
					case 3: {
						if (C.turno == 2) { C.turno = 1; }
						else { C.turno = 2; }
						vaciarJugadas();
					}break;
					default: {
						if (C.X == C.ultX && C.Y == C.ultY) {
							vaciarJugadas();
						}
						if (matriz[C.X][C.Y] == 0) {
							vaciarJugadas();
						}
					}break;
					}
				}else if (matriz[C.X][C.Y] == 1 || matriz[C.X][C.Y] == 2) {
					marcarMovimientos();
				}
				if ((matriz[C.X][C.Y] == 3 && C.turno == 1 || matriz[C.X][C.Y] == 4 && C.turno == 2)) {
					marcarMovimientos();
				}

				//cout << MovJ.car << endl;
			}
				   break;
			}
			if ((ch + 32) != 0) {
				system("cls");
				dibujarMarco();
				mostrar(matriz);
			}

			// Terminates the loop
			if ((ch + 32) == 154 || (ch + 32) == 59){
				vaciarJugadas();
				break;
			}
			

			//gotoxy(24, 19);cout << "\nKey pressed= " << (ch + 32);
		}
	}

}
void Instrucciones()
{
	cout << "\nInstrucciones del juego: " << endl;
	cout << "\n1).Cada jugador dispone de 12 fichas de un mismo color (unas blancas y otras negras) que se colocan en casillas negras" << endl;
	cout << "de las tres filas mas proximas a cada jugador." << endl;
	cout << "\n2).Hay que tener en cuenta, eso si, que el tablero se situa de manera quecada jugador tenga una casilla blanca en" << endl;
	cout << "su parte inferior derecha.El objetivo final del juego es capturar todas las fichas del oponente o, al menos, acorralar" << endl;
	cout << "para que los unicos movimientos que pueda hacer son los que lleven a su captura por el contrario." << endl;
	cout << "\n3).Los jugadores se van alternando para realizar los movimientos de sus piezas, empezando el juego aquel que tenga" << endl;
	cout << "las fichas blancas." << endl;
	cout << "\n4).Las piezas se mueven una posicion adelante, nunca hacia atras, en diagonal a la derecha o a la izquierda y a una" << endl;
	cout << "posicion adyacente que se encuentre vacia." << endl;
	cout << "\n5).En el momento de comer las piezas del oponente, estas se pueden comer varias en un mismo turno de forma diagonal" << endl;
	cout << "hacia la derecha o hacia la izquierda para adelante y para atras." << endl;
	cout << "\n6).Para comer una ficha del contrario, tenemos que estar delante de esta ficha y saltar por encima de ella en un" << endl;
	cout << "movimiento en diagonal hasta caer en la casilla siguiente, que debe estar vacia. En el caso de que el contrincante" << endl;
	cout << "tenga varias fichas en diagonal, alternadas con casillas vacias, vamos a poder comerlas a la vez en un solo movimiento." << endl;
	cout << "\n7).Si una de nuestras fichas es capaz de llegar hasta la ultima fila del jugador rival, se convierte en reina y" << endl;
	cout << "colocamos otra ficha encima.Esto va a permitir que esta ficha reina se pueda mover en diagonal por todo el tablero" << endl;
	cout << "tanto hacia delante como hacia atras. La reina tambien puede comer mas de dos piezas seguidas en todas las direcciones." << endl;
	cout << "\n8).Pierde la partida el jugador que se quede sin fichas en el tablero o cuando las piezas que le quedan estan" << endl;
	cout << "bloqueadas y solo pueda moverlas a un lugar en el que seran capturadas por el jugador contrario. Si son los dos" << endl;
	cout << "jugadores los que se quedan con piezas bloqueadas en el tablero, ganara quien tenga mas fichas en ese momento." << endl;
}


void mostrarColores(){
	for(int i=0;i<15;i++){
		Color(i+1);cout<<"C-N:"<<i+1;
		if(i!=0&&i%5==0) cout<<endl;
	}
}

void limpiar() {
	cin.clear();
	fflush(stdin);
	cout << "\n\t\t**Ingreso mal los datos**" << endl;
	cout << "\n" << endl;
	system("pause");
	system("cls");
}
void juego() {
  
	system("cls");
	salto(5);
	limpiar(matriz);
	fflush(stdin);
	Color(2);cout << "\t\tIngrese nombre de Jugador 1" << endl;
	cout << "\t\t-> ";cin.getline(jugadores[0].nombre,9);cin.sync();
	salto(4);
	Color(11);cout << "\t\tIngrese nombre de Jugador 2" << endl;
	cout << "\t\t-> ";cin.getline(jugadores[1].nombre,9);cin.sync();
    Color(15);cout<<"\t\t";cout<<"Jugador 1 juega con negras=8 o blancas=15"<<endl;
    cout << "\t\t-> ";cin>>C.colorJ1;
	if(C.colorJ1==8){
		char aux[15];
		strcpy(aux,jugadores[0].nombre);
		strcpy(jugadores[0].nombre,jugadores[1].nombre);
		strcpy(jugadores[1].nombre,aux);
	}
	C.turno=1;
	mostrarColores();Color(15);
	cout<<endl<<"\t\t";cout<<"Ingrese el color del tablero:"<<endl;
	cout << "\t\t-> ";cin>>C.colorT;
	system("cls");
	dibujarMarco();
	mostrar(matriz);
	salto(10);
	//juego
	mover(matriz);
	//simacion animacion final
}
int main()
{
	char opcion;

	cout << "\n" << endl;
	int Opciones;
	do {
		Color(14);
		system("cls");
		cout << "\n\t\t**BIENVENIDO AL MENU DE OPCIONES**" << endl;
		cout << "\t\t==================================" << endl;
		cout << "\n\t\t1).Instrucciones del juego" << endl;
		cout << "\n\t\t2).Jugar" << endl;
		cout << "\n\t\t3).Salir Programa" << endl;
		cout << "\t\t==================================" << endl;
		cout << "\nOpcion elegida-> ";
		cin >> Opciones;
		system("cls");

		if (!cin.fail()) {
			switch (Opciones)
			{
			case 1:
			{
				int r = 0;
				do {
					system("cls");
					Instrucciones();
					cout << "\n\n\tPresionar[1] para salir: ";
					cin >> r;
					if (cin.fail())
					{
						limpiar();
					}
				} while (r != 1);

			};break;

			case 2:
			{
				juego();
			};break;

			}
		}

		else {
			Opciones = 0;
			limpiar();  //limpia  todo el buffer  //cin.ignore(); solo iGnora un caracter malo
		}

	} while (Opciones != 3);


	return 0;
}
