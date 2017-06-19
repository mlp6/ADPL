#include "application.h"
#include "PublishDataSD.h"
#include "SD/SdFat/FatFile.h"
#include "SD/SdFat/SdFat.h"

bool PublishDataSD::publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO,
                            int gasOn, int bucket_tip_count, FatFile sdFile){

    char data_str [69];
    // bucket.tip_count will be ignored if not needed by sprintf
    //TODO: implement currentTime (and format it)
    fmt_string_SD = "time:%d,HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    fmt_string_no_bucket_SD = "time:%d,HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    sprintf(data_str, (bucket_tip_count > 0) ? fmt_string_SD : fmt_string_no_bucket_SD,
            millis(), HXCI, HXCO, HTR, HXHI, HXHO, gasOn, bucket_tip_count);

    int success = sdFile.write(data_str);
    //Indicates publishing success
    if(success > -1){
        return true;
    } else {
        return false;
    }

}