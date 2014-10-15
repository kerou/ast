include Sources.list

OBJ= $(patsubst %.cpp, .obj/%.o, $(SOURCES))
DEP_FILES= $(patsubst %.cpp, .deps/%.P, $(SOURCES))

CPPFLAGS= -std=c++0x
CXX= g++
CXX_COMPILE=$(CXX) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
CXX_DEPENDENCIES=-Wp,-MD,.deps/$(*F).P
CXX_LINK=$(CXX) $(CFLAGS) $(LDFLAGS) -L.
DISTDIR=$(top)ast-$(VERSION)

all: TestInput
	@echo "Built TestInput from TestInput.ast"
#	@cp astbin ~/bin/astbin
#	@cp ast ~/bin/ast
#	@cp -r astres ~/bin/

#install: astbin
#	@cp -r astres /usr/local/astres

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

%.tab.cpp: %.y
	bison -d $*.y -o $*.tab.cpp

%.lex.cpp: %.l
	flex -o$*.lex.cpp $*.l

.obj/%.o: %.cpp
	@echo '$(CXX_COMPILE) -c $<'; \
	$(CXX_COMPILE) $(CXX_DEPENDENCIES) -o $@ -c $<

TestInput: .obj $(OBJ)
	$(CXX_LINK) -o $@ $(OBJ) $(LDDEPS)

%.y: %.ast
	ast $*.ast
.obj:
	mkdir .obj
tags: 


clean:
	rm -f -r .obj .deps *.tab.* *.lex.cpp TestInput TestInput.l TestInput.y TestInput.h TestInput.cpp TestInput.yy.cpp TestInput.bisonreport *.o
