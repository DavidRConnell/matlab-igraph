CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
MXIGRAPHBUILD := $(BUILD)/mxIgraph
MEXFLAGS := -R2018a -outdir $(MXIGRAPHBUILD)

vpath %.h include src
vpath %.c src
vpath %.o $(MXIGRAPHBUILD)

sources := $(wildcard src/mx*.c)
objs := $(subst src/,$(MXIGRAPHBUILD)/,$(sources:.c=.o))

.PHONY: all
all: $(MXIGRAPHBUILD) $(LIB)/libmxIgraph.$(libext)

$(LIB)/libmxIgraph.$(libext): $(objs)
	cd $(MXIGRAPHBUILD)
	$(AR) rcs libmxIgraph.$(libext) $^
	mv libmxIgraph.$(libext) $@

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
	-$(RM) -r $(MXIGRAPHBUILD)

.PHONY: clean-dist
clean-dist: clean
	-$(RM) $(LIB)/libmxIgraph.$(libext)
