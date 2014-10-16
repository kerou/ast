SOURCES= main.cpp
AST_SOURCES= TestInput.ast
EXE_NAME= TestInput

OBJ= $(patsubst %.cpp, %.o, $(SOURCES))
DEP_FILES= $(patsubst %.cpp, .deps/%.P, $(SOURCES))
LIBS= $(patsubst %.ast, lib%.a, $(AST_SOURCES))
LDDEPS= $(patsubst %.ast, -l%, $(AST_SOURCES))
GENERATED_HEADERS= $(patsubst %.ast, %.h, $(AST_SOURCES))

CPPFLAGS= -std=c++0x
CXX= g++
CXX_COMPILE=$(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
CXX_DEPENDENCIES=-Wp,-MD,.deps/$(*F).P
CXX_LINK=$(CXX) $(CFLAGS) $(LDFLAGS) -L.
DISTDIR=$(top)ast-$(VERSION)

all: $(EXE_NAME)
	@echo "Builded"

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

%.o: %.cpp
	@echo '$(CXX_COMPILE) -c $<'; \
	$(CXX_COMPILE) $(CXX_DEPENDENCIES) -o $@ -c $<

$(EXE_NAME): $(OBJ) $(LIBS)
	$(CXX_LINK) -o $@ $(OBJ) $(LDDEPS)

%.h lib%.a: %.ast
	ast $*.ast -a
.obj:
	mkdir .obj
.lib:
	mkdir .lib
tags: 


clean:
	rm -f -r .obj .deps *.tab.* *.lex.cpp $(EXE_NAME) $(LIBS) $(GENERATED_HEADERS)
