# S80A (Simple 8080 Assembler)

**S80A** is a lightweight, prototype-stage assembler for the Intel 8080 architecture, written in pure C. It was born out of a mix of curiosity, late-night coding sessions (2 AM vibes), and a passion for low-level systems.

The goal of this project is to provide a simple tool to convert 8080 Assembly mnemonics into functional machine code binaries.

## Features

* **Instruction Support**: Handles core mnemonics including `MOV`, `MVI`, `ADD`, `SUB`, and `HLT`.
* **Control Flow**: Supports branching and subroutines with `JMP`, `JZ`, `JNZ`, and `CALL`.
* **Label Resolution**: Automatically calculates memory addresses for labels (e.g., `_start:`).
* **CLI Flags**: Built-in command-line interface for versioning, help, and custom output naming.
* **Lightweight**: Zero dependencies, just standard C libraries.

## Installation & Compilation

To compile the assembler from source, ensure you have a C compiler (like `gcc` or `clang`) installed:

```bash
gcc main.c -o s80a
```

## Usage

Run the assembler by passing your `.asm` file as the first argument:

```bash
./s80a input.asm -o program.bin
```

### Command Line Options
* `-v`: Display the current version.
* `-h`: Display help and available flags.
* `-o <filename>`: Specify the output binary name (defaults to `a.bin`).

## Example Assembly Code

The following script is compatible with S80A:

```asm
_start:
    MVI A, 65
    MVI B, 1
    ADD B
    JMP _start
    HLT
```

## Current Status: Unstable

This project is currently in its **Prototype 2** stage (`Unstable 1.0.1`). 
* The parser is sensitive to whitespace and formatting.
* Error handling is basic.
* Many instructions from the full 8080 set are still being implemented.

## 📝 Roadmap
- [ ] Add bitwise logical operators (`ANA`, `ORA`, `XRA`).
- [ ] Implement increment/decrement (`INR`, `DCR`).
- [ ] Add I/O port support (`IN`, `OUT`).
- [ ] Improve the pre-processor/parser robustness.

## 🤝 Contributing

Feel free to fork this project, report bugs, or submit pull requests. If you find a bug that was clearly written at 2 AM while I was sleep-deprived... you're probably right.

## 📄 License

This project is open-source. Feel free to use, modify, and share.
