LIB := ../$(LIB)
CPPFLAGS += -I../mxIgraph/include -I../igraph-core/include -I../$(BUILD)/igraph/include
LDFLAGS += -Wl,-rpath=$(LIB) -L$(LIB)
MEXFLAGS = -R2018a -outdir $(outdir)
# TODO: Test for OS
mexext := mexa64
outdir := +igraph

vpath %.c mex
vpath %.so $(LIB)

.PHONY: all
all: $(outdir)/cluster.(mexext)

$(outdir)/*.$(mexext): mex/%.c -ligraph -lmxIgraph
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) $^

.PHONY: clean
clean:
	-rm -r $(outdir)/*.(mexext)
