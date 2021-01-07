NAME=asm_coldfire
R2_PLUGIN_PATH=$(shell r2 -H R2_USER_PLUGINS)
LIBEXT=$(shell r2 -H LIBEXT)
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_anal)
LDFLAGS=-shared $(shell pkg-config --libs r_anal)
OBJS=$(NAME).o coldfire_decoder.o
LIB=$(NAME).$(LIBEXT)

all: $(LIB)

clean:
	    rm -f $(LIB) $(OBJS)

$(LIB): $(OBJS)
	    $(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIB)

install:
	    cp -f asm_coldfire.$(LIBEXT) $(R2_PLUGIN_PATH)

uninstall:
	    rm -f $(R2_PLUGIN_PATH)/asm_coldfire.$(LIBEXT)
