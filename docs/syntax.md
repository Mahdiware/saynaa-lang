## Syntax

**Saynaa** syntax is designed to be familiar to people coming from C-like languages like Lua, Python, Ruby and many more.
>In Saynaa semicolon separator **;** is optional.


What Saynaa code looks like:
```ruby
  for i in 0..10 do
    print("Hello World")
  end
```

### Comments
Saynaa supports both line comments:
```ruby
  #  This is a line comment
  // This is a line comment
```
and block comments:
```ruby
  /*
    This
    is
    a
    multi-line
    comment
  */
```
While Saynaa uses C-Style comments, Saynaa still supports the common "#!" shebang to tell your shell what program to execute the file with. However, the shebang must be on the first line of the file in order to use it in this way:
```ruby
  #!/path/to/saynaa

  function hello(year)
    print("Happy New Year $year")
  end
  hello(2023);
```

## Code blocks

All the blocks are closed with the `end` keyword,
all blocks are stards with either a new line and an optional "block entering keyword". For a single line
block these keywords are must. `if` blocks starts with the `then`, for `while` and `for` loops they starts
with the `do` keyword.

```ruby
  # The `do` keyword is a must here.
  while cond do something() end

  # The `do` keyword is a optional here.
  for i in 0..10
    print('$i')
  end

  # `then` is optional if new line is present.
  if cond1 then
    foo()
  else if cond2
    bar()
  else
    baz()
  end
```

### Reserved Keywords
Like many other programming languages Saynaa has some reserved keywords that assume a very specific meaning in the context of the source code:
```ruby
  if elif else class import fn function do end
  from as null in is and or not true false self
  super break while for continue return then
```

### Identifiers
Identifiers represent a naming rule used to identify objects inside your source code. Saynaa is a case-sensitive language. Identifiers start with a letter or underscore and may contain letters, digits, and underscores:
```ruby
  a
  _thisIsValid
  Hello_World
  foo123
  BYE_BYE
```
