// Define GPIO pins (Particle Electron) by function
#define HXCI A0 // Heat Exchanger Cold Inlet
#define HXCO A1 // Heat Exchanger Cold Output
#define HTR A2  // Heater
#define HXHI A3 // Heat Exchanger Hot Inlet
#define HXHO A4 // Heat Exchanger Hot Outlet
#define VALVE D4
#define IGNITOR D2
#define PUMP D0
//#define LEVEL B5
//#define BUCKET C0 // Duke
#define BUCKET C4 // Kenya
#define STEP D3
#define DIR D5
#define SLEEP D6
#define UP B5
#define DOWN C1
#define RESET C2

//SD pins do not need definitions;
// they are initialized via the SdFatSoftSpi<> command in ADPL_electron.ino
