#ifndef PublishDataSD_h
#define PublishDataSD_h

#include "application.h"
#include "SD/SdFat.h"

class PublishDataSD {
public:
    bool publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO,
                 int gasOn, int bucket_tip_count, File sdFile);
    bool pushToCell(File sdFile);
    bool logError(int errorCode);
private:
    char* fmt_string_SD;
    char* fmt_string_no_bucket_SD;
    void printTime(File file);
};

#endif
