FROM debian:latest

ARG username="devu"
ARG userhome="/home/devu"

ENV HOME ${userhome}

RUN apt update
RUN apt upgrade -y
RUN apt update

# Docker & compose to manipulate and create other containers
RUN apt-get -y docker-compose docker.io

RUN apt install -y \
	python3 python3-pip cmake \
	openssh-server \
	build-essential \
	libpthread-stubs0-dev \
	autoconf automake pkg-config

#zmq
RUN apt install -y libzmq3-dev

# Computer Graphics - OpenGL
RUN apt install -y \
	libpng-dev libglu1-mesa-dev libx11-dev \
	libglfw3-dev \
	libglew-dev \
	libgles2-mesa-dev \
	freeglut3-dev

#Misc
RUN apt install -y \
	git nano \
	sudo \
	tmux \
	fish \
	firefox-esr \
	libreoffice \
	inkscape

#Simulators, Tools
RUN apt install -y \
	ktechlab \
	sonic-visualiser 

RUN pip install \
	numpy pandas openpyxl \
	matplotlib \
	scipy\
	gns3-server\
	gns3-gui \
	fastapi uvicorn python-multipart

# Universal Ctags
RUN	git clone https://github.com/universal-ctags/ctags.git; \
	cd ctags; \
	./autogen.sh; \
	./configure; \
	make; \
	make install

# Get emscripten source
RUN git clone https://github.com/emscripten-core/emscripten.git

# Get the emsdk repo
RUN git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory
WORKDIR /emsdk

# Fetch the latest version of the emsdk (not needed the first time you clone)
#RUN git pull

# Download and install the latest SDK tools.
RUN ./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
RUN ./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
# RUN bash ./emsdk_env.sh

# User
RUN groupadd -r ${username}
RUN useradd -rm -d ${userhome} -s /usr/bin/fish -g ${username} -u 1000 ${username}
RUN adduser ${username} sudo
RUN chown -R ${username}:${username} ${userhome}
RUN echo ${username}:${username} | chpasswd

WORKDIR ${userhome}/repo

RUN service ssh start
CMD ["/usr/sbin/sshd","-D"]
EXPOSE 22

USER ${username}
ENV fish_greeting="Alle Anfang ist schwer, anfangen ist Einfach, Beharrlichkeit eine Kunst."
RUN echo "bash $HOME/repo/scripts/set.sh" >> ${userhome}/.bashrc

CMD ["/bin/bash", "-c", "source", "/emsdk/emsdk_env.sh"]
ENTRYPOINT ["/usr/bin/fish", "-c"]
