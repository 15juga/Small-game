#pragma once
#include "GameObject.h"

class ScoreSystem
{
private:
	int points;
	int player1Score;
	int player2Score;
	int finalScore1;
	int finalScore2;
protected:

public:
	ScoreSystem();
	~ScoreSystem();

	//Points
	int getPoints();
	int addPoints(int addpointsPlayer);
	int losePoints(int losepointsPlayer);

	//Player points
	int getPlayerScore();
	int getPlayer2Score();
	int addPlayerScore(int addScorePlayer);
	int losePlayerScore(int loseScorePlayer);
	int addPlayer2Score(int addScorePlayer2);
	int losePlayer2Score(int loseScorePlayer2);
	void SetPlayerScores(int score1, int score2);

	//Final Score
	int calcFinalScorePlayer1();
	int calcFinalScorePlayer2();
	int getFinalScore1();
	int getFinalScore2();

};