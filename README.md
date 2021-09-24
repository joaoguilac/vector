# Trabalho 03: Implementação do Contêiner Sequencial Dinâmico `vector`

# Apresentação

Nesse trabalho você terá a oportunidade de desenvolver um contêiner sequencial dinâmico que implementa o comportamento de um Tipo Abstrato de Dados (TAD) **Lista**.

Nesse repositório você vai encontrar os testes unitários que serão utilizados na avaliação do seu projeto e o _esqueleto_ do cabeçalho onde você deve implementar a classe template `sc::vector`.

As instruções sobre o que deve ser implementado e como deve ser implementado você pode encontrar [**aqui**](docs/projeto_vector.pdf).

Você precisa fornecer algumas informações no arquivo [author.md](author.md), indicando a autoria do trabalho e quais itens da tarefa foram realizados com sucesso. 


# Compiling

This project may be compiled with [CMake](https://cmake.org). CMake is an open-source, cross-platform family of tools designed to build, test and package software. Basically, cmake reads a script (`CMakeLists.txt`) with meta-information on the project and **creates a Makefile automatically**, tailored to your system configuration.
After that, you may run the `make` command, as usual.
Alternatively, the same script might be used to create Xcode projects, Ninja, or Microsoft Visual Studio projects, for instance.

CMake supports **out-of-source** build. This means the _source code_ is stored in **one** folder and the _generated executable files_ should be stored in **another** folder: project should never mix-up the source tree with the build tree.

In particular, this project creates a single **target** (executable), called `all_tests`.

But don't worry, they are already set up in the `CMakeLists.txt` script.

To compile this project with [cmake](https://cmake.org) follow these steps (from the root folder):

1. `cmake -S source -B build`:  asks cmake to create the build directory and generate the Unix Makefile based on the script found in `source/CMakeLists.txt`, on the current level.
3. `cmake --build build`: triggers the compiling process that creates both targets (executable).D inside `build`.

The executable is created inside the `build` directory.

For further details, please refer to the [cmake documentation website](https://cmake.org/cmake/help/v3.14/manual/cmake.1.html).

**NOTE** however, that this project **may compile at first** but the tests will most certainly fail.

We have in this project two batches of tests: 24 unit tests for the `sc::list` class, and 9 unit tests for the `sc::list::iterator` class, and 22 tests for the utility operations. You should try to get passed all these tests.

## Compiling without cmake

If you wish to compile this project without the cmake, create the `build` folder manually (`mkdir build`), then try to run the command below:

```
g++ -Wall -std=c++11 -I source/include -I source/tests/tm source/tests/main.cpp source/tests/tm/test_manager.cpp -o build/all_tests
```

# Running

From the project's root folder, run as usual (assuming `$` is the terminal prompt):

```
$ ./build/tests/all_tests (with cmake)
$ ./build/all_tests (without cmake)
```

# Driver

To run the driver you should run the command below:

```
$ g++ -Wall -std=c++11 source/src/driver_vector.cpp -o build/driver (compiling)
$ ./build/driver (running)
```

--------
&copy; DIMAp/UFRN 2021.
