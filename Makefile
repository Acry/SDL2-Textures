CFLAGS   = -Wall -Wextra -mtune=native `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2_image -lm

.SUFFIXES:
.SUFFIXES: .c .o

srcdir	 =src/
TARGETS	 = texture_generation 1 1b 1c 1d 1e 1f 1g 1h 2 3

.PHONY: all
all: $(TARGETS)

#test
texture_generation: $(srcdir)helper.c $(srcdir)main.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# render noise
1: $(srcdir)helper.c $(srcdir)1.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# render noise - press s to save as out.jpg
1b: $(srcdir)helper.c $(srcdir)1b.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# render smooth noise - press s to save as out.jpg
1c: $(srcdir)helper.c $(srcdir)1c.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# add turbulence
1d: $(srcdir)helper.c $(srcdir)1d.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# change turbulence with mousewheel (0-255)
1e: $(srcdir)helper.c $(srcdir)1e.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)
	
# Clouds - HSLtoRGB, change turbulence with mousewheel (0-255)
1f: $(srcdir)helper.c $(srcdir)1f.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# Marble
1g: $(srcdir)helper.c $(srcdir)1g.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# Wood
1h: $(srcdir)helper.c $(srcdir)1h.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# Some experiments - some more mathematical 2D functions
2: $(srcdir)helper.c $(srcdir)2.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# 3D Random Noise
3: $(srcdir)helper.c $(srcdir)3.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true

