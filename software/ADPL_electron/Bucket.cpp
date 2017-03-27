/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);

    int iter = 0;
    int bucket_array[6] = {0, 0, 0, 0, 0, 0};
    double flow_rate = 0.0;
    bool was_successful = true;
}

void Bucket::tipped() {
    tip_count++;
}

void Bucket::updateFlow(bool was_successful, int publish_delay){
    iter++;

    // if the last publish was a success, simply populate the new tip_count
    // otherwise take the difference between that now and last time
    if (was_successful){
      bucket_array[iter%6] = tip_count;
    }
    else {
      bucket_array[iter%6] = tip_count - bucket_array[(iter-1)%6];
    }

    // take sum of total tips in the given period to calculate tips/min
    double sum = 0;
    for (int i=0; i < 6; i++){
      sum += bucket_array[i];
    }

    int bucket_time = ((6*publish_delay)/60000);
    flow_rate = ((double)sum)/bucket_time;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
