#include "application.h"
#include "Publish_data.h"
#include <fstream>

bool Publish_data::publish(char* data_str, FILE sdFile){
    sdcard_publish_success = false;
    cell_publish_success = false;

    if (sdFile) {
        sdFile.println(data_str);
        sdcard_publish_success = true;
    }

    cell_publish_success = Particle.publish("DATA",data_str);

    return (sdcard_publish_success || cell_publish_success);
}