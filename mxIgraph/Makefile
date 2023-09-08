CFLAGS += -fPIC -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
MXIGRAPHBUILD := $(BUILD)/mxIgraph
MEXFLAGS := -R2018a -outdir $(MXIGRAPHBUILD)

vpath %.h include src
vpath %.c src
vpath %.o $(MXIGRAPHBUILD)

sources := $(wildcard src/mx*.c)
objs := $(subst src/,$(MXIGRAPHBUILD)/,$(sources:.c=.o))

.PHONY: all
all: $(MXIGRAPHBUILD) $(LIB)/libmxIgraph.so

$(LIB)/libmxIgraph.so: $(objs)
	$(CC) $(CFLAGS) -shared $^ -o $@

mxGraph.o: mxIterators.o
mxIterators.o: mxIterators.h

$(MXIGRAPHBUILD)/%.o: %.c mxIgraph.h
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) -c $<

.PHONY: $(MXIGRAPHBUILD)
$(MXIGRAPHBUILD):
	[ -d $(MXIGRAPHBUILD) ] || mkdir $(MXIGRAPHBUILD)

.PHONY: clean
clean:
	-rm -r $(MXIGRAPHBUILD)

.PHONY: clean-dist
clean-dist: clean
	-rm $(LIB)/libmxIgraph.so
