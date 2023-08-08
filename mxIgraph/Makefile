CFLAGS += -fPIC -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += -Iinclude -I../igraph-core/include -I../$(BUILD)/igraph/include
LDFLAGS += -L$(LIB)
MEXFLAGS = -R2018a
LIB := ../$(LIB)

vpath %.so $(LIB)
vpath %.h include src
vpath %.c src

sources := $(wildcard src/mx*.c)
objs := $(subst src/,,$(sources:.c=.o))

.PHONY: all
all: $(LIB)/libmxIgraph.so mxIgraph.h

$(LIB)/libmxIgraph.so: $(objs)
	$(CC) $(CFLAGS) -shared $^ -o $@

mxGraph.o: mxIterators.o
mxIterators.o: mxIterators.h

%.o: %.c mxIgraph.h
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) -c $<

.PHONY: clean
clean:
	-rm -r *.o

.PHONY: clean-dist
clean-dist: clean
	-rm $(LIB)/libmxIgraph.so
