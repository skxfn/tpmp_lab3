CC=gcc
CFLAGS=-Wall -Wextra -std=c11
LDFLAGS=-lsqlite3

TARGETS= \
	example1_gr12_SkaffennnnNikita \
	example2_gr12_SkaffennnnNikita \
	example3_gr12_SkaffennnnNikita \
	example4_gr12_SkaffennnnNikita \
	example5_gr12_SkaffennnnNikita

all: $(TARGETS)

example1_gr12_SkaffennnnNikita: example1_gr12_SkaffennnnNikita.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example2_gr12_SkaffennnnNikita: example2_gr12_SkaffennnnNikita.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example3_gr12_SkaffennnnNikita: example3_gr12_SkaffennnnNikita.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example4_gr12_SkaffennnnNikita: example4_gr12_SkaffennnnNikita.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example5_gr12_SkaffennnnNikita: example5_gr12_SkaffennnnNikita.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: all
	./example1_gr12_SkaffennnnNikita
	./example2_gr12_SkaffennnnNikita
	./example3_gr12_SkaffennnnNikita
	./example4_gr12_SkaffennnnNikita
	./example5_gr12_SkaffennnnNikita

clean:
	rm -f $(TARGETS) task4_examples.db exported_photo.bin
