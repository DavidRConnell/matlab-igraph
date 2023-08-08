CFLAGS += -Wall -Wextra -Wno-format -Wno-format-security
CPPFLAGS += $(INCLUDE)
LDFLAGS += -L$(LIB) -lmxIgraph -ligraph -Wl,-rpath=$(LIB)
MEXFLAGS := -R2018a

sources = $(wildcard $(target)/mex/*.c)
objs = $(subst $(target)/mex,$(target)/private,$(sources:.c=.$(mexext)))

define mex-cmd =
@cd $1; [ -d private ] || mkdir private
@cd $1; mex $(CPPFLAGS) "LDFLAGS=$$LDFLAGS $(LDFLAGS)" "CFLAGS=$$CFLAGS $(CFLAGS)" \
  "COPTIMFLAGS=$$COPTIMFLAGS $(COPTIMFLAGS)" $(MEXFLAGS) \
  -outdir private mex/$(notdir $<)
endef

vpath %.c mxIgraph/mex toolbox/mex
vpath %.so $(LIB)

.PHONY: all
all: mxIgraph toolbox

.PHONY: mxIgraph
target := mxIgraph
mxIgraph: $(objs)

mxIgraph/private/%.$(mexext): %.c libmxIgraph.so libigraph.so
	$(call mex-cmd,mxIgraph)

.PHONY: toolbox
target := toolbox
toolbox: $(objs)

toolbox/private/%.$(mexext): %.c libmxIgraph.so libigraph.so
	$(call mex-cmd,toolbox)

.PHONY: clean
clean:
	-rm -r mxIgraph/private/*.$(mexext)
	-rm -r toolbox/private/*.$(mexext)
