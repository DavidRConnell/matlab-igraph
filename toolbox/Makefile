CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
LDFLAGS += -Wl,-z,noexecstack -L$(LIB)
MEXFLAGS = -R2018a -outdir $(outdir)

mexdir := ../mex
outdir := +igraph/private

vpath %.c $(mexdir)
vpath %.h $(mexdir)
vpath %.$(libext) $(LIB)

sources := $(wildcard $(mexdir)/*.c)
mexfiles := $(subst $(mexdir)/,$(outdir)/,$(sources:.c=.$(mexext)))

.PHONY: all
all: $(mexfiles)

$(outdir)/%.$(mexext): %.c libmxIgraph.$(libext) libigraph.$(libext) utils.h
	-[ -d $(outdir) ] || mkdir $(outdir)
	mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
	  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) $< -lmxIgraph -ligraph

.PHONY: clean
clean:
	-$(RM) $(outdir)/*.$(mexext)
