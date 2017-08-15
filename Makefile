OUTDIR=dist
CFLAGS= -O2

.PHONY: default clean

default: test clean

$(OUTDIR):
	@mkdir -p $(OUTDIR)

test: $(OUTDIR) $(OUTDIR)/crc.o $(OUTDIR)/main.o
	gcc $(CFLAGS) $(OUTDIR)/crc.o $(OUTDIR)/main.o -o test
	./test

$(OUTDIR)/crc.o: crc.c
	gcc $(CFLAGS) -o $(OUTDIR)/crc.o -c $^

$(OUTDIR)/main.o: main.c
	gcc $(CFLAGS) -o $(OUTDIR)/main.o -c $^ 

clean:
	@rm -f test
	@rm -rf $(OUTDIR)
