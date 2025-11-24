CXX      = g++
AS       = gcc
CXXFLAGS_DEBUG = -g -O0 -Wall -Wextra
CXXFLAGS_OPT   = -O2 -Wall -Wextra
ASFLAGS  = -c

INCLUDES = -Iinclude
SRCDIR   = src
OBJDIR   = obj

OBJS_DEBUG = \
    $(OBJDIR)/main.o \
    $(OBJDIR)/basicIO.o \
    $(OBJDIR)/UserDevice.o \
    $(OBJDIR)/CellularCore.o \
    $(OBJDIR)/CellTower.o \
    $(OBJDIR)/Simulator.o \
    $(OBJDIR)/syscall.o

OBJS_OPT = \
    $(OBJDIR)/main_opt.o \
    $(OBJDIR)/basicIO_opt.o \
    $(OBJDIR)/UserDevice_opt.o \
    $(OBJDIR)/CellularCore_opt.o \
    $(OBJDIR)/CellTower_opt.o \
    $(OBJDIR)/Simulator_opt.o \
    $(OBJDIR)/syscall.o

all: sim_debug sim_opt

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Debug objects
$(OBJDIR)/main.o: $(SRCDIR)/main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/basicIO.o: $(SRCDIR)/basicIO.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/UserDevice.o: $(SRCDIR)/UserDevice.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/CellularCore.o: $(SRCDIR)/CellularCore.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/CellTower.o: $(SRCDIR)/CellTower.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/Simulator.o: $(SRCDIR)/Simulator.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(INCLUDES) -c $< -o $@

$(OBJDIR)/syscall.o: $(SRCDIR)/syscall.S | $(OBJDIR)
	$(AS) $(ASFLAGS) $< -o $@

sim_debug: $(OBJS_DEBUG)
	$(CXX) -nostartfiles $(OBJS_DEBUG) -o sim_debug

# Optimized objects
$(OBJDIR)/main_opt.o: $(SRCDIR)/main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/basicIO_opt.o: $(SRCDIR)/basicIO.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/UserDevice_opt.o: $(SRCDIR)/UserDevice.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/CellularCore_opt.o: $(SRCDIR)/CellularCore.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/CellTower_opt.o: $(SRCDIR)/CellTower.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

$(OBJDIR)/Simulator_opt.o: $(SRCDIR)/Simulator.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS_OPT) $(INCLUDES) -c $< -o $@

sim_opt: $(OBJS_OPT)
	$(CXX) -nostartfiles $(OBJS_OPT) -o sim_opt

clean:
	rm -rf $(OBJDIR) sim_debug sim_opt

.PHONY: all clean
