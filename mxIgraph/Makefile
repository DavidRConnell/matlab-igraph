CFLAGS += -fPIC -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += -Iinclude -I../igraph-core/include -I../$(BUILD)/igraph/include
LDFLAGS += -L$(LIB)
MEXFLAGS = -R2018a
LIB := ../$(LIB)

mexext := mexa64

vpath %.so $(LIB)
vpath %.h include
vpath %.c src

.PHONY: all
all: $(LIB)/libmxIgraph.so mxIgraph.h

$(LIB)/libmxIgraph.so: mxGraph.o mxPartition.o mxPredicate.o
	$(CC) $(CFLAGS) -shared $^ -o $@

%.o: %.c mxIgraph.h
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) -c $<

.PHONY: clean
clean:
	-rm -r *.o

.PHONY: clean-dist
clean-dist: clean
	-rm $(LIB)/libmxIgraph.so
