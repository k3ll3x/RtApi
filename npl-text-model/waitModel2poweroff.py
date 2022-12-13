#!/usr/bin/python3

import os

cmd = "docker top 487c4b9f87e699cecbaf4b9facb8d5099cfafd5c45e2b34ace10a8313cd4ca78 | grep textgen | awk '{print $2}'"

while os.popen(cmd).read() != "":
	continue
os.popen("notify-send poweroff; sleep 5m; poweroff")
