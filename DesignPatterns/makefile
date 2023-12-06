# Based on:
# https://stackoverflow.com/questions/27001837/makefile-to-compile-all-cpp-files-in-all-subdirs#answer-27025822

SRCDIR := .
OBJDIR := obj
DEPDIR := dep

SRCS     := $(shell find $(SRCDIR) -name "*.cpp")
OBJS     := $(SRCS:$(SRCDIR)/%.cpp=%.o)
# DEPS     := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
# TREE     := $(patsubst %/,%,$(dir $(OBJS)))
CPPFLAGS  = -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

.PHONY: all clean

all: $(OBJS)

.SECONDEXPANSION:
%.o: $(SRCDIR)/%.cpp | $$(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# $(TREE): %:
# 	mkdir -p $@
# 	mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)

clean:
	$(RM) -r $(OBJDIR) $(DEPDIR)

ifeq "$(MAKECMDGOALS)" ""
# -include $(DEPS)
endif