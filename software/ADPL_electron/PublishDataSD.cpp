#include "application.h"
#include "PublishDataSD.h"
#include "SD/SdFat.h"

bool PublishDataSD::publish(double HXCI, double HXCO, double HTR, double HXHI, double HXHO,
                            int gasOn, int bucket_tip_count, File sdFile){
    char data_str [69];
    int bitsWritten = -1;
    // bucket.tip_count will be ignored if not needed by sprintf
    fmt_string_SD = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    fmt_string_no_bucket_SD = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    sprintf(data_str, (bucket_tip_count > 0) ? fmt_string_SD : fmt_string_no_bucket_SD,
            HXCI, HXCO, HTR, HXHI, HXHO, gasOn, bucket_tip_count);

    // open the file for write at end like the "Native SD library"
    if (!sdFile.open("adpl_data.txt", O_RDWR | O_CREAT | O_AT_END)) {
        Log.error("opening file for write failed.");
    }
    // if the file opened okay, write to it:
    // print time
    sdFile.print("[");
    sdFile.print(Time.hour());
    sdFile.print(":");
    sdFile.print(Time.minute());
    sdFile.print(":");
    sdFile.print(Time.second());
    sdFile.print("]");
    // print data
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
    if (!sdFile.open("adpl_data.txt", O_RDWR)) {
        Log.error("opening file for write failed.");
        return false;
    }
    // initialize a bool to determine success
    bool success = true;
    // read each line in the file and subsequently publish it
    char data_str[69];
    size_t n;
    while ((n = sdFile.fgets(data_str, sizeof(data_str))) > 0) {
        //determine if reading the end of a line
        if (data_str[n - 1] != '\n') {
            Log.error("Line in data file did not end with newline character.");
            return false;
        }
        if(!Particle.publish("SD DATA", data_str)){
            // if publish failed
            success = false;
        }
    }
    // close the file:
    sdFile.close();
    // return the success bool
    return success;
}
