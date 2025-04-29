ifeq ($(OS),Windows_NT)
	DIR_SEP = \\
	MKDIR = @mkdir
	LFLAGS = -L.\\lib-mingw32 -lglew32 -lglfw3
	LFLAGS2 = -lopengl32
        CFLAGS = -std=c++14 -O0 -ggdb -Iinclude -Iinclude\\glm
else
	DIR_SEP = /
	MKDIR = @mkdir -p
	LFLAGS = -lglfw -lGL -lGLEW
	LFLAGS2 =
	CFLAGS = -std=c++17 -O0 -ggdb -Iinclude/glm
endif

COMPILER = g++
LINKER = g++
MODULES = src src_utils
SOURCE_DIR = .
TEMP_DIR = tmp
TARGET = opengl

SRC_DIR = $(addprefix $(SOURCE_DIR)$(DIR_SEP),$(MODULES))
BUILD_DIR = $(addprefix $(TEMP_DIR)$(DIR_SEP),$(MODULES))

SOURCES = $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJECTS = $(patsubst $(SOURCE_DIR)$(DIR_SEP)%.cpp,$(TEMP_DIR)$(DIR_SEP)%.o,$(SOURCES))
DEPS = $(patsubst $(SOURCE_DIR)$(DIR_SEP)%.cpp,$(TEMP_DIR)$(DIR_SEP)%.d,$(SOURCES))
INCLUDES = $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(COMPILER) -MMD -MP $(CFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: run all checkdirs clean

ifeq ($(OS),Windows_NT)
all: checkdirs $(TARGET)
	copy lib-mingw32\\glew32.dll glew32.dll
	copy lib-mingw32\\glfw3.dll glfw3.dll
else
all: checkdirs $(TARGET)
endif

run: all
	.$(DIR_SEP)$(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $(LFLAGS) $^ -o $(TARGET) $(LFLAGS2)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	$(MKDIR) $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f .$(DIR_SEP)$(TARGET)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))

-include $(DEPS)


