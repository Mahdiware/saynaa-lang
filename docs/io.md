# io

The `io` module provides functions and classes for input and output operations.

## Functions

### write

Write `bytes` to the `stream`. `stream` should be one of `io.stdin`, `io.stdout`, or `io.stderr`.

```ruby
io.write(stream, bytes)
```

**Parameters:**
- `stream` (Number): One of `io.stdin`, `io.stdout`, or `io.stderr`.
- `bytes` (String): The data to write.

**Example:**
```ruby
import io
io.write(io.stdout, "Hello World\n")
```

### flush

Flush the `stdout` buffer.

```ruby
io.flush()
```

### getc

Read a single character from `stdin`.

```ruby
char = io.getc()
```

**Returns:**
- (String): The character read from stdin.

### readfile

Reads a file at `path` and returns its content as a string.

```ruby
content = io.readfile(path)
```

**Parameters:**
- `path` (String): Path to the file.

**Returns:**
- (String): The content of the file.

### open

Opens a file at the `path` with the specified `mode`. See `io.File.open` for details.

```ruby
f = io.open("test.txt", "r")
```

## Classes

### File

A class for file manipulation.

#### methods

##### open

Opens a file at the `path` with the `mode`. `path` should be either absolute or relative to the current working directory.

```ruby
f.open(path, mode)
```

**Parameters:**
- `path` (String): The path to the file.
- `mode` (String): The mode to open the file in.

**Modes:**

| Mode | If file exists | If file does not exist |
|------|----------------|------------------------|
| 'r'  | Read from start| Error                  |
| 'w'  | Destroy content| Create new             |
| 'a'  | Write to end   | Create new             |
| 'r+' | Read from start| Error                  |
| 'w+' | Destroy content| Create new             |
| 'a+' | Write to end   | Create new             |

You can append 'b' for binary mode (e.g., 'rb', 'wb').

##### read

Reads `count` bytes from the file. If `count` is -1, reads until the end of the file.

```ruby
data = f.read(count)
```

**Parameters:**
- `count` (Number): Number of bytes to read, or -1 for all.

**Returns:**
- (String): The data read.

##### write

Writes `data` to the file.

```ruby
f.write(data)
```

**Parameters:**
- `data` (String): The data to write.

##### getline

Reads a line from the file. Only for text files.

```ruby
line = f.getline()
```

**Returns:**
- (String): The line read.

##### seek

Moves the file cursor.

```ruby
f.seek(offset, whence)
```

**Parameters:**
- `offset` (Number): The offset to move.
- `whence` (Number): The starting position (0: Beginning, 1: Current, 2: End).

##### tell

Returns the current file cursor position.

```ruby
pos = f.tell()
```

**Returns:**
- (Number): The current position.

##### close

Closes the file.

```ruby
f.close()
```

## Constants

- `io.stdin`: Standard Input (0)
- `io.stdout`: Standard Output (1)
- `io.stderr`: Standard Error (2)
