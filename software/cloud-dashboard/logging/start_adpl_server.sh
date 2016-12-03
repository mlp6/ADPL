#!/bin/bash

function print_log_seperator {
    seq 60 | xargs -Iz echo -n "=" >> log.log  
    echo "" >> log.log # New Line
} 

function print_log_header {
    print_log_seperator
    echo $1 >> log.log
    print_log_seperator
}

export PORT=80 
print_log_header "Server startup on $(date)"
authbind node /home/bitnami/ADPL/software/logging/server.js | tee -a log.log

