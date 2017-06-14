#include "genetic_strategy.h"
#include "genetic_game_master.h"

UNIT_TYPE GeneticStrategy::train()
{
	return UNIT_TYPE(); //TODO! It has to decide what unit to train
}

void GeneticStrategy::changeOrders(AllOrders &orders)
{

	std::vector<int> input (n_input);
	//divided in quarters: [northwest-scissors, northwest-stones,northwest-papers, northeeast-scissors...]

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			Unit* u = board(Position(i,j),player).unit; //the following looks complicated
			// if u is nullptr then there is no unit there!
			// i/board.size() always zero here! integer division!, try 2*i/board.size(), this can be 0 or 1.
			input[4 * (i / board.size() + (2 * (j / board.size()))) + ((u -> type)*((u -> id)==player))]++;
		}
	}
	int index = 12;
	input[index] = board.getPlayerAtOutpost(0, player);
	index++;
	input[index] = board.getPlayerAtOutpost(1, player);
	index++;
	//is my homebase occupied?
	//Home is (0,0)
	//Enemy base is (board.size()-1, board.size()-1) !!
	input[index] = (int)(board(Position(board.size(), board.size()), player).unit->player != player);
}

GeneticStrategy::GeneticStrategy(int input, int output, int scope)
{
	n_input = input;
	n_output = output;
	initiate_weights(0.2);
}

//int * GeneticTactics::gpass(int input[]) {
//	forward_pass(gweights, input);
//}

//Weight matrix forward pass
std::vector<int> GeneticStrategy::wpass(std::vector<int> input) {
	return forward_pass(weights, input);
}

//initiate weights with some randomness
void GeneticStrategy::initiate_weights(float scope)
{
	initiate_abst_weights(weights, scope);
}

//void GeneticTactics::initiate_gweights(float scope)
//{
//	initiate_abst_weights(gweights, scope);
//}

void GeneticStrategy::initiate_abst_weights(matrix &lweights, float scope)
{
	for (int i = 0; i < lweights.size(); i++) {
		for (int j = 0; j < lweights[i].size(); j++) {
			lweights[i][j] = get_rand(-scope, scope);
		}
	}
}

std::vector<int> GeneticStrategy::forward_pass(const matrix &lweights, const std::vector<int> &input)
{
	assert(lweights.size() == n_output && lweights[0].size() == input.size());
	std::vector<int> output(n_output, 0);
	for (int o = 0; o < output.size(); o++) {
		for (int a = 0; a < input.size(); a++) {
			output[o] += input[a] * lweights[o][a];
		}
	}
	return output;
}

//mutate weights
void GeneticStrategy::mutate(float scope)
{
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			weights[i][j] += get_rand(-scope, scope);
		}
	}
}

//cross over with external weight matrix.
void GeneticStrategy::cross_over(matrix& genome, float scope)
{
	for (int i = 0; i < n_output; i++) {
		float r = get_rand(0, 1);
		if (r < scope) continue;
		genome[i].swap(weights[i]);
	}
}


/*input:
0-7: surrounding fields
8,9: x,y of own position
10,11: own type
12 : General command
13: bias
*/

