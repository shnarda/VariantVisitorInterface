
# VariantVisitorInterface

VariantVisitorInterface, provides a syntax sugar for std::variant based runtime polymorphic objects by defining classes that provides visitor functions for the available classes (Just like abstract classes).\
The resulting interface, functions similar to virtual-function based interfaces and still holds all of the advantages variant based polymorphism provides.
 


## Installation

This project is header only, so only requirement is C++17. Example files are available for build.

    
## Usage/Examples

Let's say, we have several animal classes and we want to implement "fly" method only on the animals that can fly, "swim" only on the animals that can swim and "run" only on the animals that can run.\
We can implement these classes as below:
```c++
class Animal {
public:
	explicit Animal(const std::string_view name) : m_name(name) {}
	std::string_view name() const { return m_name; };
private:
	std::string m_name;
};

class Goose : public Animal {
public:
	Goose(const std::string_view name) : Animal(name) {};
	void fly(const bool successful) const;
	void swim(const bool successful) const;
	void run(const bool successful) const;

	std::string identify() const { return std::string(name()) + " the duck"; }
};

class EuropeanSwallow : public Animal {
public:
	EuropeanSwallow(const std::string_view name) : Animal(name) {};
	void fly(const bool successful) const;
	void run(const bool successful) const;

	std::string identify() const { return std::string(name()) + " the unladen swallow"; }
};

class FlyingFish : public Animal {
public:
	FlyingFish(const std::string_view name) : Animal(name) {};
	void fly(const bool successful) const;
	void swim(const bool successful) const;

	std::string identify() const { return std::string(name()) + " the flying fish"; }
};
```
Currently, these object don't have a direct connection between them thorughout any interface. But this changes quickly once we define the interface to determine the available methods.

```cpp
VARVIS_DEFINE_VARIANT_VISITOR(Swimmer, (Goose, FlyingFish), swim, identify)
VARVIS_DEFINE_VARIANT_VISITOR(Flying, (Goose, EuropeanSwallow, FlyingFish), fly, identify)
VARVIS_DEFINE_VARIANT_VISITOR(Runner, (Goose, EuropeanSwallow), run, identify)
```
With this macro, we defined three interface classes. These classes define that Goose and FlyingFish can "swim", Goose and EuropeanSwallow can "fly" and Goose and EuropeanSwallow can "run". With this knowledge, we can achieve runtime polymorphism

```cpp
//Interface classes also owns the variants and can be initialized just like their variant alternatives
//Here are some of the constructor alternatives (for ambiguous cases)
Swimmer swimmer = Goose{ "John" };
Flying flying{std::in_place_index<1>, "Elise"};
Runner runner{ std::in_place_type <Goose>, "Adam" };

//Now, we can access the interface functions just like the virtual functions
swimmer.swim(false); //Calls the swim method of the Goose object (with argument "false")
flying.fly(true);	 //Calls the fly method of the EuropeanSwallow object (with argument "true")
runner.run(true);	 //Calls the run method of the Goose object (with argument "true")
```
Sometimes, we want to check if an animal on the "Swimmer" object can also "run". Similar to "dynamic_cast", we have "interface_cast" that attempts to transfer one interface to another. 

```cpp
//We can transfer the variant between interfaces if the object inside the variant supports both of the interfaces
std::optional<Flying> castedFlying = VariantVisitorInterface::interface_cast<Flying>(swimmer);

//Check if the cast was successful
if (castedFlying)
	castedFlying->fly(true); //Calls the fly method of the Goose object (with argument "true")
```

It should be noted that just like std::variant, these interfaces also follow the value semantics. As result, unintentional copies can be made while the type casting. Moving the object is always an option.\
\
There is one other alternative way to access interface functions, which is reference visitors that holds the reference of the variant instead of having the ownership of the object.

```cpp
//Define the reference visitor interfaces
VARVIS_DEFINE_VARIANT_VISITOR_REF(SwimmerRef, swim, identify)
VARVIS_DEFINE_VARIANT_VISITOR_REF(FlyingRef, fly, identify)


std::variant<Goose, FlyingFish> v_swimmer = FlyingFish{ "George" };
SwimmerRef swimmerRef{ v_swimmer };
swimmerRef.swim(true); //Calls the swim method of the FlyingFish object (with argument "true")
FlyingRef flyingrRef{ v_swimmer };
flyingrRef.fly(true); //Calls the fly method of the FlyingFish object (with argument "true")
```


