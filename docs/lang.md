## lang Module
lang is a builtin Module.

```ruby
import lang
```

### gc
Trigger garbage collection and return the amount of bytes cleaned.

```ruby
lang.gc() -> Number
```

### disas
Returns the disassembled opcode of the function [fn].

```ruby
lang.disas(fn:Closure) -> String
```

### backtrace
Returns the backtrace as a string,
each line is formated as '<function>;<file>;<line>'.

```ruby
lang.backtrace() -> String
```

### modules
Returns the list of all registered modules.

```ruby
lang.modules() -> List
```

### debug_break
A debug function for development (will be removed).

```ruby
lang.debug_break() -> Null
```
