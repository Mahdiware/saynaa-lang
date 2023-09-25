## os Module
os provides functions for interacting with the operating system

```ruby
import os
```

### getcwd
Returns the current working directory

```ruby
os.getcwd() -> String
```

### chdir
Change the current working directory

```ruby
os.chdir(path:String)
```

### mkdir
Creates a directory at the path. The path should be valid.

```ruby
os.mkdir(path:String)
```

### rmdir
Removes an empty directory at the path.

```ruby
os.rmdir(path:String)
```

### unlink
Removes a file at the path.

```ruby
os.unlink(path:String)
```

### moditime
Returns the modified timestamp of the file.

```ruby
os.moditime(path:String) -> Number
```

### filesize
Returns the file size in bytes.

```ruby
os.filesize(path:String) -> Number
```

### system
Execute the command in a subprocess, Returns the exit code of the child process.

```ruby
os.system(cmd:String) -> Number
```

### getenv
Returns the environment variable as String if it exists otherwise it'll return null.

```ruby
os.getenv(name:String) -> String
```

### exepath
Returns the path of the saynaa interpreter executable.

```ruby
os.exepath() -> String
```
