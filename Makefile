OUTDIR=.

CFLAGS= -O2
MODULES= $(OUTDIR)/crc.o

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/crc.o: crc.c
	gcc $(CFLAGS) -o $(OUTDIR)/crc.o $^
	chmod +x crc.o
