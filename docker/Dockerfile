FROM debian/eol:etch

RUN printf 'deb http://archive.debian.org/debian etch main\n' > /etc/apt/sources.list \
    && apt-get -o Acquire::Check-Valid-Until=false update \
    && apt-get -y --force-yes install gcc-3.4 g++-3.4 make

WORKDIR /work
