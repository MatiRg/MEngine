# Code Style Guide 
In short use same style as in rest of the files.More in details below.
## Style
Use following rules:
* Code uses C++17 standart, do not use compiler specific extensions
* Braces in new line
* One indentation level is 4 spaces
* Name functions, methods, namespaces beginning with upper case e.g. `void Foo()`, `namespace Bar`
* Name classes beginning with C and upper case word first letters e.g `class CFooBar`
* Abstract classes and interfaces should begin with I and upper case word first letters e.g `class IFooBar`
* Name structs beginning with S and upper case word first letters e.g `struct SFooBar`
* Name unions beginning with U and upper case word first letters e.g `union UFooBar`
* Use only enum class for enumeration types
* Name enum beginning with E and upper case word first letters e.g `enum class EBlendMode`
* Enum values beginning with upper case e.g. `Alpha`
* Name template classes, structs beginning with T and upper case word first letters e.g `class|struct TFooBar`
* Use lambda, auto, STL Algorithms and foreach if applicable
* Global variables should be minimized(preffer static functions in classes), but if used only upper case letters, words separated and const or constexpr with _ e.g. `static constexpr char PHYSICS_TEXT[] = "PHYSICS";`
* Name accessors with Set and get prefix: `void SetX()` and `float GetX()`
* Use new C++-style casts, e.g. `static_cast<Type>(value)`
* Do not use `using namespace Name;`, provide full namespace qualifier wherever possible
* Divide class, struct into sections: public Types, public Methods, pubnlic fields, protected ..., private e.g.
```C++
class CFoo
{
public:
    void Bar();
public:
    int x;
private:
    struct STest
    {
        ...
    };
private:
    flaot GetSth() const;
private:
    std::string Name;
};
```
* Use const methods if applicable
* Use virtual destructor if class can be inherited from
* Make copy and/or move constructors/assigment deleted if applicable with utility class - `class IUpdatable: public NonCopyableMovable`
* Mark classes, methods final if they should not be further overriden e.g. `class CEngine final`
* Mark methods override if they override some method e.g. `void OnInit() override` or `virtual void OnInit() override`
* Pass primitve types to functions/methods as value with optional const e.g. `Foo(const float)`
* Pass or return complex types to or from functions/methods as refernce with optional const e.g. `Foo(const Vector&)` or `const Vector& () const`
* Initialize class fields in same order as declared and always set them default value
* Use STL functions, containers and classes when needed
* Try not to use exceptions
* Loops varibles should be named like: `int i, j, k, l, m, n`
* Try to compare same numerical types e.g. int with int and not int with std::size_t
* Name variables with upper case e.g. `int Value;`
* You can name Constructor and Method parameters with a prefix to skip shadowning variables e.g. `int aValue`
* Name macros with upper case letters and words separated with _ e.g. `#define LOG_IF(Severity, Condition)`
* In cpp file first include should be `.hpp` file associated with source file e.g. `App.cpp` first include should be `App.hpp`
* Source files should have `.cpp` extension and header files should have `.hpp` extension
* Use `#pragma once` for include guard