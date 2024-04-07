//Muhammad Muaz Ahmed | 22i-1125 | Final Project

void move(int delta_x)
{
    for (int i=0;i<4;i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        point_1[i][0] += delta_x;
    }

    if (!anamoly()) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }
    }
}

void rotates(int px, int py)
{
    for (int i=0;i<4;i++)
    {
        int x = point_1[i][1] - py;
        int y = point_1[i][0] - px;
        point_1[i][0] = px - x;
        point_1[i][1] = py + y;
    }
}

void checkLines(int &score)
{
    int linescore = 0;
    int k = M - 1;

    for (int i = M - 1; i >= 0; i--) {
        int count = 0;

        for (int j = 0; j < N; j++) {
            if (gameGrid[i][j])
                count++;

            gameGrid[k][j] = gameGrid[i][j];
        }

        if (count == N)
            linescore++;

        if (count < N)
            k--;
    }

    if(linescore == 1)
    {score = score + 10;}
    else if (linescore == 2)
    {score = score + 30;}
    else if (linescore == 3)
    {score = score + 60;}
    else if (linescore == 4)
    {score = score + 100;}
}

void fallingPiece(float& timer, float& delay, int& colorNum, bool &drop)
{
    if (timer>delay || drop)
    {
        for (int i=0; i < 4; i++)
        { 
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1] += 1;
        }

        if (!anamoly())
        {
            for (int i=0;i<4;i++)
                gameGrid[point_2[i][1]][point_2[i][0]] = colorNum;

            colorNum = 1 + rand() % 7;
            int n = rand() % 7;
            for (int i = 0 ;i < 4; i++) {
                point_1[i][0] = BLOCKS[n][i] % 2;
                point_1[i][1] = BLOCKS[n][i] / 2;
            }

	    drop = false;
        }

        timer=0;
    }

}

void readHighscore(int highscore[])
{
    std::ifstream hsfile("highscore.txt");

    int n;
    while (hsfile >> n)
        *highscore++ = n;

    hsfile.close();
}

void updateHighscore(int highscore[], int score)
{
    for (int i = 0; i < 10; i++) {
        if (score > highscore[i]) {
            int tmp = highscore[i];
            highscore[i] = score;
            score = tmp;
        }
    }

    // Write to file
    std::ofstream hsfile("highscore.txt");

    for (int i = 0; i < 10; i++)
        hsfile << highscore[i] << std::endl;

    hsfile.close();
}
