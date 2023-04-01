
CC=gcc
CFLAGS=-ansi -Wall
LDFLAGS=-lm -lMLV
EXEC=game
SRC=src/
INC=include/
BIN=bin/
OBJ=$(BIN)Main.o $(BIN)Direction.o $(BIN)Point.o $(BIN)Disc.o $(BIN)Player.o $(BIN)Guard.o $(BIN)Grid.o $(BIN)Terrain.o $(BIN)Collision.o $(BIN)Detection.o $(BIN)Ranking.o $(BIN)GameEngine.o $(BIN)Graphic.o

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(BIN)Main.o: $(SRC)Main.c $(INC)GameEngine.h $(INC)Graphic.h $(INC)Ranking.h

$(BIN)Direction.o: $(SRC)Direction.c $(INC)Direction.h

$(BIN)Point.o: $(SRC)Point.c $(INC)Point.h

$(BIN)Disc.o: $(SRC)Disc.c $(INC)Disc.h $(INC)Point.h

$(BIN)Player.o: $(SRC)Player.c $(INC)Player.h $(INC)Point.h $(INC)Disc.h $(INC)Direction.h $(INC)Grid.h

$(BIN)Guard.o: $(SRC)Guard.c $(INC)Guard.h $(INC)Point.h $(INC)Disc.h $(INC)Direction.h $(INC)Grid.h $(INC)Player.h

$(BIN)Grid.o: $(SRC)Grid.c $(INC)Grid.h

$(BIN)Terrain.o: $(SRC)Terrain.c $(INC)Terrain.h $(INC)Grid.h $(INC)Point.h $(INC)Disc.h $(INC)Player.h $(INC)Guard.h

$(BIN)Collision.o: $(SRC)Collision.c $(INC)Collision.h $(INC)Point.h $(INC)Disc.h $(INC)Direction.h $(INC)Grid.h $(INC)Terrain.h $(INC)Guard.h

$(BIN)Detection.o: $(SRC)Detection.c $(INC)Detection.h $(INC)Point.h $(INC)Disc.h $(INC)Guard.h $(INC)Player.h $(INC)Grid.h $(INC)Terrain.h

$(BIN)Ranking.o: $(SRC)Ranking.c $(INC)Ranking.h

$(BIN)GameEngine.o: $(SRC)GameEngine.c $(INC)GameEngine.h $(INC)Grid.h $(INC)Player.h $(INC)Guard.h $(INC)Terrain.h $(INC)Collision.h $(INC)Detection.h

$(BIN)Graphic.o: $(SRC)Graphic.c $(INC)Graphic.h $(INC)GameEngine.h $(INC)Ranking.h

$(BIN)%.o: $(SRC)%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN)*
	rm $(EXEC)
