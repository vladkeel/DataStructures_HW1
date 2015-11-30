#ifndef POKEMON_H_
#define POKEMON_H_
class Pokemon{
private:
	int pokemonID;
	int level;
	int trainerID;
	Pokemon(){};
public:
	Pokemon(int id, int lvl, int trn) : pokemonID(id), level(lvl), trainerID(trn){};
	Pokemon(const Pokemon& pkmn) : pokemonID(pkmn.pokemonID), level(pkmn.level), trainerID(pkmn.trainerID){};
	Pokemon& operator=(const Pokemon& pkmn){
		if (this != &pkmn){
			pokemonID = pkmn.pokemonID;
			level = pkmn.level;
			trainerID = pkmn.trainerID;
		}
		return *this;
	}
	~Pokemon(){};
	int getID()const{
		return pokemonID;
	}
	bool operator==(const Pokemon& other)const{
		return (pokemonID == other.pokemonID);
	}
	int getLevel()const{
		return level;
	}
	int getTrainer()const{
		return trainerID;
	}
	void setLevel(int lvl){
		level = lvl;
	}
};

#endif