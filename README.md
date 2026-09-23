# C++ Object-Oriented Circuit Solver

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Eigen](https://img.shields.io/badge/Eigen3-Computational_Math-blue?style=for-the-badge)

An Object-Oriented electrical circuit simulator written in **modern C++17**. 

This project parses and models electrical circuit topologies using **Modified Nodal Analysis (MNA)** to assemble the underlying linear system ($Ax = b$). The system is then efficiently solved using a custom-built implementation of the **Conjugate Gradient (CG)** algorithm, optimized via **Eigen3** data structures.

## Main Features

*   **RAII Paradigm & Memory Safety:** Dynamic management of circuit components via `std::unique_ptr`. Memory is automatically allocated and deallocated, guaranteeing zero memory leaks (strictly validated via AddressSanitizer).
*   **Object-Oriented Architecture:** Polymorphic design featuring a base `Component` class, easily extensible to support resistors, capacitors, inductors, and voltage/current sources.
*   **Custom Numerical Solver:** Built-from-scratch implementation of the Conjugate Gradient iterative method for solving the linear system, highly efficient for large-scale sparse matrices.
*   **Robust Build System:** Configured with **CMake**, including dedicated flags for maximum compiler optimization (`-O3`) and memory profiling (`-fsanitize=address`).

## Prerequisites

To build and run the project, ensure you have the following installed on your system:

*   A compiler supporting **C++17** (GCC, Clang, or MSVC)
*   **CMake** (version 3.20 or higher)
*   **Eigen3** library (for linear algebra operations)

## Build and Run

The project uses CMake as its build system. Follow these steps to compile the code:

1. Clone the repository and navigate to the project directory:
   ```bash
   git clone [https://github.com/francescofoglia0/circuitsproject.git](https://github.com/francescofoglia0/circuitsproject.git)
   cd circuitsproject
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
   *(Note: AddressSanitizer is enabled by default. To disable it for pure performance benchmarking, use `cmake .. -DENABLE_ASAN=OFF`)*

3. Compile the project:
   ```bash
   make
   ```

4. Run the simulator:
   ```bash
   ./circuit_sim
   ```

## Usage Example (API)

The API is designed to be intuitive. Here is how to define a circuit and compute nodal voltages:

```cpp
#include "Circuit.hpp"

int main() {
    // 1. Initialize a circuit with 2 nodes (Node 0 is always Ground/GND)
    Circuit myCircuit(2);

    // 2. Build the network topology
    myCircuit.addResistor(1, 0, 10.0); // 10Ω resistor between Node 1 and GND
    myCircuit.addResistor(1, 2, 5.0);  // 5Ω resistor between Node 1 and Node 2
    myCircuit.addResistor(2, 0, 20.0); // 20Ω resistor between Node 2 and GND

    // 3. Add power sources
    myCircuit.addCurrentSource(1, 2.0); // 2A current source at Node 1

    // 4. Solve the system
    Eigen::VectorXd node_voltages = myCircuit.solve();
    
    return 0;
}
```

## Project Structure

*   `CMakeLists.txt`: Build system configuration, Eigen3 linking, and compiler flags setup.
*   `Circuit.hpp`: Object-Oriented data structures (`Component`, `Resistor`, `Circuit`) and MNA matrix assembly logic.
*   `Solver.hpp`: Mathematical namespace containing the custom Conjugate Gradient algorithm.
*   `main.cpp`: Application entry point, test topology setup, and results output.

## Academic Context

This project was developed as a practical application of software engineering and scientific computing concepts during my BSc in Mathematical Engineering at Politecnico di Torino. Key focus areas include: SOLID principles in modern C++, smart pointers, computational complexity of iterative algorithms, and High-Performance Computing (HPC) practices.
