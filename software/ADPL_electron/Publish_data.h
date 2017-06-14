#ifndef Publish_data_h
#define Publish_data_h

#include "application.h"
#include <fstream>

class Publish_data {
public:
    bool publish(char* data_str, FILE sdFile);
    bool sdcard_publish_success;
    bool cell_publish_success;
    const char* fmt_string;
    const char* fmt_string_no_bucket;
    const char* fmt_string_SD;
    const char* fmt_string_no_bucket_SD;
};