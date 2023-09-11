FROM debian:latest

ARG username="devu"
ARG userhome="/home/${username}"

ENV HOME ${userhome}

RUN apt update
RUN apt upgrade -y
RUN apt update

# Docker & compose to manipulate and create other containers
RUN apt-get -y install \
	ca-certificates \
	curl \
	lsb-release \
	wget

RUN mkdir -p /etc/apt/keyrings

RUN apt update

#Misc
RUN apt install -y \
	git nano \
	sudo \
	tmux \
	fish \
	firefox-esr 

# webscrapper
RUN pip install selenium\
	guesslang
#---------------------------------------------------

# User
RUN groupadd -r ${username}
RUN useradd -rm -d ${userhome} -s /usr/bin/fish -g ${username} -u 1000 ${username}
#RUN adduser ${username} sudo
RUN chown -R ${username}:${username} ${userhome}
RUN echo ${username}:${username} | chpasswd

WORKDIR ${userhome}/repo

RUN service ssh start
CMD ["/usr/sbin/sshd","-D"]
EXPOSE 22

#user
USER ${username}

ENV fish_greeting="Big brother is watching you"
ENTRYPOINT ["/usr/bin/fish", "-c"]
