#include "application.h"
#include "PublishDataCell.h"

bool PublishDataCell::publish(){
    // bucket.tip_count will be ignored if not needed by sprintf
    //TODO: implement currentTime (and format it)
    fmt_string = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    fmt_string_no_bucket = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    char* data_str = sprintf(data_str, (bucket.tip_count > 0) ? fmt_string : fmt_string_no_bucket,
                       tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp, tempHXHO.temp,
                       int(valve.gasOn), int(bucket.tip_count));

    return Particle.publish("DATA",data_str);
}