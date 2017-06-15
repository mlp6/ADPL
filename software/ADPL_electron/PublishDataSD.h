#ifndef PublishDataSD_h
#define PublishDataSD_h

#include "application.h"
#include "SD/SD.h"

class PublishDataSD {
public:
    bool publish(File sdFile);
    //TODO: implement formatting within publishing class
private:
    char* fmt_string;
    char* fmt_string_no_bucket;
};

#endif
