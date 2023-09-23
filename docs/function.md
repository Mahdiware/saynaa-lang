## Function
function is a block of organized code that is used to perform a single task. They provide better modularity for your application and reuse-ability. Depending on the programming language, a function may be called a subroutine, a procedure, a routine, a method, or a subprogram

```ruby
  function message()
    print("Hello ", "World!");
  end
  
  message();
```

### Function parameters
Functions aren’t very useful if you can’t pass values to them so you can provide a parameter list in the function declaration.

```ruby
  function sum(a, b)
    return a + b;
  end

  # execute the sum function
  # and returns 30 as result
  sum(10,20);
```

You can also specify a default value in the parameter

```ruby
  function sum(a = 10, b = 20)
    return a+b
  end

  # result: 10+20 = 30
  sum()
  # result: 40+20 = 60
  sum(40)
```

If a function is called with missing arguments (less than declared), the missing values are set to **null**.
```ruby
  # sum modified to take in account missing arguments
  function sum(a, b)
    # equivalent to if (a == null) then a = 30 end
    if (!a) then a = 30 end

    # equivalent to if (b == null) then b = 50 end
    if (!b) then b = 50 end

    return a + b;
  end

  # execute the sum function without any argument
  # a has a 30 default value and b has a 50 default value
  # return value is 80
  sum();
```

### Recursion
Function recursion is fully supported in Saynaa:
```ruby
  function fibonacci(n)
    if (n<2) then return n end
    return fibonacci(n-2) + fibonacci(n-1);
  end

  return fibonacci(20);
```

### Returning values
A function without a return statement returns **null** by default. You can explicitly return a value using a return statement.
