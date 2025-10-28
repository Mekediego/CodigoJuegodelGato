//Diego Chacón - Programación dinámica
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <locale>
//Iniciamos, Inge
using namespace std;

vector<string> tablero = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
string nombreJugador1, nombreJugador2;

// Muestra el tablero durante el juego
void mostrarTablero() {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        cout << " " << tablero[i] << " ";
        if ((i + 1) % 3 == 0) {
            cout << "\n";
            if (i < 8) cout << "---+---+---\n";
        } else {
            cout << "|";
        }
    }
    cout << "\n";
}

// Muestra el tablero final con solo X y O
void mostrarTableroFinal() {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        string celda = (tablero[i] == "X" || tablero[i] == "O") ? tablero[i] : " ";
        cout << " " << celda << " ";
        if ((i + 1) % 3 == 0) {
            cout << "\n";
            if (i < 8) cout << "---+---+---\n";
        } else {
            cout << "|";
        }
    }
    cout << "\n";
}

// Reinicia el tablero con números del 1 al 9
void reiniciarTablero() {
    for (int i = 0; i < 9; ++i)
        tablero[i] = to_string(i + 1);
}

// Verifica si hay un ganador
string verificarGanador() {
    string combinaciones[8][3] = {
        {"1","2","3"}, {"4","5","6"}, {"7","8","9"},
        {"1","4","7"}, {"2","5","8"}, {"3","6","9"},
        {"1","5","9"}, {"3","5","7"}
    };

    for (auto& c : combinaciones) {
        string a = tablero[stoi(c[0]) - 1];
        string b = tablero[stoi(c[1]) - 1];
        string c_ = tablero[stoi(c[2]) - 1];
        if (a == b && b == c_) {
            return a;
        }
    }
    return "";
}

// Verifica si hay empate
bool esEmpate() {
    for (auto& celda : tablero)
        if (celda != "X" && celda != "O")
            return false;
    return true;
}

// Verifica si la jugada es válida
bool esJugadaValida(int posicion) {
    return posicion >= 1 && posicion <= 9 && tablero[posicion - 1] != "X" && tablero[posicion - 1] != "O";
}

// Turno del jugador 1
void turnoJugador1() {
    int posicion;
    do {
        cout << nombreJugador1 << " (X), elige una posición (1-9): ";
        cin >> posicion;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            posicion = -1;
        }
    } while (!esJugadaValida(posicion));
    tablero[posicion - 1] = "X";
}

// Turno del jugador 2 o CPU
void turnoJugador2(bool esCPU) {
    int posicion;
    if (esCPU) {
        this_thread::sleep_for(chrono::milliseconds(800));
        do {
            posicion = rand() % 9 + 1;
        } while (!esJugadaValida(posicion));
        cout << "CPU eligió la posición " << posicion << "\n";
    } else {
        do {
            cout << nombreJugador2 << " (O), elige una posición (1-9): ";
            cin >> posicion;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                posicion = -1;
            }
        } while (!esJugadaValida(posicion));
    }
    tablero[posicion - 1] = "O";
}

// Valida nombres con acentos y ñ
bool contieneSoloLetrasValidas(const string& texto) {
    regex patron("^[a-zA-ZáéíóúÁÉÍÓÚñÑ\\s]+$");
    return regex_match(texto, patron);
}

// Solicita nombre con validación
string solicitarNombre(const string& etiqueta) {
    string nombre;
    cin.ignore();
    do {
        cout << "Ingresa el nombre del " << etiqueta << ": ";
        getline(cin, nombre);
    } while (!contieneSoloLetrasValidas(nombre));
    return nombre;
}

// Juego principal
void jugar(bool vsCPU) {
    reiniciarTablero();
    string ganador = "";
    string turno = "X";
    mostrarTablero();

    while (ganador == "") {
        if (turno == "X") {
            turnoJugador1();
        } else {
            turnoJugador2(vsCPU);
        }

        mostrarTablero();
        ganador = verificarGanador();
        if (ganador != "") {
            string nombreGanador = (ganador == "X") ? nombreJugador1 : nombreJugador2;
            cout << "\n🎉 ¡Ganador: " << nombreGanador << " (" << ganador << ")!\n";
            mostrarTableroFinal();
            break;
        } else if (esEmpate()) {
            cout << "\n🤝 ¡Empate!\n";
            mostrarTableroFinal();
            break;
        }
        turno = (turno == "X") ? "O" : "X";
    }
}

int main() {
    system("chcp 65001");
    locale::global(locale(""));
    srand(time(0));

    int opcion;
    do {
        cout << "\n=== Menú Tic Tac Toe ===\n";
        cout << "1. Jugar 2 jugadores\n";
        cout << "2. Jugar contra CPU\n";
        cout << "3. Salir\n";
        cout << "Selecciona una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                nombreJugador1 = solicitarNombre("Jugador 1");
                nombreJugador2 = solicitarNombre("Jugador 2");
                jugar(false);
                break;
            case 2:
                nombreJugador1 = solicitarNombre("Jugador");
                nombreJugador2 = "CPU";
                jugar(true);
                break;
            case 3:
                cout << "¡Hasta luego!\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 3);

    return 0;
}