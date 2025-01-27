# MIPS Simulator

## Overview

The **MIPS Simulator** is a functional tool designed to emulate the core functionalities of the MIPS I architecture. This project provides a hands-on, interactive way to explore and understand computer architecture concepts by assembling and executing MIPS assembly code. It allows users to:

- **Write and Execute**: Translate MIPS assembly into tangible results.
- **Analyze Program Behavior**: Observe register modifications, memory interactions, and control flow.
- **Experiment with Architecture**: Test architectural modifications or optimizations.

## Features

### Supported Instructions
- **Arithmetic and Logic**: `add`, `addi`, `sub`, `mul`, `slt`, `sll`, `ori`
- **Memory Access**: `lw`, `sw`
- **Control Flow**: `beq`, `j`, `jal`, `jr`
- **System Calls**: `syscall` (basic I/O and program termination)

### Assembler Capabilities
- Two-pass assembly with error detection:
  - Invalid instruction mnemonics.
  - Incorrect number of operands.
  - Missing commas or parentheses.
  - Invalid register names.
- Support for basic directives:
  - `.data`, `.text`, `.word`, `.space`, `.asciiz`.

### Memory Model
- 1 MB linear memory.
- 32-bit signed integer operations.

### Additional Functionality
- System calls for I/O and program termination.
- Comprehensive error handling for assembly issues.

## Architecture and Design

The simulator is built around the following components:

- **Datapath**:
  - Includes the Program Counter (PC), instruction memory, ALU, data memory, and multiplexers.
  - Depicts the flow of data and instructions through the datapath.
- **Control Unit**:
  - Generates precise control signals for datapath operations, including `RegDst`, `ALUSrc`, `MemtoReg`, and others.
- **Memory**:
  - A linear memory model with 1 MB size, shared between instructions and data.
- **Register File**:
  - Implements 32 general-purpose 32-bit registers.

## Testing

The simulator was tested with various MIPS assembly programs. Here are some test cases:

1. **Arithmetic Operations**:
   - Perform `add`, `sub`, `or`, `and`, and `slt` operations.
   - Verify correct register values after execution.

2. **Branching and Jump Instructions**:
   - Test conditional branching (`beq`) and unconditional jumps (`j`, `jal`).

3. **Complex Programs**:
   - Calculate the Nth Catalan number.
   - Compute the factorial of a number.
   - Build a calculator supporting addition, subtraction, and multiplication.

**Result**: All test cases passed successfully, confirming the accuracy of the implementation.

## How to Run

To run the MIPS Simulator, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/username/mips-simulator.git
   cd mips-simulator
2. **Compile the Code**: Compile the source code using a C++ compiler (e.g., g++):
    ```bash
    g++ -o mips_simulator mainBody.cpp
3. **Run the Simulator**: Execute the compiled binary:
    ```bash
    ./mips_simulator
4. **Provide an Assembly File**: When prompted, provide the path to a valid MIPS assembly file:
    ```vbnet
    Enter the Asm file name : testCode1.txt
5. **View the Output**: The simulator will assemble the code, execute the instructions, and display the results, including register values and memory modifications.

## Limitations
- Does not support floating-point operations or coprocessor instructions.
- No pipelining or advanced memory management features.
- **lw** does not directly handle label references but can be used in conjunction with **la** instructions.

## License
This project is licensed under the MIT License. See the LICENSE file for details.