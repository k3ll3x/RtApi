#!/bin/bash

contname="devenv"

usage(){
	printf "$0 run\n$0 build"
}

if [[ $# -eq 0 ]]
then
	usage
	exit
fi

if [[ "$1" == "run" ]]
then
	xhost +
	docker run -it \
		-v $(pwd):/src \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v $HOME/.ssh:/root/.ssh \
		-e DISPLAY=:0 \
		--device /dev/dri/card0 \
		--device /dev/snd \
		--net=host \
		$contname \
		"bash ./scripts/set.sh"
else
	docker build -t $contname .
fi
