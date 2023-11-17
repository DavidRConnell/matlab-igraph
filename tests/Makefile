CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
LDFLAGS += -L$(LIB) -Wl,-z,noexecstack
MEXFLAGS := -R2018a

sources = $(wildcard $(target)/mex/*.c)
objs = $(subst $(target)/mex,$(target)/private,$(sources:.c=.$(mexext)))

define mex-cmd =
@cd $1; [ -d private ] || mkdir private
@cd $1; mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) \
  -outdir private mex/$(notdir $<) -lmxIgraph -ligraph
endef

vpath %.c mxIgraph/mex toolbox/mex
vpath %.$(libext) $(LIB)

.PHONY: all
all: mxIgraph toolbox

.PHONY: mxIgraph
target := mxIgraph
mxIgraph: $(objs)

mxIgraph/private/%.$(mexext): %.c libmxIgraph.$(libext) libigraph.$(libext)
	$(call mex-cmd,mxIgraph)

.PHONY: toolbox
target := toolbox
toolbox: $(objs)

toolbox/private/%.$(mexext): %.c libmxIgraph.$(libext) libigraph.$(libext)
	$(call mex-cmd,toolbox)

.PHONY: clean
clean:
	-$(RM) mxIgraph/private/*.$(mexext)
