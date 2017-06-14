#ifndef PublishDataSD_h
#define PublishDataSD_h
#endif

#include "application.h"
#include "SD/SD.h"

class PublishDataSD {
public:
    bool publish(File sdFile);
    //TODO: implement formatting within publishing class
    char* fmt_string_SD;
    char* fmt_string_no_bucket_SD;
};