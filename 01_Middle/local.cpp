#include "local.h"
#include <random>

static std::default_random_engine rnd_engine(42);
static std::uniform_real_distribution<float> rnd_distribution;

Dir Options::choose()
{
	float sums[5] = { data[0] };
	for(int i = 1; i < 5; ++i)
		sums[i] = sums[i - 1] + data[i];
	int ind = 0;	float rnd = rnd_distribution(rnd_engine);
	while(rnd*sums[4] > sums[ind]) ++ind;
	Dir ret = Dir(0, 0);
	switch(ind)
	{
	case 0: ret = Dir(-1, 0); break; //up
	case 1: ret = Dir(0, -1); break; //left
	//case 2: ret = Dir(0, 0); break; //still
	case 3: ret = Dir(0, 1); break; //right
	case 4: ret = Dir(1, 0); break; //down
	//default: throw std::exception("Switch error"); break;
	}
	return ret;
}

void localAvoid3x3(const Unit& unit, const Board &board, Options &options)
{
	if (unit.pos.x > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(-1, 0), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.up = options.still = 0;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 2)
			options.up = 3;
	}
	else options.up = 0;
	if(unit.pos.x < board.size() - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(+1, 0), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.down = options.still = 0;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 2)
			options.down = 3;
	}
	else options.down = 0;
	if (unit.pos.y > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(0, -1), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.left = options.still = 0;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 2)
			options.left = 3;
	}
	else options.left = 0;

	if(unit.pos.y < board.size() - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(0, +1), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.right = options.still = 0;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 2)
			options.right = 3;
	}
	else options.right = 0;

	if (unit.pos.x > 0 && unit.pos.y > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(-1, -1), unit.player).unit;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 1)	options.up = options.left = 0.1;
	}

	if (unit.pos.x > 0 && unit.pos.y < board.size() - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(-1, 1), unit.player).unit;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 1)	options.up = options.right = 0.1;
	}
	
	if (unit.pos.x < board.size() - 1 && unit.pos.y > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(1, -1), unit.player).unit;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 1)	options.down = options.left = 0.1;
	}
	
	if (unit.pos.x < board.size() - 1 && unit.pos.y < board.size() - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(1, 1), unit.player).unit;
		if (other_unit && (unit.type - other_unit->type + 3) % 3 == 1)	options.down = options.right = 0.1;
	}
}

void localNxN(const Unit& unit, const Board &board, Options &options)
{
	const int window_size = 3;
	int xlb = std::max(unit.pos.x - window_size, 0);
	int xub = std::min(unit.pos.x + window_size, board.size() - 1);
	int ylb = std::max(unit.pos.y - window_size, 0);
	int yub = std::min(unit.pos.y + window_size, board.size() - 1);
	const float reaction[3] = { -1.f, -19.0f, +19.0f }; //{both die, we die, we win}
	//const float panicing[3] = { 0.9f, 1.3f, 0.2f }; //{both die, we die, we win}
	const float panicing[3] = { -1.f, 0.1f, 1.f }; //{both die, we die, we win}
	const float mid_favr = 1.f/(float)window_size;
	glm::vec2 vec = glm::vec2(0);
	float panic = 0.5; int num = 0;
	for(int x = xlb; x <= xub; ++x) for(int y = ylb; y <= yub; ++y)
	{
		Position pos = Position(x,y);
		Dir dir = pos - unit.pos;
		Unit *other_unit = board(pos, unit.player).unit;
		if(other_unit && other_unit->player != unit.player) //enemy
		{
			float dist = powf((float)norm1(dir), 2) + 1.f;
			int outcome = (unit.type - other_unit->type + 3) % 3;
			vec += glm::vec2((float)dir.x, (float)dir.y) / dist * reaction[outcome];
			//panic *= panicing[outcome];
			panic *= exp(panicing[outcome]/dist);
			++num;
		}
		else if(other_unit)
		{
			float dist = powf((float)norm1(dir)*0.5f, 4) + 1.f;
			vec += glm::vec2((float)dir.x, (float)dir.y)*mid_favr / dist;
		}
	}
	options.down *= panic;
	options.up *= panic;
	options.right *= panic;
	options.left *= panic;
	options.still *= 0.7f * panic / (panic*panic + 1.0f);

	options.down	+= std::max(0.f, +vec.x);
	options.up		+= std::max(0.f, -vec.x);
	options.right	+= std::max(0.f, +vec.y);
	options.left	+= std::max(0.f, -vec.y);
	options.still	+= 0.5f/(abs(vec.x) + abs(vec.y) + 1.0f);
	unit.movementvec = vec;
	unit.numberofenemys = num;
}
