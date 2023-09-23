## Closure

Closures are self-contained blocks of functionality that can be passed around and used in your code. Closures can capture and store references to any constants and variables from the context in which they are defined. Closures can be nested and can be anonymous (without a name):

```ruby
  function f1(a)
    return function(b)
      return a + b
    end
  end

  if _name == "@main"
    addTen = f1(10)
    return addTen(20)  // result is 30
  end
```

### Disassemble
A closure can be disassembled in order to reveal its bytecode:
```ruby
  import lang
  function sum (a, b)
    return a + b;
  end

  if _name == "@main"
    print(lang.disas(sum));
  end

  // Output:
  // Instruction Dump of function sum @main
  //   2:     0  PUSH_LOCAL_0          (param:0)
  //          1  PUSH_LOCAL_1          (param:1)
  //          2  ADD
  //          4  RETURN
  //   3:     5  POP
  //          6  POP
  //          7  RETURN
  //          8  END
```
