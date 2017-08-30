#include "application.h"
#include "PublishDataSD.h"
#include "SD/SdFat.h"

bool PublishDataSD::publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO,
                            int gasOn, int bucket_tip_count, File sdFile){
    char data_str [79];
    int bitsWritten = -1;
    // bucket.tip_count will be ignored if not needed by sprintf
    fmt_string_SD = "[%d]HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    fmt_string_no_bucket_SD = "[%d]HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    sprintf(data_str, (bucket_tip_count > 0) ? fmt_string_SD : fmt_string_no_bucket_SD,
            Time.now(), HXCI, HXCO, HTR, HXHI, HXHO, gasOn, bucket_tip_count);

    // open the file for write at end like the "Native SD library"
    sdFile.open("adpl_data.txt", O_RDWR | O_CREAT | O_AT_END);

    bitsWritten = sdFile.println(data_str);

    sdFile.close();

    //Indicates publishing success
    if(bitsWritten > -1){
        return true;
    } else {
        return false;
    }

}

bool PublishDataSD::pushToCell(File sdFile) {
    // open the file for write at end like the "Native SD library"
    if (!sdFile.open("adpl_data.txt", O_RDWR | O_CREAT)) {
        return false;
    }
    // initialize a bool to determine success
    bool success = true;
    // read each line in the file and subsequently publish it
    char data_str[79];
    size_t n;
    while ((n = sdFile.fgets(data_str, sizeof(data_str))) > 0) {
        //determine if reading the end of a line
        if (data_str[n - 1] != '\n') {
            return false;
        }
        if(!Particle.publish("SD DATA", data_str)) {
            success = false;
        }
    }
    // close the file:
    sdFile.close();
    // return the success bool
    return success;
}

bool PublishDataSD::logError(int errorCode) {
    File errorFile;
    int bitsWritten = -1;

    if(!errorFile.open("error_log.txt", O_WRITE | O_CREAT)){
        // if the file doesn't open
        return false;
    }
    char error_str [69];
    sprintf(error_str, "[%d]%d", Time.now(), errorCode);
    bitsWritten = errorFile.println(error_str);

    errorFile.close();
    //Indicates publishing success
    if(bitsWritten > -1){
        return true;
    } else {
        return false;
    }
}
