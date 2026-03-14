# os

The `os` module provides functions for interacting with the operating system.

## Functions

### getcwd

Returns the current working directory.

```ruby
path = os.getcwd()
```

**Returns:**
- (String): The current working directory.

### chdir

Changes the current working directory.

```ruby
os.chdir(path)
```

**Parameters:**
- `path` (String): The new working directory.

### mkdir

Creates a directory at the `path`.

```ruby
os.mkdir(path)
```

**Parameters:**
- `path` (String): The path relative to CWD or absolute.

### rmdir

Removes an empty directory at the `path`.

```ruby
os.rmdir(path)
```

**Parameters:**
- `path` (String): The directory to remove.

### unlink

Removes a file at the `path`.

```ruby
os.unlink(path)
```

**Parameters:**
- `path` (String): The file to remove.

### moditime

Returns the modified timestamp of the file.

```ruby
timestamp = os.moditime(path)
```

**Parameters:**
- `path` (String): The file path.

**Returns:**
- (Number): The modification time (Unix timestamp).

### filesize

Returns the file size in bytes.

```ruby
size = os.filesize(path)
```

**Parameters:**
- `path` (String): The file path.

**Returns:**
- (Number): The size in bytes.

### system

Executes the command in a subprocess. Returns the exit code of the child process.

```ruby
exit_code = os.system(cmd)
```

**Parameters:**
- `cmd` (String): The command to execute.

**Returns:**
- (Number): The exit code.

### getenv

Returns the environment variable as String if it exists, otherwise it returns `null`.

```ruby
value = os.getenv(name)
```

**Parameters:**
- `name` (String): The environment variable name.

**Returns:**
- (String|Null): The value or null.

### exepath

Returns the path of the saynaa interpreter executable.

```ruby
path = os.exepath()
```

**Returns:**
- (String): The executable path.

### exec (Linux Only)

Executes the command and returns the output (first line/limited buffer).

```ruby
output = os.exec(cmd)
```

### setenv (Linux Only)

Sets an environment variable.

```ruby
os.setenv(name, value)
```

**Parameters:**
- `name` (String): The variable name.
- `value` (String): The value to set.

## Constants

- `os.name`: (String) The operating system name (e.g., "linux", "windows").
- `os.platform`: (String) The platform name.
- `os.argc`: (Number) The number of command line arguments.
- `os.argv`: (List) The list of command line arguments strings.
