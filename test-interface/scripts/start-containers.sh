#!/bin/bash
DOCKERCOMPOSE=$1
docker-compose -f $DOCKERCOMPOSE build
docker-compose -f $DOCKERCOMPOSE up $ELSE
