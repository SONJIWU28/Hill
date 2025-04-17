#pragma once
#include "Game Settings.h"
#include "Resource.h"
#include "Anthill.h"
#include "Raid.h"
#include "Informers.h"
#include "Tlya.h"
#include "Cemetery.h"

class Game {
public:
	Game() :ticks(0) {};
	~Game() = default;
	Anthill anthill;
	Raid raid;
	Cemetery cemetery;
	vector<Resource> resources;
	vector<Aphid> aphids;

	vector<Text> statsLines;
	Text OVER, YOU;


	static Game* current;
	static Game* get_current() { return current; }
	static void set_current(Game* g) { current = g; }
	void update(Font& font);
	void add_stats(Font& font);
	void spawn_aphids();
	void update_aphids();

	void update_ants();
	void update_enemies();
	void handle_collisions();
	bool check_game_over();
	void spawn_body();

	void reset();
	void spawn_res();
	void over(Font& font);
	//void clean_ants();
	//void clean_enemy();
	void kill_all() { anthill.kill_colony(); }
	int get_ticks() const { return ticks; }
	string to_K(int x);
private:
	int ticks;
	bool has_started_colony = false;
};