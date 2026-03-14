# lang

The `lang` module provides functions to interact with the runtime.

## Functions

### gc

Trigger garbage collection and return the amount of bytes cleaned.

```ruby
freed = lang.gc()
```

**Returns:**
- (Number): Bytes cleaned.

### disas

Returns the disassembled opcode of the function.

```ruby
code = lang.disas(fn)
```

**Parameters:**
- `fn` (Closure): The function/closure to disassemble.

**Returns:**
- (String): The bytecode disassembly.

### backtrace

Return the current backtrace as a list of strings.

```ruby
bt = lang.backtrace()
```

**Returns:**
- (List): The backtrace.

### modules

Returns a list of all loaded module objects.

```ruby
mods = lang.modules()
```

**Returns:**
- (List): List of Module objects.
