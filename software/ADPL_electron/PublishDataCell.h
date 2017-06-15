#ifndef PublishDataCell_h
#define PublishDataCell_h

#include "application.h"

class PublishDataCell {
public:
    bool publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO, int gasOn, int bucket_tip_count);
    //TODO: implement formatting within publishing class
private:
    char* fmt_string;
    char* fmt_string_no_bucket;
};

#endif