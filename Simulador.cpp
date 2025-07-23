#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>

using namespace std;

struct Equipo {
    string nombre;
    int puntos;
    int golesFavor;
    int golesContra;

    Equipo() : puntos(0), golesFavor(0), golesContra(0) {}
    Equipo(string n) : nombre(n), puntos(0), golesFavor(0), golesContra(0) {}
};

struct Partido {
    Equipo* local;
    Equipo* visitante;
    int golesLocal;
    int golesVisitante;

    Partido(Equipo* l, Equipo* v) : local(l), visitante(v), golesLocal(-1), golesVisitante(-1) {}
};

struct PartidoEliminatoria {
    string equipoA;
    string equipoB;
    int golesA;
    int golesB;
    string ronda;
};

bool existeEquipo(const vector<Equipo>& equipos, const string& nombre) {
    for (const auto& e : equipos)
        if (e.nombre == nombre)
            return true;
    return false;
}

void registrarEquipos(vector<Equipo>& equipos, int cantidad) {
    cout << "Registro de equipos manual\n";
    for (int i = 0; i < cantidad; ++i) {
        string nombre;
        cout << "Nombre del equipo #" << (i + 1) << ": ";
        cin >> ws;
        getline(cin, nombre);
        if (existeEquipo(equipos, nombre)) {
            cout << "Error: el equipo ya existe. Intente otro nombre.\n";
            --i;
        } else {
            equipos.push_back(Equipo(nombre));
        }
    }
}

bool cargarEquiposDesdeCSV(vector<Equipo>& equipos, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << nombreArchivo << "\n";
        return false;
    }
    string linea;
    int lineaNum = 0;
    while (getline(archivo, linea)) {
        lineaNum++;
        if (linea.empty()) continue;
        stringstream ss(linea);
        string nombre;
        if (!getline(ss, nombre, ',')) {
            cerr << "Línea " << lineaNum << " con formato inválido.\n";
            continue;
        }
        if (existeEquipo(equipos, nombre)) {
            cerr << "Advertencia: equipo repetido '" << nombre << "' en línea " << lineaNum << ".\n";
            continue;
        }
        equipos.push_back(Equipo(nombre));
    }
    archivo.close();
    return !equipos.empty();
}

vector<Partido> generarPartidos(vector<Equipo>& equipos) {
    vector<Partido> calendario;
    for (size_t i = 0; i < equipos.size(); ++i) {
        for (size_t j = i + 1; j < equipos.size(); ++j) {
            calendario.push_back(Partido(&equipos[i], &equipos[j]));
        }
    }
    return calendario;
}

void simularPartidos(vector<Partido>& partidos) {
    for (auto& p : partidos) {
        p.golesLocal = rand() % 11;
        p.golesVisitante = rand() % 11;
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

void mostrarTabla(const vector<Equipo>& equipos) {
    vector<Equipo> temp = equipos;
    sort(temp.begin(), temp.end(), [](const Equipo& a, const Equipo& b) {
        if (a.puntos != b.puntos) return a.puntos > b.puntos;
        int gdA = a.golesFavor - a.golesContra;
        int gdB = b.golesFavor - b.golesContra;
        return gdA > gdB;
    });
    cout << "\n--- TABLA DE POSICIONES ---\n";
    cout << "Equipo\tPuntos\tGF\tGC\tGD\n";
    for (auto& e : temp) {
        int gd = e.golesFavor - e.golesContra;
        cout << e.nombre << "\t" << e.puntos << "\t" << e.golesFavor << "\t" << e.golesContra << "\t" << gd << "\n";
    }
}

void exportarResultados(const vector<Partido>& partidos, const string& archivo) {
    ofstream f(archivo);
    if (!f.is_open()) {
        cerr << "Error al crear " << archivo << "\n";
        return;
    }
    f << "Local,Visitante,GolesLocal,GolesVisitante\n";
    for (auto& p : partidos) {
        f << p.local->nombre << "," << p.visitante->nombre << "," << p.golesLocal << "," << p.golesVisitante << "\n";
    }
    f.close();
}

void exportarTabla(const vector<Equipo>& equipos, const string& archivo) {
    ofstream f(archivo);
    if (!f.is_open()) {
        cerr << "Error al crear " << archivo << "\n";
        return;
    }
    f << "Equipo,Puntos,GF,GC,GD\n";
    for (auto& e : equipos) {
        f << e.nombre << "," << e.puntos << "," << e.golesFavor << "," << e.golesContra
          << "," << (e.golesFavor - e.golesContra) << "\n";
    }
    f.close();
}

string nombreRonda(int cantidad) {
    map<int, string> rondas = {
        {16, "Octavos de final"},
        {8,  "Cuartos de final"},
        {4,  "Semifinal"},
        {2,  "Final"},
        {1,  "Campeón"}
    };
    return rondas.count(cantidad) ? rondas[cantidad] : "Ronda de " + to_string(cantidad);
}

Equipo* simularFaseEliminatoria(vector<Equipo*>& clasificados, vector<PartidoEliminatoria>& resultados, int cantidadEquipos) {
    if (clasificados.size() == 1) return clasificados[0];
    vector<Equipo*> ganadores;
    string ronda = nombreRonda(cantidadEquipos);
    cout << "\n-- " << ronda << " --\n";
    for (size_t i = 0; i < clasificados.size(); i += 2) {
        Equipo* a = clasificados[i];
        Equipo* b = clasificados[i + 1];
        int ga = rand() % 11;
        int gb = rand() % 11;
        cout << a->nombre << " " << ga << " - " << gb << " " << b->nombre << "\n";
        if (ga >= gb) ganadores.push_back(a);
        else ganadores.push_back(b);

        resultados.push_back({a->nombre, b->nombre, ga, gb, ronda});
    }
    return simularFaseEliminatoria(ganadores, resultados, cantidadEquipos / 2);
}

void exportarEliminatoriaCSV(const vector<PartidoEliminatoria>& resultados, const string& archivo) {
    ofstream f(archivo);
    if (!f.is_open()) {
        cerr << "Error al crear " << archivo << "\n";
        return;
    }
    f << "Ronda,Equipo A,Goles A,Equipo B,Goles B\n";
    for (const auto& p : resultados) {
        f << p.ronda << "," << p.equipoA << "," << p.golesA << "," << p.equipoB << "," << p.golesB << "\n";
    }
    f.close();
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    vector<Equipo> equipos;
    vector<Partido> partidos;
    vector<PartidoEliminatoria> eliminatoriaResultados;

    int opcion;
    do {
        cout << "\n--- MENÚ PRINCIPAL ---\n";
        cout << "1) Cargar equipos desde CSV\n";
        cout << "2) Registrar equipos manualmente\n";
        cout << "3) Simular fase de grupos y mostrar tabla\n";
        cout << "4) Ejecutar fase eliminatoria\n";
        cout << "5) Exportar resultados y tabla finales\n";
        cout << "0) Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        switch (opcion) {
            case 1: {
                string archivo;
                cout << "Nombre del CSV: "; cin >> archivo;
                if (!cargarEquiposDesdeCSV(equipos, archivo))
                    cout << "No se cargaron equipos.\n";
                break;
            }
            case 2: {
                int n;
                cout << "Cantidad de equipos a registrar: "; cin >> n;
                registrarEquipos(equipos, n);
                break;
            }
            case 3: {
                if (equipos.size() < 2) {
                    cout << "Se necesitan al menos 2 equipos.\n";
                    break;
                }
                partidos = generarPartidos(equipos);
                simularPartidos(partidos);
                mostrarTabla(equipos);
                break;
            }
            case 4: {
                if (equipos.empty()) {
                    cout << "Primero simule la fase de grupos (opción 3).\n";
                    break;
                }
                eliminatoriaResultados.clear();
                vector<Equipo> copia = equipos;
                sort(copia.begin(), copia.end(), [](const Equipo& a, const Equipo& b) { return a.puntos > b.puntos; });
                int n = 1;
                while (n * 2 <= (int)copia.size()) n *= 2;
                vector<Equipo*> clasificados;
                for (int i = 0; i < n; ++i) clasificados.push_back(&copia[i]);
                Equipo* campeon = simularFaseEliminatoria(clasificados, eliminatoriaResultados, n);
                cout << "\n=== ¡El campeón del torneo es " << campeon->nombre << "! ===\n";
                exportarEliminatoriaCSV(eliminatoriaResultados, "fase_eliminatoria.csv");
                cout << "Fase eliminatoria exportada a 'fase_eliminatoria.csv'\n";
                break;
            }
            case 5: {
                if (partidos.empty()) partidos = generarPartidos(equipos);
                exportarResultados(partidos, "resultados.csv");
                exportarTabla(equipos, "tabla.csv");
                cout << "Exportación completada.\n";
                break;
            }
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
