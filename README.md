# Project of Formal Languages and Automata

## Introduction

A simple implementation of PushDown Automata Simulator and Turing Machine Simulator, with CLI support.

## Usage

```txt
fla
A CLI tool to parse and simulate PDA or TM

Usage: fla[EXE] <PATH> [INPUT]

Arguments:
  PATH   Path to the PDA/TM syntax file. Must end with `.pda` or `.tm`
  INPUT  Input for the PDA or TM. Reads from stdin if not provided

Options:
  -c, --check    Check the grammar of the syntax file
  -d, --debug    Print the parameters of the PDA/TM
  -v, --verbose  Display the entire simulation process in details
  -r, --rich     Enable rich style output
  -h, --help     Print help
```

- Parse model and check it:

```sh
xmake run fla path/to/model.pda -c
```

- In verbose mode and print model:

```sh
xmake run fla path/to/model.tm -c -d -v
```

- Run some input on the model:

```sh
xmake run fla path/to/model.pda your_input
```

- Run some input (from stdin) on the model:

```sh
xmake run fla path/to/model.pda
```

- Run some input on the model in verbose and rich mode:

```sh
xmake run fla path/to/model.tm your_input -v -r
```

For CMake users, Good Luck!

## Development

Requires [XMake](https://xmake.io/) and a compiler supporting C++17.

- Build project:

```sh
xmake build
```

- Run project (for full usage, see `--help`):

```sh
xmake run fla
```

- For CMake fans, you can generate `CMakeLists.txt` in this way:

```sh
xmake project -k cmakelists
```

- Then build with CMake:

```sh
cmake -B build
cd ./build; make
```

## Testing

The fixtures are from <https://github.com/NijikaIjichi/nju-fla-turing>.
