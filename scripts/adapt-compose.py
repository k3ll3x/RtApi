#!/usr/bin/python3

# adapts an existing docker compose file with tools

import  yaml

volumes = [
    "${cronpath}:/etc/cron.d",
- ${logspath}:/var/log
- ${scriptspath}:/usr/local/scripts