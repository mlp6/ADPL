#include "application.h"
#include "PublishDataCell.h"

bool PublishDataCell::publish(double HXCI, double HXCO, double HTR,
                              double HXHI, double HXHO, double Exhaust,
                              int gasOn, int bucket_tip_count){
    char data_str [69];

    fmt_string = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,E:%.1f,V:%d,B:%d";

    sprintf(data_str, fmt_string, HXCI, HXCO, HTR, HXHI, HXHO, Exhaust,
            gasOn, bucket_tip_count);

    return Particle.publish("DATA", data_str);
}
