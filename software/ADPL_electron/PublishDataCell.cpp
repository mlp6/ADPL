#include "application.h"
#include "PublishDataCell.h"

bool PublishDataCell::publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO, int gasOn,
                              int bucket_tip_count){
    char data_str [69];

    // bucket.tip_count will be ignored if not needed by sprintf
    fmt_string = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    fmt_string_no_bucket = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    sprintf(data_str, (bucket_tip_count > 0) ? fmt_string : fmt_string_no_bucket,
            HXCI, HXCO, HTR, HXHI, HXHO, gasOn, bucket_tip_count);

    return Particle.publish("DATA", data_str);
}