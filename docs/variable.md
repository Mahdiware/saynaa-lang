# Variables

Variables are dynamically typed. A name is resolved in this order:

1. Local scope (function and block scope)
2. Upvalues (captured from outer functions)
3. Module globals
4. Builtin functions and types

If a name cannot be resolved, a runtime error is raised:

```
Name 'x' is not defined.
```

## Undefined Globals and _missing

You can define a module-level `_missing` function to provide a fallback value
when a global name is not defined. If `_missing` returns `null` or `undefined`,
the runtime raises the usual "Name '@' is not defined." error. The fallback
value is **not** stored as a new global.

```ruby
called = false

_module._missing = function(name)
  if name == "answer" then
    return 42
  end
  return null
end

print(answer)        # 42
print(answer)        # 42 (still fallback, not stored)
print(unknown_name)  # error: Name 'unknown_name' is not defined.
```

## Global Assignment Inside Functions

If a global already exists, assigning to it inside a function updates the
existing global value instead of creating a new local.

```ruby
x = 2

function test()
  x = 3
end

test()
print(x) # 3
```

## Tips

- To create a new local inside a function, assign a name that does not exist
  in globals.
- Use `define(name, value)` to explicitly create a global at runtime.
