CFLAGS += -fPIC -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
BUILD := $(BUILD)/mxIgraph
MEXFLAGS := -R2018a -outdir $(BUILD)

vpath %.h include src
vpath %.c src
vpath %.o $(BUILD)

sources := $(wildcard src/mx*.c)
objs := $(subst src/,$(BUILD)/,$(sources:.c=.o))

.PHONY: all
all: $(LIB)/libmxIgraph.so

$(LIB)/libmxIgraph.so: $(objs) | $(BUILD)
	$(CC) $(CFLAGS) -shared $^ -o $@

mxGraph.o: mxIterators.o
mxIterators.o: mxIterators.h

$(BUILD)/%.o: %.c mxIgraph.h
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) -c $<

$(BUILD):
	mkdir $(BUILD)

.PHONY: clean
clean:
	-rm -r $(BUILD)

.PHONY: clean-dist
clean-dist: clean
	-rm $(LIB)/libmxIgraph.so
