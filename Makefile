CC=gcc
CFLAGS=-m32 -g
CLIB=-I./include -L./lib -lhardware
EXEC=dmps frmt frmt_reverse print_vol create_vol remove_vol


all: $(EXEC)

print_vol : print_vol.o mbr.o volume.o Driver.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

create_vol : create_vol.o mbr.o volume.o Driver.o
		$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

remove_vol : remove_vol.o mbr.o volume.o Driver.o
				$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

dmps: dmps.o Driver.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

frmt: frmt.o Driver.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

frmt_reverse: frmt_reverse.o Driver.o
		$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

Driver.o: Driver.c Driver.h
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

mbr.o : mbr.c mbr.h Driver.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

volume.o : volume.c volume.h mbr.o Driver.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean realclean

clean:
	rm -rf *~ *.o

realclean:
	make clean
	rm -rf $(EXEC) vdisk*.bin
