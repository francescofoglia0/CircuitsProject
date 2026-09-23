# C++ Object-Oriented Circuit Solver

Un simulatore di circuiti elettrici basato su architettura Object-Oriented e scritto in **C++17 moderno**. 

Questo progetto analizza topologie di circuiti elettrici utilizzando l'**Analisi Nodale Modificata (MNA)** per assemblare il sistema lineare sottostante (Ax = b). Il sistema viene poi risolto in modo efficiente utilizzando un'implementazione personalizzata dell'algoritmo del **Gradiente Coniugato (CG)**, ottimizzato tramite le strutture dati della libreria **Eigen3**.

## Funzionalità Principali

*   **Paradigma RAII e Memory Safety:** Gestione dinamica dei componenti del circuito tramite `std::unique_ptr`. La memoria viene allocata e deallocata automaticamente, garantendo l'assenza totale di memory leak (validato tramite AddressSanitizer).
*   **Architettura Object-Oriented:** Design polimorfico con una classe base `Component` facilmente estendibile per supportare resistori, condensatori, induttori e generatori.
*   **Risolutore Numerico Custom:** Implementazione da zero del metodo iterativo del Gradiente Coniugato per la risoluzione del sistema lineare, ideale per matrici di grandi dimensioni (sparse).
*   **Build System Robusto:** Configurato con **CMake**, include opzioni per la compilazione con massima ottimizzazione (`-O3`) e profiling della memoria (`-fsanitize=address`).

## Prerequisiti

Per compilare ed eseguire il progetto, assicurati di avere installati nel tuo sistema:

*   Un compilatore che supporti **C++17** (GCC, Clang o MSVC)
*   **CMake** (versione 3.20 o superiore)
*   Libreria **Eigen3** (per l'algebra lineare)

## Compilazione ed Esecuzione

Il progetto utilizza CMake come sistema di build. Segui questi step per compilare il codice:

1. Clona il repository e naviga nella cartella del progetto:
   ```bash
   git clone [https://github.com/tuo-username/circuit-solver.git](https://github.com/tuo-username/circuit-solver.git)
   cd circuit-solver
   ```

2. Crea una directory di build ed esegui CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
   *(Nota: AddressSanitizer è abilitato di default. Per disabilitarlo per test di performance pure, usa `cmake .. -DENABLE_ASAN=OFF`)*

3. Compila il progetto:
   ```bash
   make
   ```

4. Esegui il simulatore:
   ```bash
   ./circuit_sim
   ```

## Esempio di Utilizzo (API)

L'interfaccia utente è progettata per essere intuitiva. Ecco come definire un circuito e calcolare le tensioni nodali:

```cpp
#include "Circuit.hpp"

int main() {
    // 1. Inizializza un circuito con 2 nodi (il nodo 0 è sempre la massa/GND)
    Circuit myCircuit(2);

    // 2. Costruisci la topologia
    myCircuit.addResistor(1, 0, 10.0); // Resistor da 10Ω tra Nodo 1 e GND
    myCircuit.addResistor(1, 2, 5.0);  // Resistor da 5Ω tra Nodo 1 e Nodo 2
    myCircuit.addResistor(2, 0, 20.0); // Resistor da 20Ω tra Nodo 2 e GND

    // 3. Inserisci i generatori
    myCircuit.addCurrentSource(1, 2.0); // Generatore di corrente da 2A nel Nodo 1

    // 4. Risolvi il sistema
    Eigen::VectorXd node_voltages = myCircuit.solve();
    
    return 0;
}
```

## Struttura del Progetto

*   `CMakeLists.txt`: Configurazione del sistema di build, linking di Eigen3 e flag di compilazione.
*   `Circuit.hpp`: Strutture dati Object-Oriented (`Component`, `Resistor`, `Circuit`) e logica di assemblaggio della matrice MNA.
*   `Solver.hpp`: Namespace matematico contenente l'algoritmo del Gradiente Coniugato.
*   `main.cpp`: Entry point dell'applicazione, setup della topologia di test e stampa dei risultati.

## Contesto Accademico

Progetto sviluppato come applicazione pratica dei concetti di ingegneria del software e calcolo scientifico (Politecnico di Torino). Le aree di focus includono: principi SOLID in C++, smart pointers, complessità computazionale degli algoritmi iterativi e High Performance Computing (HPC).
