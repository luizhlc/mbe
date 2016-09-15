###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

################################## VARIABLES ##################################
# Directories
SRCDIR    :=src
HDRDIR    :=include
BUILDIR   :=build
BINDIR    :=bin
TESTDIR   :=tests
DEPDIR    :=.deps
# Compiler & linker flags
LDLIBS    :=-lm -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS   := 
CXXFLAGS  :=-std=c++14 -Wall -O3 -funroll-loops
INCLUDE   :=-I$(HDRDIR)
# Files
SRC       :=$(shell find $(SRCDIR) -name '*.cpp')
DEP       :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(SRC))
OBJ       :=$(patsubst %.cpp,$(BUILDIR)/%.o,$(SRC))
MAIN      :=main
EXEC      :=$(BINDIR)/execute_me
PUREOBJ   :=$(filter-out $(BUILDIR)/$(SRCDIR)/$(MAIN).o,$(OBJ))
TSTFILE   :=$(shell find $(TESTDIR) -name '*.cpp' 2> /dev/null)
TSTDEP    :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(TSTFILE))
TSTOBJ    :=$(patsubst %.cpp,$(BUILDIR)/%.o,$(TSTFILE))
TSTEXEC   :=$(patsubst $(TESTDIR)/%.cpp,$(BINDIR)/%,$(TSTFILE))

.PHONY: all makedir clean clean_deps clean_all tests

################################# MAIN RULES ##################################
all: makedir $(EXEC)

$(EXEC): $(OBJ)
	@echo "[linking] $@"
	@$(CXX) $(OBJ) -o $@ $(LDLIBS) $(LDFLAGS)

$(BUILDIR)/%.o: %.cpp
	@echo "[  $(CXX)  ] $< -> .o"
	@mkdir -p $(BUILDIR)/$(*D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# For each .cpp file, creates a .d file with all dependencies of .cpp,
# including .d as target (to ensure up-to-date dependencies, in case of
# new includes being added)
$(DEPDIR)/%.d: %.cpp
	@echo "[makedep] $< -> .d"
	@mkdir -p $(DEPDIR)/$(*D)
	@$(CXX) -MM -MP -MT "$(BUILDIR)/$*.o $@" -MF "$@" $< $(INCLUDE) $(CXXFLAGS)

makedir: | $(BINDIR) $(HDRDIR) $(SRCDIR)

$(BINDIR) $(HDRDIR) $(SRCDIR):
	@echo "[ mkdir ] Creating directory '$@'"
	@mkdir -p $@

################################ TESTS RULES ##################################
tests: makedir $(TSTEXEC)

$(TSTEXEC): $(PUREOBJ) $(TSTOBJ)
	@echo "[linking] $@"
	@$(CXX) $(PUREOBJ) $(BUILDIR)/$(TESTDIR)/$(@F).o -o $@ $(LDLIBS) $(LDFLAGS)

################################ CLEAN RULES ##################################
# Only remove object files
clean:
	@$(RM) -r $(BUILDIR)

# Only remove dependency files
clean_deps:
	@$(RM) -r $(DEPDIR)

# Remove object, binary and dependency files
clean_all: clean clean_deps
	@$(RM) -r $(BINDIR)

################################## INCLUDES ###################################
# Do not include list of dependencies when they are going to be deleted, i.e.,
# when the target is clean_all or clean_deps
ifneq ($(MAKECMDGOALS), clean_all)
ifneq ($(MAKECMDGOALS), clean_deps)
-include $(DEP)
-include $(TSTDEP)
endif
endif
