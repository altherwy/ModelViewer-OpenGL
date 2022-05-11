
#Linux:#
FLAGS = -I/usr/include -I/usr/include/eigen3 -L/usr/lib -L/usr/lib/nvidia-304-updates -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings

#Mac:#
#FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/X11/include -I/opt/local/include -I/opt/local/include/eigen3 -L/opt/X11/lib -L/opt/local/lib -lGLEW -lGLUT -Wno-write-strings

%: %.cpp camera.h mesh.*
	g++ $*.cpp mesh.cpp -o ../$* $(FLAGS)

clean:
	rm modelviewer modelviewerVA modelviewerVBO modelviewerVAO
