# base on debian
FROM debian:latest

RUN apt-get update

# Docker Compose
RUN apt-get -y install docker-compose docker.io

# Cron
RUN apt-get -y install cron curl

# Python for rest api
RUN apt install -y \
	python3 python3-pip

RUN pip3 install \
    flask pyyaml
