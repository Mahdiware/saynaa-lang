[![build](https://github.com/mahdiware/saynaa-lang/actions/workflows/build.yml/badge.svg)](https://github.com/mahdiware/saynaa-lang/actions/workflows/build.yml)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://paypal.me/mahdiware)
[![Telegram](https://img.shields.io/badge/-telegram-red?color=white&logo=telegram&logoColor=black)](https://telegram.me/mahdiware)
[![License](https://img.shields.io/packagist/l/longman/telegram-bot.svg)](https://github.com/mahdiware/saynaa-lang/blob/main/LICENSE)


<p align="center"> <img src="docs/assets/icon.png" alt="Saynaa-Language" title="Saynaa Programming Language" align="center"> </p>

# What is Saynaa?
**Saynaa** is an interpreted, object-oriented, lightweight, embeddable scripting language written in C.
It has a simple syntax and ships as a standalone executable with zero external dependencies.

## What Saynaa code looks like

```ruby
class Vector
  function _init(x, y)
    this.x = x; this.y = y
  end
  function _str
    return "<${this.x}, ${this.y}>"
  end
  function +(other)
    return Vector(this.x + other.x,
                  this.y + other.y)
  end
end

if _name == "@main"
  # initialize a new vector object
  v1 = Vector(1, 2);

  # initialize a new vector object
  v2 = Vector(3, 4);

  # add two vector
  v3 = v1 + v2

  # print v3, result is "<4, 6>"
  print(str(v3))
end
```

## Features of Saynaa
- Simple Syntax
- No setup. Single binary and you're good to go.
- REPL
- Object oriented
- Dynamic typing
- Concurrency
- Garbage collection
- Operator overloading
- First class functions
- Lexical scoping
- Embeddable
- Direct interop with C
- Highly optimized loops
- Tail call optimization
- **optional semicolons**

## Quickstart
```bash
make
./saynaa --help
./saynaa
./saynaa examples/helloworld.sa
```

## Documentation
- Language docs: [docs/README.md](docs/README.md)
- Syntax reference: [docs/syntax.md](docs/syntax.md)
- Standard library: [docs/lang.md](docs/lang.md)

## Optional modules
Saynaa ships with optional modules that can be enabled at build time. These include:
- `io`, `json`, `math`, `os`, `path`, `re`, `term`, `time`

## Testing
```bash
python3 util/test.py
```

## Benchmarking
```bash
make benchmark
# or
python3 util/run.py --app ./saynaa

# compare to a baseline
python3 util/run.py --app ./saynaa --baseline tests/benchmark/results/latest.json

# run a specific benchmark file case
python3 util/run.py --app ./saynaa --case runtime_function_call.sa

# fail if regression is above 15%
python3 util/run.py --app ./saynaa --baseline tests/benchmark/results/latest.json --fail-on-regression-pct 15

# CI-oriented quick profile
make benchmark-ci

# compare local build vs installed binary and generate HTML report
python3 util/compare.py --app1 ./saynaa --app2 /usr/local/bin/saynaa

# compare three or more builds
python3 util/compare.py --app1 ./saynaa --app2 /usr/local/bin/saynaa --app3 ./saynaa-other

# custom labels in HTML report
python3 util/compare.py --app1 ./saynaa --name1 local --app2 /usr/local/bin/saynaa --name2 system

# Makefile shortcut (override inputs)
make benchmark-compare BENCH_APP1=./saynaa BENCH_APP2=/usr/local/bin/saynaa
```

Benchmark cases are auto-discovered from metadata headers in `tests/benchmark/phases/*.sa`.

## Special thanks
Saynaa was supported by a couple of open-source projects.
- Bob Nystrom. *craftinginterpreters* www.craftinginterpreters.com
  *Pratt Parsers: Expression Parsing Made Easy* http://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy
  and [Wren](https://github.com/wren-lang/wren) programming language
- Roberto Ierusalimschy, Luiz Henrique de Figueiredo, Waldemar Celes *Closures in Lua* https://www.cs.tufts.edu/~nr/cs257/archive/roberto-ierusalimschy/closures-draft.pdf
- [Gravity](https://github.com/marcobambini/gravity) programming language for documentation

## Getting started

### Install
To install, simply execute the commands given below.

```bash
  git clone https://github.com/mahdiware/saynaa-lang.git
  cd saynaa-lang
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

## Contributing
Contributions to Saynaa are welcomed and encouraged!<br>
More information is available in the official [CONTRIBUTING](CONTRIBUTING.md) file.
* <a href="https://github.com/mahdiware/saynaa-lang/issues/new">Open an issue</a>:
  * if you need help
  * if you find a bug
  * if you have a feature request
  * to ask a general question
* <a href="https://github.com/mahdiware/saynaa-lang/pulls">Submit a pull request</a>:
  * if you want to contribute

## Donate
 [![](docs/assets/btn_donate.gif)](https://www.paypal.me/mahdiware)

## License
Saynaa is available under the permissive MIT license.
