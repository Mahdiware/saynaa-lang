## List

Lists (or arrays) are simple sequence of objects, their size is dynamic and their index starts always at 0. They provide fast random access to their elements. You can create a list by placing a sequence of comma-separated expressions inside square brackets:

```ruby
  r = [1, 2, "Hello", 3.1415, true];

  // list has a length attribute
  n = r.length;  // n is 5
```

### Accessing items
You can access an element from a list by calling the subscript operator [] on it with the index of the element you want. Like most languages, indices start at 0:
      
```ruby
  fruits = ["Banana", "Apple", "Lime", "Orange", "Grape", "Mango"];
  fruits[0];  // "Banana"
  fruits[2];  // "Lime"
```

Negative indices count backwards from the end:
```ruby
  names = ["Banana", "Apple", "Lime", "Orange", "Grape", "Mango"];
  names[-1];  // "Mango"
  names[-2];  // "Grape"
```

### Iterating items
The subscript operator works well for finding values when you know the key you’re looking for, but sometimes you want to see everything that’s in the list. Since the List class implements the iterator method, you can easily use it in a for loop:
```ruby
  names = ["Banana", "Apple", "Lime", "Orange", "Grape", "Mango"];
  for name in names do
    print("Current name is " + name);
  end
```

### List as a stack
The List class implements the pop methods as a convenient way to treat a list as a stack:
```ruby
  list = [10,20,30,40,50];
  v1 = list.pop();  // pop 50
  v2 = list.pop();  // pop 40
```

### List Join
The List class implements the join method as a convenient way to interpret a list as a string:
```ruby
  list = [1,2,3,4,5];
  list.join(" + "); // Becomes: "1 + 2 + 3 + 4 + 5"
```

### List Sort
The List class implements the sort method as a convenient way to sort its items. By default, the sort() method sorts the values as strings (or numbers) in alphabetical (for strings) and ascending order:
      
```ruby
  fruits = ["Banana", "Orange", "Apple", "Mango"];
  fruits.sort();
  // fruits is now [Apple,Banana,Mango,Orange]

  numbers = [10, 3.14, 82, 1, 7];
  numbers.sort();
  // numbers is now [1,3.14,7,10,82]

```
