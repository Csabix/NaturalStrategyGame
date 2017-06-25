#include "Updater.h"
#include "abstract_game.h"
#include "abstract_player.h"
#include "official_game.h"
#include "human_player.h"
#include "super_player.h"
#include "probabilistic_tactic.h"
#include "cyborg_strategy.h"
#include "simple_player.h"

void Updater::AddHumanVsHuman(int game_size)
{
	std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<HumanPlayer>()),
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<HumanPlayer>()),
		game_size);
	tasks.push_back([=](int iters)->bool
	{
		game->Update();
		game->Render();
		return game->getPlayerScore() != glm::dvec2(0);
	});
}

void Updater::AddCyborgVsHeuristics(int game_size)
{
	std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<SuperPlayer>(
			std::static_pointer_cast<AbstractStrategy>(std::make_shared<CyborgStrategy>()),
			std::static_pointer_cast<AbstractTactic>(std::make_shared<ProbabilisticTactic>())
			)),
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<SimplePlayer>()),
		game_size);
	tasks.push_back([=](int iters)->bool
	{
		game->Update();
		game->Render();
		return game->getPlayerScore() != glm::dvec2(0);
	});
}

void Updater::AddCyborgVsHuman(int game_size)
{
	std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<SuperPlayer>(
			std::static_pointer_cast<AbstractStrategy>(std::make_shared<CyborgStrategy>()),
			std::static_pointer_cast<AbstractTactic>(std::make_shared<ProbabilisticTactic>())
			)),
		std::static_pointer_cast<AbstractPlayer>(std::make_shared<HumanPlayer>()),
		game_size);
	tasks.push_back([=](int iters)->bool
	{
		game->Update();
		game->Render();
		return game->getPlayerScore() != glm::dvec2(0);
	});
}