export BUILD := build
export LIB := $(BUILD)/lib

vpath %.a $(LIB)

.PHONY: all
all: toolbox

.PHONY: toolbox
toolbox: mxIgraph igraph
	@cd toolbox; $(MAKE) all

.PHONY: mxIgraph
mxIgraph: build igraph
	@cd mxIgraph; $(MAKE) all

.PHONY: build
build:
	@[ -d $(BUILD) ] || mkdir $(BUILD)
	@[ -d $(LIB) ] || mkdir $(LIB)

.PHONY: igraph
igraph: build $(LIB)/libigraph.so

$(LIB)/libigraph.so: $(BUILD)/igraph/Makefile
	@cd $(BUILD)/igraph; $(MAKE) all
	@cp $(BUILD)/igraph/src/libigraph.so $@

$(BUILD)/igraph/Makefile: igraph-core/CHANGELOG.md
	@[ -d $(BUILD)/igraph ] || mkdir $(BUILD)/igraph
	cd $(BUILD)/igraph; \
	cmake ../../igraph-core \
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
	matlab -nodesktop -nosplash -r "buildtool tests";

.PHONY: dist
dist: clean-dist matlab-igraph.tar.gz

matlab-igraph.tar.gz: toolbox
	tar -xzf $@ build

.PHONY: clean-dist
clean-dist: clean
	@rm -rf build
	@rm *.tar.gz

.PHONY: clean
clean:
	@cd toolbox; $(MAKE) clean
	@cd mxIgraph; $(MAKE) clean
