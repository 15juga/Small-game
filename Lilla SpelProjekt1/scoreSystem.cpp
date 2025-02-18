#include "scoreSystem.h"

ScoreSystem::ScoreSystem()
{
	//Score för antal coins och points för tiden
	this->points = 0;
	this->player1Score = 0;
	this->player2Score = 0;
	this->finalScore1 = 0;
	this->finalScore2 = 0;
}

ScoreSystem::~ScoreSystem()
{
}

int ScoreSystem::getPoints()
{
	return this->points;
}

int ScoreSystem::getPlayerScore()
{
	return this->player1Score;
}

int ScoreSystem::getPlayer2Score()
{
	return this->player2Score;
}

int ScoreSystem::addPlayerScore(int addScorePlayer)
{
	this->player1Score += addScorePlayer;
	return this->player1Score;
}

int ScoreSystem::losePlayerScore(int loseScorePlayer)
{
	this->player1Score -= loseScorePlayer;
	return this->player1Score;
}

int ScoreSystem::addPlayer2Score(int addScorePlayer2)
{
	this->player2Score += addScorePlayer2;
	return this->player2Score;
}

int ScoreSystem::losePlayer2Score(int loseScorePlayer2)
{
	this->player2Score -= loseScorePlayer2;
	return this->player2Score;
}

void ScoreSystem::SetPlayerScores(int score1, int score2)
{
	this->player1Score = score1;
	this->player2Score = score2;
}

int ScoreSystem::addPoints(int addpointsPlayer)
{
	this->points += addpointsPlayer;
	return points;
}

int ScoreSystem::losePoints(int losepointsPlayer)
{
	this->points -= losepointsPlayer;
	return this->points;
}

int ScoreSystem::calcFinalScorePlayer1()
{
	this->finalScore1 += this->getPlayerScore() + this->getPoints();
	return this->finalScore1;
}

int ScoreSystem::calcFinalScorePlayer2()
{
	this->finalScore2 += this->getPlayer2Score() + this->getPoints();
	return this->finalScore2;
}

int ScoreSystem::getFinalScore1()
{
	return this->finalScore1;
}

int ScoreSystem::getFinalScore2()
{
	return this->finalScore2;
}
