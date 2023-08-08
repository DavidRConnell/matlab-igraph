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

$(LIB)/libigraph.so: soname := libigraph.so
$(LIB)/libigraph.so: version := \
  $(shell sed -n 's/set_target_properties(.* VERSION "\([0-9\.]*\)")/\1/p' <\
  igraph-core/src/CMakeLists.txt)
$(LIB)/libigraph.so: soversion := \
  $(shell sed -n 's/set_target_properties(.* SOVERSION \([0-9\.]*\))/\1/p' <\
  igraph-core/src/CMakeLists.txt)
$(LIB)/libigraph.so: $(BUILD)/igraph/Makefile
	@cd $(BUILD)/igraph; $(MAKE) all
	cp $(BUILD)/igraph/src/$(soname).$(version) $(LIB)
	@cd $(LIB); \
        [ -L $(soname).$(soversion) ] || \
          ln -s $(soname).$(version) $(soname).$(soversion); \
        [ -L $(soname) ] || \
	  ln -s $(soname).$(soversion) $(soname)

# CHANGELOG.md dependency is a fragile hack. Need a better method to determine
# if igraph-core has been changed.
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
check: mxIgraph
	@cd tests; $(MAKE) all
	matlab -nodesktop -nosplash -r "buildtool test; exit"

.PHONY: dist
dist: clean-dist matlab-igraph.tar.gz

matlab-igraph.tar.gz: toolbox
	tar -xzf $@ build

.PHONY: clean-dist
clean-dist: clean
	-[ -d build ] && rm -rf build
	-[ -f *.tar.gz ] && rm *.tar.gz

.PHONY: clean
clean:
	@cd toolbox; $(MAKE) clean
	@cd mxIgraph; $(MAKE) clean
