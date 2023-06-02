#!/bin/bash
# {"username":"xyz","password":"xyz"}
curl --header "Content-Type: application/json" \
  --request POST \
  --data "$(cat $1 | tr -d '\n' | tr -d ' ')" \
  http://localhost:8000/config