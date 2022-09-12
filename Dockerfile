FROM debian:latest

# SHELL ["/bin/bash", "-c"] 

RUN apt update
RUN apt upgrade -y
RUN apt update

RUN apt install -y \
	python3 python3-pip cmake \
	build-essential libpng-dev libglu1-mesa-dev libx11-dev libpthread-stubs0-dev \
	autoconf automake pkg-config

RUN apt install -y \
	libglfw3-dev \
	libglew-dev \
	libgles2-mesa-dev \
	freeglut3-dev

RUN apt install -y \
	git nano \
	openssh-server \
	tmux \
	firefox-esr \
	xonsh \
	libreoffice \
	sonic-visualiser

RUN pip install numpy pandas openpyxl

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

WORKDIR /src

RUN service ssh start
EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]

# SHELL ["xonsh"]

#ENTRYPOINT ["/bin/bash", "-c"]
# ENTRYPOINT ["/bin/bash", "/emsdk/emsdk_env.sh", "&", "xonsh"]
CMD ["/bin/bash", "-c", "source", "/emsdk/emsdk_env.sh"]
# ENTRYPOINT ["/bin/bash", "-c"]
ENTRYPOINT ["/usr/bin/xonsh", "-c"]
#CMD ["/bin/bash"]
