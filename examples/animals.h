#pragma once
#include <string>
#include <iostream>



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




void Goose::swim(const bool successful) const
{
	if (successful)
		std::cout << identify() << " enjoys swimming peacefully in nearby lake.\n";
	else
		std::cout << identify() << " doesn't want to swim! It's too peaceful for him.\n";
}

void Goose::run(const bool successful) const
{
	if (successful)
		std::cout << identify() << " runs towards you. Peace was never an option!\n";
	else
		std::cout << identify() << " prefers not to run this time. You are spared.\n";
}

void Goose::fly(const bool successful) const
{
	constexpr int DUCK_VELOCITY_MPH = 45;
	if (successful)
		std::cout << identify() << " sure can fly with an air speed velocity of around " << std::to_string(DUCK_VELOCITY_MPH) << "mph.\n";
	else
		std::cout << "Oh no, " << identify() << " is failed to fly! He ate too much recently.\n";
}

void EuropeanSwallow::fly(const bool successful) const
{
	constexpr int SWALLOW_VELOCITY_MPH = 20;
	if (successful)
		std::cout << identify() << " flies with an air speed velocity of around " << std::to_string(SWALLOW_VELOCITY_MPH) << "mph.\n";
	else
		std::cout  << identify() << " is on overhead line and it's too comfortable to fly away.\n";
}

void EuropeanSwallow::run(const bool successful) const
{
	if (successful)
		std::cout << identify() << " reaches his destination through running since it requires less calories.\n";
	else
		std::cout << identify() << " doesn't bother to run since flying is faster." << std::endl;
}

void FlyingFish::swim(const bool successful) const
{
	if (successful)
		std::cout << identify() << " swims quickly to find a potential food.\n";
	else
		std::cout << identify() << " prefers sleeping over swimming.\n";
}

void FlyingFish::fly(const bool successful) const
{
	if (successful)
		std::cout << identify() << " majestically flies while running away from the predator.\n";
	else
		std::cout << "Oh no, " << identify() << " is caught by the predator just below the surface!\n";
}