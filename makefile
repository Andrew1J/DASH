BIN=./bin
OBJ=./obj
SRC=./src

OUTPUT=$(BIN)/dash
DEPS=$(wildcard $(SRC)/*.h)
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	mkdir -p $(BIN)
	gcc $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c $(DEPS)
	mkdir -p $(@D)
	gcc -c $< -o $@

run: $(OUTPUT)
	$(OUTPUT)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)
