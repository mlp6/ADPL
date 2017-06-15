#include "TempProbe.h"
#include "Valve.h"
#include "Bucket.h"
#include "application.h"
#define VOLUME 250.0 //300 mL, varies by location
#define OPTIMAL_FLOW 5.0 //5.0 L/hr, varies by location

extern TempProbe tempHXCI("HXCI", HXCI);
extern TempProbe tempHXCO("HXCO", HXCO);
extern TempProbe tempHTR("HTR", HTR);
extern TempProbe tempHXHI("HXHI", HXHI);
extern TempProbe tempHXHO("HXHO", HXHO);
extern Valve valve(VALVE);
extern Bucket bucket(BUCKET, VOLUME, OPTIMAL_FLOW);
