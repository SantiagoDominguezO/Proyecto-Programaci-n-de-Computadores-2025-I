# Simulador de Torneo de Fútbol

Proyecto de clase de Programación de Computadores 2025-I UNAL

**Autores**:

* Jose Santiago Dominguez Ortiz ([jdominguezo@unal.edu.co](mailto:jdominguezo@unal.edu.co))
* Cristian Camilo Castillo Aguirre ([crcastilloa@unal.edu.co](mailto:crcastilloa@unal.edu.co))

**Profesor**: David Alberto Herrera Alvarez

---

## Descripción

Este proyecto es un simulador completo de torneo de fútbol escrito en C++. Permite:

* Registrar equipos manualmente o cargarlos desde un archivo CSV.
* Simular la fase de grupos (round-robin) con resultados pseudoaleatorios (0–10 goles).
* Mostrar y exportar la tabla de posiciones.
* Ejecutar fases eliminatorias (Octavos, Cuartos, Semifinal, Final) mediante recursión.
* Exportar resultados de eliminación a CSV y mostrar el campeón.
* Gestionar errores de entrada y archivos corruptos.

El objetivo es aplicar buenas prácticas de programación estructurada, manejo de archivos, y control de versiones colaborativo.

---

## Estructura del repositorio

```plaintext
├── docs/
│   ├── PC_Proyecto_Entrega_1_Solo.pdf      # Especificación inicial (Entrega 1)
│   └── PC_Proyecto_Entrega_2_Equipo_69.pdf  # Documento completo de requisitos y prototipos
├── datasets/                               # Ejemplos de CSV de entrada
│   └── equipos.csv                         # Lista de equipos (una por línea, o nombre,otras columnas)
├── src/                                    # Código fuente
│   └── main.cpp                            # Implementación completa del simulador
├── bin/                                    # Ejecutable compilado
│   └── simulador                           # Binary output
├── results/                                # Salida de ejemplo
│   ├── resultados.csv                      # Fase de grupos
│   ├── tabla.csv                           # Tabla de posiciones
│   └── fase_eliminatoria.csv               # Resultados de eliminación
└── README.md                               # Esta documentación
```

---

## Compilación

Se requiere un compilador C++17 (o superior). Desde la raíz del repositorio:

```bash
mkdir -p bin
g++ src/main.cpp -o bin/simulador -std=c++17
```

---

## Uso

Ejecuta el programa y sigue el menú:

```bash
./bin/simulador
```

Opciones disponibles:

1. **Cargar equipos desde CSV**: Introduce ruta a `datasets/equipos.csv` (formato: nombre\_por\_línea o CSV con nombres en la primera columna).
2. **Registrar equipos manualmente**: Ingresa N y luego nombres únicos.
3. **Simular fase de grupos y mostrar tabla**: Genera resultados aleatorios y muestra la clasificación.
4. **Ejecutar fase eliminatoria**: Realiza Octavos, Cuartos, Semifinal y Final de forma recursiva; muestra partidos y declara campeón.
5. **Exportar resultados y tabla finales**: Crea `results/resultados.csv` y `results/tabla.csv`.
6. **Salir**

Al ejecutar la fase eliminatoria, se genera también `results/fase_eliminatoria.csv` con columnas: Ronda, Equipo A, Goles A, Equipo B, Goles B.

---

## Ejemplos

* Cargar CSV y simular todo en un solo run:

  1. Opción 1: `datasets/equipos.csv`
  2. Opción 3: fase de grupos
  3. Opción 4: fase eliminatoria
  4. Opción 5: exportar CSV

* Registrar manualmente 8 equipos y simular directamente:

  1. Opción 2: ingresa 8 nombres
  2. Opción 3 y 4

---

## Buenas prácticas de Git

* Cada nueva funcionalidad en **branches** (`feat/...`, `fix/...`).
* Pull requests descriptivos y revisiones antes de merge.
* Commits con mensajes claros: `feat: cargar CSV`, `refactor: simulación eliminatoria`.

---

## Historial de entregas

1. **Entrega 1**: `docs/PC_Proyecto_Entrega_1_Solo.pdf` — Especificación.
2. **Entrega 2**: Código funcional con fase de grupos, carga manual y CSV.
3. **Entrega 3**: Integración de fase eliminatoria recursiva y exportación CSV (actual).

---

¡Gracias por revisar este proyecto!

