CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
LDFLAGS += -Wl,-rpath=$(LIB) -L$(LIB) -ligraph -lmxIgraph
MEXFLAGS = -R2018a -outdir $(outdir)

vpath %.c mex
vpath %.h mex
vpath %.so $(LIB)

outdir := +igraph/private
sources := $(wildcard mex/*.c)
mexfiles := $(subst mex/,$(outdir)/,$(sources:.c=.$(mexext)))

.PHONY: all
all: $(mexfiles)

$(outdir)/%.$(mexext): %.c libigraph.so libmxIgraph.so utils.h
	-[ -d $(outdir) ] || mkdir $(outdir)
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) $<

.PHONY: clean
clean:
	-rm $(outdir)/*.$(mexext)
