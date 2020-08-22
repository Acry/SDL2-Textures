# libsdl 2 flags
CFLAGS   = -Wall -Wextra -mtune=native -no-pie `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2_image -lm

# libsdl flags
CFLAGS2   = -Wall -Wextra -mtune=native `sdl-config --cflags`
LDFLAGS2  = `sdl-config --libs` -lSDL_image -lm
.SUFFIXES:
.SUFFIXES: .c .o

# no need to build 4a1 - it was for testing purposes like texture_generation

srcdir	 =src/
TARGETS	 = texture_generation 1 1b 1c 1d 1e 1f 1g 1h 2 3 4a 4a1 4b

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

# 2d LUT deformations and tunnels 1 - Repeat Texture / tiling
4a: $(srcdir)helper_soft.c $(srcdir)4a.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

# SDL 1 arithmetic pattern
4a1: $(srcdir)4a1.c
	$(CC) $(CFLAGS2) -o $@ $+ $(LDFLAGS2)

# 2d LUT deformations and tunnels 1 - deform texture
4b: $(srcdir)helper_soft.c $(srcdir)4b.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true

