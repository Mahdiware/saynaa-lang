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
use `is` keyword at the class definition. However you cannot inherit from the builtin class like
Number, Boolean, Null, String, List, ...

```ruby
class Shape # Implicitly inherit Object class
end

class Circle is Shape # Inherits the Shape class
end
```

To override a method just redefine the method in a subclass.

```ruby
class Shape
  function area()
    assert(false)
  end
end

class Circle is Shape
  function _init(r)
    this.r = r
  end
  function area()
    return math.PI * r ** 2
  end
end
```

To call the a method on the super class use `super` keyword. If the method name is same as the current
method `super()` will do otherwise method name should be specified `super.method_name()`.

```ruby
class Rectangle is Shape
  function _init(w, h)
    this.w = w; this.h = h
  end
  function scale(fx, fy)
    this.w *= fx
    this.h *= fy
  end
end

class Square is Rectangle
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