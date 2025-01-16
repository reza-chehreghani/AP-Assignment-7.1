CC = g++ -c --std=c++11

APP_NAME = futballFantasy.out

$(APP_NAME): main.o tools.o futballFantasy.o premierLeague.o team.o player.o weekStats.o fantasyLeague.o fantasyTeam.o
	g++ --std=c++11 main.o tools.o futballFantasy.o premierLeague.o team.o player.o weekStats.o fantasyLeague.o fantasyTeam.o -o $(APP_NAME)

main.o: main.cpp config.hpp
	$(CC) main.cpp -o main.o

tools.o: tools.cpp tools.hpp
	$(CC) tools.cpp -o tools.o

futballFantasy.o: futballFantasy.cpp futballFantasy.hpp config.hpp
	$(CC) futballFantasy.cpp -o futballFantasy.o

premierLeague.o: premierLeague.cpp premierLeague.hpp config.hpp
	$(CC) premierLeague.cpp -o premierLeague.o

team.o: team.cpp team.hpp config.hpp
	$(CC) team.cpp -o team.o

player.o: player.cpp player.hpp config.hpp
	$(CC) player.cpp -o player.o

weekStats.o: weekStats.cpp weekStats.hpp config.hpp
	$(CC) weekStats.cpp -o weekStats.o

fantasyLeague.o: fantasyLeague.cpp fantasyLeague.hpp config.hpp
	$(CC) fantasyLeague.cpp -o fantasyLeague.o

fantasyTeam.o: fantasyTeam.cpp fantasyTeam.hpp fantasyTeam.hpp
	$(CC) fantasyTeam.cpp -o fantasyTeam.o

clear:
	rm *.o
	rm *.out