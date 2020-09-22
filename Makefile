TARGET  := boggle
SRCDIR  := src
SRC     := $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJDIR  := obj
OBJ     := $(SRC:%.cpp=$(OBJDIR)/%.o)
DEP     := $(SRC:%.cpp=$(OBJDIR)/%.d)

CC      := g++
CFLAGS  := -MMD -MP -I$(SRCDIR) -std=c++14
WFLAGS  := -Wall -Wextra -Wshadow -Wwrite-strings -Wno-implicit-fallthrough

C_GREEN  := \033[1;32m
C_YELLOW := \033[1;33m
C_NONE   := \033[0m

$(TARGET): $(OBJ)
	@echo "$(C_GREEN)linking$(C_NONE) $@"
	@$(CC) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@echo "$(C_GREEN)compiling\033[0m $<"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@echo "$(C_YELLOW)removing$(C_NONE) $(OBJDIR)/"
	-@rm -r $(OBJDIR)

mrproper: clean
	@echo "$(C_YELLOW)removing$(C_NONE) $(TARGET)"
	-@rm $(TARGET)

all: mrproper $(TARGET)
