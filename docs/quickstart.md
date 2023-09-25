## Getting started

### Install
To install, simply execute the commands given below.

```bash
  git clone https://github.com/saynaa-lang/saynaa.git
  cd saynaa
```

### Makefile
```bash
  make
```

### GCC / MinGw / Clang (alias with gcc)
```
gcc -o saynaa src/cli/*.c src/compiler/*.c src/optionals/*.c src/runtime/*.c src/shared/*.c src/utils/*.c -lm -ldl
```

### MSVC
```
cl /Fesaynaa src/cli/*.c src/compiler/*.c src/optionals/*.c src/runtime/*.c src/shared/*.c src/utils/*.c && rm *.obj
```

### Windows batch script
```
build.bat
```

### Command line
To view all possible flags you can run the command below:
```bash
  ./saynaa --help
```

### REPL mode
```bash
  ./saynaa
```

### To directly execute a file
```bash
  ./saynaa myfile.sa
```

### Hello World
A simple <strong>Hello World</strong> code in Saynaa looks like:
```ruby
  print("Hello World!")
```
