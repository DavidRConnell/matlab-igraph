export BUILD := $(PWD)/build
export LIB := $(PWD)/toolbox/lib
export DIST := $(PWD)/dist
export INCLUDE := -I$(PWD)/mxIgraph/include -I$(PWD)/igraph-core/include -I$(BUILD)/igraph/include

ifeq ($(OS), Windows_NT)
arch := win64
export mexext := mexw64
export libext := dll
else ifeq ($($shell uname -s), Darwin)
arch := maci64
export mexext := mexmaci64
export libext := so
else
arch := linux
export mexext := mexa64
export libext := so
endif

.PHONY: all
all: toolbox

.PHONY: toolbox
toolbox: mxIgraph
	@cd toolbox; $(MAKE) all

.PHONY: mxIgraph
mxIgraph: igraph | $(LIB) $(BUILD)
	@cd mxIgraph; $(MAKE) all

.PHONY: igraph
igraph: $(LIB)/libigraph.$(libext) | $(LIB) $(BUILD)

$(BUILD):
	mkdir $(BUILD)

$(LIB):
	mkdir $(LIB)

$(LIB)/libigraph.$(libext): soname := libigraph.$(libext)
$(LIB)/libigraph.$(libext): $(BUILD)/igraph/Makefile
	@cd $(BUILD)/igraph; cmake --build .
	cp $(BUILD)/igraph/src/$(soname)* $(LIB)

# CHANGELOG.md dependency is a fragile hack. Need a better method to determine
# if igraph-core has been changed.
$(BUILD)/igraph/Makefile: igraph-core/CHANGELOG.md | $(BUILD)
	@[ -d $(BUILD)/igraph ] || mkdir $(BUILD)/igraph
	cmake -B $(BUILD)/igraph -S $(PWD)/igraph-core \
	  -DIGRAPH_GRAPHML_SUPPORT=ON \
	  -DIGRAPH_GLPK_SUPPORT=ON \
	  -DIGRAPH_OPENMP_SUPPORT=ON \
	  -DIGRAPH_ENABLE_LTO=AUTO \
	  -DIGRAPH_ENABLE_TLS=ON \
	  -DBUILD_SHARED_LIBS=ON;

.PHONY: check-igraph
check-igraph: igraph
	@cd $(BUILD)/igraph; $(MAKE) build_tests
	@cd $(BUILD)/igraph; $(MAKE) test

.PHONY: check
check: toolbox
	@cd tests; $(MAKE) all
	matlab -batch "buildtool test"

docs: toolbox
	matlab -softwareopengl -batch "buildtool makeDocs"

.PHONY: dist
dist: $(DIST)/matlab-igraph-$(arch).tar.gz

$(DIST)/matlab-igraph-$(arch).tar.gz: toolbox
	-[ -d $(DIST) ] || mkdir $(DIST)
	tar -czf $@ toolbox

.PHONY: clean-dist
clean-dist: clean
	-[ -d $(DIST) ] && $(RM) -r $(DIST)
	-[ -d $(BUILD) ] && $(RM) -r $(BUILD)
	-[ -d $(LIB) ] && $(RM) -r $(LIB)

.PHONY: clean
clean:
	@cd toolbox; $(MAKE) clean
	@cd mxIgraph; $(MAKE) clean
	@cd tests; $(MAKE) clean
