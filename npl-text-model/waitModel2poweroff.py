#!/usr/bin/python3

import os

while os.popen("docker top 487c4b9f87e699cecbaf4b9facb8d5099cfafd5c45e2b34ace10a8313cd4ca78 | grep model | awk '{print $2}'").read() != "":
	continue
os.popen("notify-send poweroff; sleep 5m; poweroff")
