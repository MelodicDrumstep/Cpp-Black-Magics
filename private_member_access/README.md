# Accessing Class Private Members

## Background knowledge

+ .* operator in C++

We can use .* operator for accessing the member for a class object. The grammar is like

```cpp
class MyClass {
public:
    int32_t data;
    void func() {}
};

// declaration for the member pointers.
int32_t MyClass::* ptr_data = &MyClass::data;
void (MyClass::* ptr_func)() = &MyClass::func;

// Usage
MyClass obj;
obj.*ptr_data = 42;
(obj.*ptr_func)();
```

## How it works?

Now we want to access the private member of a class, which is prohibited by the compiler.

Let's think about the intuition: use raw address! If we can take out the address of this private data member, the without doubt we can access it regardless of the access qualifications.

Then how can we take out the address of the private data member of a class object? We should think about the support for C++ type system.

The answer is using pointer to class member! If we can successfully assign "the pointer to class member" (e.g. assign it as "&Target::x_"), then we can access it like "t.*the_ptr". And we will do it using static struct tricks or friend function injection.

We should notice that

1. An explicit instantiation of a library class may be an implementation detail of a client class.

2. Explicit instantiations may only be declared at namespace scope.

And thorough comments have been added to all of the source code. Please check them out for more details. It's pure magic!

## References

1. [allowing-access-to-private-members](https://stackoverflow.com/questions/15110526/allowing-access-to-private-members)

2. [why-is-it-possible-to-place-friend-function-definitions-inside-of-a-class-defini](https://stackoverflow.com/questions/17512557/why-is-it-possible-to-place-friend-function-definitions-inside-of-a-class-defini)

3. [declare-template-friend-function-of-template-class](https://stackoverflow.com/questions/18792565/declare-template-friend-function-of-template-class)

4. [cppreference-friend](https://en.cppreference.com/w/cpp/language/friend)