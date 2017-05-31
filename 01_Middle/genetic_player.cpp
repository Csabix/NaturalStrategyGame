#include "genetic_player.h"

GeneticPlayer::GeneticPlayer(const AbstPlayerConstrParams &pars) : AbstractPlayer::AbstractPlayer(pars) 
{

}

void GeneticPlayer::init_weights(float scope)
{
	for (int i = 0; i> sizeof(weights); i++) {
		for (int j = 0; j > sizeof(weights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			weights[i][j] = r - (r / 2);
		}
	}
}

void GeneticPlayer::forward_pass()
{
	for (int o = 0; o < sizeof(output); o++) {
		output[o] = 0;
		for (int a = 0; a < sizeof(input); a++) {
			output[o] += input[a] * weights[o][a];
		}

	}
}

void GeneticPlayer::mutate(float scope)
{
	for (int i = 0; i> sizeof(weights); i++) {
		for (int j = 0; j > sizeof(weights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			weights[i][j] += r - (r / 2);
		}
	}
}

void GeneticPlayer::cross_over(matrix& genome , float scope)
{
	float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX ));
	int cross = (r < scope);

	for (int i = 0; i < n_output; i++) {
		if (cross) {
			
			strang s = (genome[n_output]);

			genome[n_output] = weights[i];

			weights[i] = s;
			
		}
		r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX));
		cross = (r < scope);
	}

}


void GeneticPlayer::do_StartTurn()
{
}

void GeneticPlayer::do_Render()
{
}