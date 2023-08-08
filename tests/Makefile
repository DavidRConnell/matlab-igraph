CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += -I../mxIgraph/include -I../igraph-core/include -I../$(BUILD)/igraph/include
LDFLAGS += -L$(LIB) -lmxIgraph -ligraph -Wl,-rpath=$(PWD)/$(LIB)
MEXFLAGS = -R2018a
LIB := ../$(LIB)

mexext := mexa64
outdir := mxIgraph/private
sources := $(wildcard mxIgraph/mex/*.c)

vpath %.c mxIgraph/mex
vpath %.so $(LIB)

.PHONY: all
all: $(subst mxIgraph/mex,$(outdir),$(sources:.c=.$(mexext)))

$(outdir)/%.$(mexext): %.c libmxIgraph.so
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) \
	  -outdir $(outdir) $<

.PHONY: clean
clean:
	-rm -r $(outdir)/*.$(mexext)
