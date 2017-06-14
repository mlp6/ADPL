#ifndef PublishDataCell_h
#define PublishDataCell_h
#endif

#include "application.h"
#include "TempProbe.h"
#include "Valve.h"
#include "Bucket.h"

class PublishDataCell {
public:
    bool publish();
    //TODO: implement formatting within publishing class
    char* fmt_string;
    char* fmt_string_no_bucket;

    /* does not work
    extern TempProbe tempHXCI;
    extern TempProbe tempHXCO;
    extern TempProbe tempHTR;
    extern TempProbe tempHXHI;
    extern TempProbe tempHXHO;
    extern Valve valve;
    extern Bucket bucket;
     */
};