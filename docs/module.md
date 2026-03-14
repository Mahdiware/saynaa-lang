# module

The `module` module provides functions and variables related to module loading.

## Properties

### path

A list of strings specifying the search paths for modules. The default paths usually include current directory and installation directory.

```ruby
module.path.append("/my/custom/path")
```

### searchers

A list of functions used to search for modules. You can add custom searchers.

```ruby
module.searchers.append(my_searcher)
```

## Functions

### load

Loads a module by name. It returns the module object.

```ruby
mod = module.load("math")
```

**Parameters:**
- `name` (String): The module name.

**Returns:**
- (Module): The loaded module.
