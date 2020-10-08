#!/usr/bin/env bash

## Path to you installed GDAL
GDAL_PATH=/usr/local/Cellar/gdal/2.4.4_4

INCLUDE_PATH=${GDAL_PATH}/include
LIBS_PATH=${GDAL_PATH}/lib
GDAL_LIB=-lgdal

# g++ -I${INCLUDE_PATH} -L${LIBS_PATH} ${GDAL_LIB} main.cpp -o main
# g++ -I${INCLUDE_PATH} -L${LIBS_PATH} ${GDAL_LIB} -std=c++11 scrolledcanvas.cc -o scrolledcanvas   `wx-config --cxxflags --libs` `wx-config  --libs --gl_libs -lglut -lGL -lGLU -lgdal`
g++ -I${INCLUDE_PATH} -L${LIBS_PATH} ${GDAL_LIB} -std=c++11 scrolledcanvas_first.cc -o scrolledcanvas   `wx-config --cxxflags --libs --gl_libs -lglut -lGL -lGLU -lgdal` -framework GLUT -framework OpenGL -framework Cocoa  -ljsoncpp
# g++ -I${INCLUDE_PATH} -L${LIBS_PATH} ${GDAL_LIB} -std=c++11 terrain.cc -o terrain
# g++ -I${INCLUDE_PATH} -L${LIBS_PATH} ${GDAL_LIB} -std=c++11 main.cc -o main   `wx-config --cxxflags --libs --gl_libs -lglut -lGL -lGLU -lgdal` -framework GLUT -framework OpenGL -framework Cocoa
