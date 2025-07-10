#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Equipo {
    string nombre;
    int puntos = 0;
    int golesFavor = 0;
    int golesContra = 0;
};

struct Partido {
    Equipo* local;
    Equipo* visitante;
    int golesLocal = -1;
    int golesVisitante = -1;
};

// Verifica si un nombre de equipo ya existe
bool existeEquipo(const vector<Equipo>& equipos, const string& nombre) {
    for (const auto& eq : equipos)
        if (eq.nombre == nombre)
            return true;
    return false;
}

// Registra equipos manualmente
void registrarEquipos(vector<Equipo>& equipos, int cantidad) {
    cout << "Registro de equipos:\n";
    for (int i = 0; i < cantidad; ++i) {
        string nombre;
        cout << "Nombre del equipo #" << i + 1 << ": ";
        cin >> nombre;
        if (existeEquipo(equipos, nombre)) {
            cout << "Error: el equipo ya existe. Intente con otro nombre.\n";
            --i;
        } else {
            Equipo nuevoEquipo;
nuevoEquipo.nombre = nombre;
equipos.push_back(nuevoEquipo);
        }
    }
}

// Genera partidos round-robin
vector<Partido> generarPartidos(vector<Equipo>& equipos) {
    vector<Partido> calendario;
    for (size_t i = 0; i < equipos.size(); ++i) {
        for (size_t j = i + 1; j < equipos.size(); ++j) {
            Partido partido;
partido.local = &equipos[i];
partido.visitante = &equipos[j];
calendario.push_back(partido);
        }
    }
    return calendario;
}

// Simula los resultados de los partidos
void simularPartidos(vector<Partido>& partidos) {
    srand(time(nullptr));
    for (auto& p : partidos) {
        p.golesLocal = rand() % 11;
        p.golesVisitante = rand() % 11;

        // Actualiza estadísticas
        p.local->golesFavor += p.golesLocal;
        p.local->golesContra += p.golesVisitante;
        p.visitante->golesFavor += p.golesVisitante;
        p.visitante->golesContra += p.golesLocal;

        if (p.golesLocal > p.golesVisitante)
            p.local->puntos += 3;
        else if (p.golesLocal < p.golesVisitante)
            p.visitante->puntos += 3;
        else {
            p.local->puntos += 1;
            p.visitante->puntos += 1;
        }
    }
}

// Muestra tabla de posiciones
void mostrarTabla(const vector<Equipo>& equipos) {
    cout << "\nTabla de posiciones:\n";
    cout << "Equipo\tPuntos\tGF\tGC\tGD\n";
    for (const auto& e : equipos) {
        int gd = e.golesFavor - e.golesContra;
        cout << e.nombre << "\t" << e.puntos << "\t" << e.golesFavor << "\t" << e.golesContra << "\t" << gd << "\n";
    }
}

// Exporta resultados a archivo CSV
void exportarResultados(const vector<Partido>& partidos, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << "\n";
        return;
    }

    archivo << "Local,Visitante,Goles Local,Goles Visitante\n";
    for (const auto& p : partidos) {
        archivo << p.local->nombre << "," << p.visitante->nombre << "," << p.golesLocal << "," << p.golesVisitante << "\n";
    }

    archivo.close();
}

// Exporta tabla de posiciones a archivo CSV
void exportarTabla(const vector<Equipo>& equipos, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << "\n";
        return;
    }

    archivo << "Equipo,Puntos,GF,GC,GD\n";
    for (const auto& e : equipos) {
        archivo << e.nombre << "," << e.puntos << "," << e.golesFavor << "," << e.golesContra << "," << (e.golesFavor - e.golesContra) << "\n";
    }

    archivo.close();
}

int main() {
    vector<Equipo> equipos;
    vector<Partido> partidos;
    int cantidad;

    cout << "Ingrese la cantidad de equipos: ";
    cin >> cantidad;

    if (cantidad < 2) {
        cerr << "Error: se requieren al menos 2 equipos para simular un torneo.\n";
        return 1;
    }

    registrarEquipos(equipos, cantidad);
    partidos = generarPartidos(equipos);
    simularPartidos(partidos);

    mostrarTabla(equipos);
    exportarResultados(partidos, "resultados.csv");
    exportarTabla(equipos, "tabla.csv");

    cout << "\nSimulación completada. Resultados exportados a 'resultados.csv' y 'tabla.csv'.\n";

    return 0;
}
