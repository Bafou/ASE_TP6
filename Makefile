CC=gcc
CFLAGS=-m32 -g
CLIB=-I./include -L./lib -lhardware
EXEC=dmps frmt frmt_reverse print_vol create_vol remove_vol free_bloc new_bloc if_pfile if_nfile if_cfile if_dfile if_cpfile


all: $(EXEC)

print_vol : print_vol.o mbr.o volume.o Driver.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

create_vol : create_vol.o mbr.o volume.o Driver.o
		$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

remove_vol : remove_vol.o mbr.o volume.o Driver.o
				$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

new_bloc : new_bloc.o alloc.o mbr.o volume.o Driver.o
				$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

free_bloc : free_bloc.o alloc.o mbr.o volume.o Driver.o
				$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

test_vol : test_vol.o
				$(CC) -o $@ $^ $(CFLAGS)

test_alloc : test_alloc.o alloc.o volume.o mbr.o Driver.o
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

inode.o : inode.c inode.h mbr.o Driver.o alloc.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

alloc.o : alloc.c alloc.h mbr.o volume.o Driver.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

ifile.o : ifile.c ifile.h mbr.o volume.o inode.o Driver.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

if_pfile: if_pfile.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

if_nfile: if_nfile.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

if_dfile: if_dfile.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

if_cpfile: if_cpfile.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

if_cfile: if_cfile.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

if_status: if_status.c mount.o ifile.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLIB)

mount.o: mount.c mount.h volume.o mbr.o
	$(CC) -o $@ -c $< $(CFLAGS) $(CLIB)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: realclean all
	./create_vol 1 1 1 BASE
	./print_vol
	./create_vol 1 1 1 BASE
	./create_vol 2 3 7 BASE
	./print_vol
	./create_vol 1 3 7 BASE
	./print_vol
	./create_vol 2 2 1 BASE
	./print_vol
	./create_vol 2 1 2 BASE
	./print_vol
	./create_vol 2 6 2 BASE
	./print_vol
	./create_vol 2 9 2 BASE
	./print_vol
	./create_vol 2 10 2 BASE
	./print_vol
	./remove_vol 3
	./print_vol

test2: realclean all
	./create_vol 2 3 4 ANNEXE
	./print_vol
	./new_bloc 0
	./new_bloc 0
	./new_bloc 0
	./new_bloc 0
	./free_bloc 0 2
	./free_bloc 0 2
	./free_bloc 0 5
	./new_bloc 0
	./free_bloc 0 2

.PHONY: clean realclean test

clean:
	rm -rf *~ *.o

realclean:
	make clean
	rm -rf $(EXEC) vdisk*.bin
