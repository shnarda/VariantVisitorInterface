#include "VariantVisitorInterface.h"
#include "animals.h"


VARVIS_DEFINE_VARIANT_VISITOR(Swimmer, (Goose, FlyingFish), swim, identify)
VARVIS_DEFINE_VARIANT_VISITOR(Flying, (Goose, EuropeanSwallow, FlyingFish), fly, identify)
VARVIS_DEFINE_VARIANT_VISITOR(Runner, (Goose, EuropeanSwallow), run, identify)
VARVIS_DEFINE_VARIANT_VISITOR_REF(SwimmerRef, swim, identify)
VARVIS_DEFINE_VARIANT_VISITOR_REF(FlyingRef, fly, identify)

int main()
{
	//Interface classes also owns the variants and can be initialized just like their variant alternatives
	Swimmer swimmer = Goose{ "John" };
	Flying flyer{std::in_place_index<1>, "Elise"};
	Runner runner{ std::in_place_type <Goose>, "Adam" };

	//Now, we can access the interface functions just like the virtual functions
	swimmer.swim(false); //Calls the swim method of the Goose object (with argument "false")
	flyer.fly(true);	 //Calls the fly method of the EuropeanSwallow object (with argument "true")
	runner.run(true);	 //Calls the run method of the Goose object (with argument "true")

	//We can transfer the variant between interfaces if the object inside the variant supports both of the interfaces
	std::optional<Flying> castedFlyer = VariantVisitorInterface::interface_cast<Flying>(swimmer);

	//Check if the cast was successful
	if (castedFlyer)
		castedFlyer->fly(true); //Calls the fly method of the Goose object (with argument "true")

	//Reference interface doesn't have the ownership the variant object. It purely exist to access the visitor functions
	std::variant<Goose, FlyingFish> v_swimmer = FlyingFish{ "George" };
	SwimmerRef swimmerRef{ v_swimmer };
	swimmerRef.swim(true); //Calls the swim method of the FlyingFish object (with argument "true")
	FlyingRef flyingrRef{ v_swimmer };
	flyingrRef.fly(true); //Calls the fly method of the FlyingFish object (with argument "true")

}
