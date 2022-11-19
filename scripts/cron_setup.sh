#!/bin/bash
# install cron & curl
RUN apt -y install cron curl

# Give execution rights on the cron jobs
chmod 0644 /etc/cron.d/*

# Apply cron job
crontab /etc/cron.d/*
 
# Create the log file to be able to run tail
touch /var/log/cron.log
 
# Run the command on container startup
cron &
tail -f /var/log/cron.log &