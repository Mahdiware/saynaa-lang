# path

The `path` module provides functions for path manipulation.

## Functions

### abspath

Returns the absolute version of a path.

```ruby
abs_path = path.abspath("file.txt")
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (String): The absolute path.

### basename

Returns the base name of pathname property.

```ruby
name = path.basename("/foo/bar/baz.txt")
## name -> "baz.txt"
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (String): The base name.

### dirname

Returns the directory name of pathname property.

```ruby
dir = path.dirname("/foo/bar/baz.txt")
## dir -> "/foo/bar"
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (String): The directory name.

### exists

Returns true if the path refers to an existing path or an open file descriptor.

```ruby
if path.exists("file.txt") {
  print("File exists")
}
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (Boolean): True if explicitly exists.

### getcwd

Returns the current working directory.

```ruby
cwd = path.getcwd()
```

**Returns:**
- (String): The current working directory.

### getext

Returns the file extension.

```ruby
ext = path.getext("file.txt")
## ext -> ".txt"
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (String): The file extension.

### isabs

Returns true if the path is an absolute path.

```ruby
is_abs = path.isabs("/foo/bar")
## is_abs -> true
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (Boolean): True if absolute.

### isdir

Returns true if the path is an existing directory.

```ruby
is_dir = path.isdir("docs")
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (Boolean): True if directory.

### isfile

Returns true if the path is an existing regular file.

```ruby
is_file = path.isfile("file.txt")
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (Boolean): True if file.

### join

Joins one or more path components intelligently.

```ruby
full_path = path.join("/foo", "bar", "baz.txt")
## full_path -> "/foo/bar/baz.txt"
```

**Parameters:**
- `*paths` (String): The path components.

**Returns:**
- (String): The joined path.

### listdir

Returns a list containing the names of the entries in the directory given by path.

```ruby
files = path.listdir(".")
```

**Parameters:**
- `path` (String): The directory path.
- `recursive` (Boolean): Optional. If true, list recursively. Default is `false`.

**Returns:**
- (List): A list of file names/paths.

### normpath

Normalize a pathname.

```ruby
norm = path.normpath("/foo/../bar")
## norm -> "/bar"
```

**Parameters:**
- `path` (String): The path.

**Returns:**
- (String): The normalized path.

### relpath

Return a relative filepath to path either from the current directory or from an optional start directory.

```ruby
rel = path.relpath("/foo/bar/baz", "/foo")
## rel -> "bar/baz"
```

**Parameters:**
- `path` (String): The target path.
- `start` (String): The starting path.

**Returns:**
- (String): The relative path.
