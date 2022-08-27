FROM shapelets/intel-opencl:latest
RUN apt update && apt install -y cmake build-essential software-properties-common lsb-release
RUN apt install -y libeigen3-dev libboost-all-dev
RUN apt install -y libglu1-mesa-dev mesa-common-dev
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt update && apt install -y python3.8 python3.8-dev python3.8-venv
RUN apt install -y python3-pip libjpeg-dev
RUN python3.8 -m pip install numpy==1.21 matplotlib
ENV TZ=US/Pacific
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt install -y python3-tk
#RUN apt-get clean all

WORKDIR /home/jhuang/repo/dbscan
