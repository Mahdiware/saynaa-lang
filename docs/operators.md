## Operators

An operator is a special symbol or phrase that you use to check, change, or combine values. For example, the addition operator (+) adds two numbers, as in **i = 1 + 2**, and the logical AND operator (`and`) combines two Boolean values, as in **if (flag1 and flag2)**.
<br><br>
Saynaa supports most standard C operators and improves several capabilities to eliminate common coding errors. The assignment operator (=) does not return a value, to prevent it from being mistakenly used when the equal to operator (==) is intended. Saynaa also provides two [range](types.md) operators as a shortcut for expressing a range of values.
      
### Arithmetic Operators
* Addition (+)
* Subtraction (-)
* Multiplication (*)
* Division (/)
* Remainder (%)

```ruby
  n1 = 1 + 2        # equals 3
  n2 = 5 - 3        # equals 2
  n3 = 2 * 3        # equals 6
  n4 = 10.0 / 2.5   # equals 4
  n5 = 9 % 4        # equals 1
```

### Assignment Operator
The assignment operator = initialize or update a value:
```ruby
  a = 50;       # a = 50
  b = a;        # b = 50
  c = a * b;    # c = 50 * 50
```
Please note that contrary to many other programming languages, the assignment operator has no side effect, it means that it does not return any value.

### Comparison Operators
The comparison operators return a Bool value to indicate whether or not the statement is true:

* Equal (==)
* Not equal (!=)
* Less than (<)
* Less than or equal (<=)
* Greater than (>)
* Greater than or equal (>=)
* Type check (is)

```ruby
  1 == 1      # true because 1 is equal to 1
  1 != 2      # true because 1 is not equal to 2
  1 < 2       # true because 1 is less than 2
  1 <= 1      # true because 1 is less than or equal to 1
  1 > 2       # false because 1 is not greater than 2
  1 >= 1      # true because 1 is greater than or equal to 1
  1 is Number # true because 1 is of class Number
```

### Logical Operators
The comparison operators return a Bool value to indicate whether or not the statement is true:

* Logical NOT (!)
* Logical AND (and)
* Logical OR (or)

```ruby
  !1          # false because 1 is true
  1 and 0     # false because one of the two values is false
  1 or 0      # true because one of the two values is true
```
In order to improve code readability the reserved keywords **not, and, or** has been introduces as an alisas to logical operators.

### Bitwise Operators

* Bitwise shift left (<<)
* Bitwise shift right (>>)
* Bitwise AND (&)
* Bitwise OR (|)
* Bitwise XOR (^)
* Bitwise NOT or one's complement (~)

```ruby
  n = 0B00110011;
  n1 = n << 2             # equals 204
  n2 = n >> 2             # equals 12
  n3 = n & 0B00001111     # equals 3
  n4 = n | 0B00001111     # equals 63
  n5 = n ^ 0B00001111     # equals 60
  n6 = ~n;                # equals -52
```

### Compound Assignment Operators
As a shortcut, assignment and operators can be combined together:

* Multiply and assign (*=)
* Divide and assign (/=)
* Remainder and assign (%=)
* Add and assign (+=)
* Subtract and assign (-=)
* Left bit shift and assign (<<=)
* Right bit shift and assign (>>=)
* Bitwise AND and assign (&=)
* Bitwise XOR and assign (^=)
* Bitwise OR and assign (|=)
