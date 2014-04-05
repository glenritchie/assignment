ODIR = ../objects
objects = $(ODIR)/logic.o $(ODIR)/main.o $(ODIR)/keyboard.o $(ODIR)/mouse.o $(ODIR)/help.o \
$(ODIR)/init.o $(ODIR)/display.o $(ODIR)/frames.o $(ODIR)/global.o $(ODIR)/lightmap.o $(ODIR)/menu.o \
$(ODIR)/create.o $(ODIR)/object.o $(ODIR)/model.o \
$(ODIR)/box.o $(ODIR)/cylinder.o $(ODIR)/bezier.o $(ODIR)/sphere.o \
$(ODIR)/ground.o $(ODIR)/file.o $(ODIR)/lightobject.o $(ODIR)/waypointobject.o
include flags.def
include common.def
echo: 
	echo Using args: $(CFLAGS)

FORCE:
link: all 
	cd ../../gamelib/sources; $(MAKE) link
	echo Linking...
#	g++ -rdynamic -ggdb -g3 $(objects)  -lGLU -lGL -o ../bin/assignment -lglut -lXmu -lXt -lX11 -lpthread -lXi -lXext -lm -L/usr/X11R6/lib ../../gamelib/gamelib.o
	g++ -rdynamic -ggdb -g3 $(objects)  -lGLU -lGL -o ../bin/assignment -lglut -lXmu -lXt -lX11 -lXi -lXext -lm -L/usr/X11R6/lib ../../gamelib/gamelib.o

cleanboth: clean
	cd ../../gamelib/sources; $(MAKE) clean
	
clean:
	rm -f $(ODIR)/*.o
	@echo Cleaned cp2060 sources...
	
