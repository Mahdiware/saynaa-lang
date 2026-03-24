## Class

Classes are the blueprint of objects, contains method definitions and behaviors for its instances.
The instance of a class method can be accessed with the `this` keyword.

```ruby
class Foo end
foo = Foo() ## Create a foo instance of Foo class.
```

To initialize an instance when it's constructed use `_init` method. Saynaa instance attributes
are dynamic (means you can add a new field to an instance on the fly).

```ruby
class Foo
  function _init(bar, baz)
    this.bar = bar
  end
end

foo = Foo('bar', 'baz')
```

To override an operator just use the operator symbol as the method name.

```ruby
class Vec2
  function _init(x, y)
    this.x = x; this.y = y
  end
  function _str
    return "<${this.x}, ${this.y}>"
  end
  function + (other)
    return Vec2(this.x + other.x,
                this.y + other.y)
  end
  function += (other)
    this.x += other.x
    this.y += other.y
    return this
  end
  function == (other)
    return this.x == other.x and this.y == other.y
  end
end
```

To distinguish unary operator with binary operator the `this` keyword should be used.

```ruby
class N
  function _init(n)
    this.n = n
  end

  function - (other) ## N(1) - N(2)
    return N(this.n - other.n)
  end

  function -this () ## -N(1)
    return N(-this.n)
  end
end
```

All classes are ultimately inherit an abstract class named `Object` to inherit from any other class
use a parent class in parentheses at the class definition. However you cannot inherit from the builtin class like
Number, Boolean, Null, String, List, ...

```ruby
class Shape # Implicitly inherit Object class
end

class Circle(Shape) # Inherits the Shape class
end
```

To override a method just redefine the method in a subclass.

```ruby
class Shape
  function area()
    assert(false)
  end
end

class Circle(Shape)
  function _init(r)
    this.r = r
  end
  function area()
    return math.pi * r ** 2
  end
end
```

To call the a method on the super class use `super` keyword. If the method name is same as the current
method `super()` will do otherwise method name should be specified `super.method_name()`.

```ruby
class Rectangle(Shape)
  function _init(w, h)
    this.w = w; this.h = h
  end
  function scale(fx, fy)
    this.w *= fx
    this.h *= fy
  end
end

class Square(Rectangle)
  function _init(s)
    super(s, s) ## Calls super._init(s, s)
  end
  
  function scale(x)
    super(x, x)
  end

  function scale2(x, y)
    super.scale(x, y)
  end
end
```
## Magic Methods

Saynaa supports several magic methods that allow you to customize the behavior of your objects.

### `_init(...)`
The constructor method. Called when a new instance is created.
### `_new(...)`
Called before `_init(...)` when a class is invoked. If it returns `null` or
`undefined`, the default allocation path is used and `_init(...)` is called.
If it returns a non-instance value, `_init(...)` is skipped and the returned
value becomes the result of the call.

```ruby
class Foo
  function _new(x)
    inst = Foo() # or allocate using other logic
    return inst
  end
end
```

### `_del()`
Called by `delete(obj)` if defined on the instance.

```ruby
class Foo
  function _del()
    print("cleanup")
  end
end
```

```ruby
class Foo
  function _init(x)
    this.x = x
  end
end
```

### `_str()`
Returns a string representation of the object. Called by the `str()` builtin function or when the object is interpolated into a string.

```ruby
class Foo
  function _str()
    return "Foo"
  end
end
```

### `_call(...)`
Allows an instance to be called as a function.

```ruby
class Multiplier
  function _init(factor)
    this.factor = factor
  end
  
  function _call(x)
    return x * this.factor
  end
end

double = Multiplier(2)
print(double(5)) # Output: 10
```

### `_getter(name)`
Called when an attribute lookup fails. This allows for dynamic property access.

```ruby
class Dynamic
  function _getter(name)
    if (name == "foo") return "bar"
    return null
  end
end

d = Dynamic()
print(d.foo) # Output: bar
```

### `_setter(name, value)`
Called when an attribute assignment occurs.
### `_getattribute(name)`
Called for **every** attribute access. It overrides normal lookup.

```ruby
class Dynamic
  function _getattribute(name)
    return "attr: $name"
  end
end
```

### `_getattr(name)`
Called only when an attribute lookup fails (after normal lookup).

```ruby
class Dynamic
  function _getattr(name)
    return "missing: $name"
  end
end
```

### `_setattr(name, value)`
Called on every attribute assignment. Use `this.setattr(name, value, true)` to bypass.

```ruby
class Dynamic
  function _setattr(name, value)
    this.setattr(name, value, true)
  end
end
```

### `_delattr(name)`
Called on attribute deletion. Use `this.delattr(name, true)` to perform the actual removal.

```ruby
class Dynamic
  function _delattr(name)
    this.delattr(name, true)
  end
end
```

```ruby
class Checked
  function _setter(name, value)
    if (name == "age" and value < 0)
      print("Error: Age cannot be negative")
      return
    end
    this.age = value
  end
end
```
