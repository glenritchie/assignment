ODIR = ../objects
objects = $(ODIR)/cresource.o $(ODIR)/resource_manager.o $(ODIR)/ctexture.o $(ODIR)/utils.o \
$(ODIR)/textutils.o $(ODIR)/font.o $(ODIR)/anim_texture.o $(ODIR)/vertexarray.o $(ODIR)/gl_varray.o\
$(ODIR)/3dmath.o $(ODIR)/vector.o $(ODIR)/log.o $(ODIR)/config.o $(ODIR)/light.o $(ODIR)/lightmanager.o \
$(ODIR)/matrix.o $(ODIR)/base_entity.o $(ODIR)/frustum.o $(ODIR)/camera.o\
$(ODIR)/draw_box.o $(ODIR)/displaylist.o

include common.def
echo: 
	echo Using args: $(CFLAGS)

FORCE:

link: all
	rm -f ../gamelib.o
	ar r ../gamelib.o ../objects/*.o
	
clean:
	rm -f ../objects/*.o
	rm -f ../gamelib.o
	@echo Cleaned...
		

