#include "MyDamageType.h"

UMyDamageType::UMyDamageType()
{
	DamageImpulse = 5000.f;

	bScaleMomentumByMass = true;

	bCausedByWorld = true;
}
