FROM gcc:latest

RUN apt-get update && apt-get install -y valgrind libcmocka0 libcmocka-dev
COPY ./src ./src
COPY ./Makefile ./Makefile
RUN make test -B
RUN make -B 

ENTRYPOINT [ "bin/helios-*" ]