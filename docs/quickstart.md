# Getting Started

Welcome to Saynaa! This guide will help you install the language and run your first code.

## Installation

Saynaa is written in C and is designed to be easy to compile.

### 1. Clone the Repository
```bash
git clone https://github.com/mahdiware/saynaa-lang.git
cd saynaa-lang
```

### 2. Build
You can build Saynaa using Make or GCC directly.

**Using Makefile:**
```bash
make
```

**Using GCC/Clang directly:**
```bash
gcc -o saynaa src/cli/*.c src/compiler/*.c src/optionals/*.c src/runtime/*.c src/shared/*.c src/utils/*.c -lm -ldl
```

**On Windows (MSVC):**
```bash
cl /Fesaynaa.exe src/cli/*.c src/compiler/*.c src/optionals/*.c src/runtime/*.c src/shared/*.c src/utils/*.c
del *.obj
```

## Running Code

### Interactive REPL
To explore the language interactively, simply run the executable without arguments:

```bash
./saynaa
```

You'll see the Saynaa prompt `>`. Type code and press Enter.
```ruby
> print("Hello from REPL!")
Hello from REPL!
```

### Running a Script
Save your code in a file with the `.sa` extension (e.g., `hello.sa`).

```ruby
# hello.sa
print("Hello, World!")
```

Run it using the interpreter:
```bash
./saynaa hello.sa
```

## Next Steps

*   Check out the [Language Guide](syntax.md) to learn the syntax.
*   Learn about [Standard Library](io.md) modules.
*   See how to [Create Modules](creating_modules.md).
```
