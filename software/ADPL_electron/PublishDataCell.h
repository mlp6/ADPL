#ifndef PublishDataCell_h
#define PublishDataCell_h
#endif

#include "application.h"

class PublishDataCell {
public:
    bool publish();
    //TODO: implement formatting within publishing class
    char* fmt_string;
    char* fmt_string_no_bucket;
};