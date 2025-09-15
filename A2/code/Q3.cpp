#include <iostream>
#include <vector>

using namespace std;

const int inf = 1000000000;  

vector<vector<int>> stations;
vector<vector<int>> dist;
vector<int> ted;

void input()
{
    int n;
    cin >> n;
    stations.resize(n + 1);
    dist.resize(n + 1);
    ted.resize(n + 1);

    for (int i = 1; i <= n; i++)
    {
        cin >> ted[i];
        stations[i].resize(ted[i] + 1);
        dist[i].resize(ted[i] + 1);
        for (int j = 1; j <= ted[i]; j++)
        {
            cin >> stations[i][j];
            dist[i][j] = inf;
        }
    }
}

void calc_dist(int x, int y)
{
    int d = dist[x][y];
    if (y > 1 && d + 1 < dist[x][y - 1])
    {
        dist[x][y - 1] = d + 1;
        calc_dist(x, y - 1);
    }
    if (y < ted[x] && d + 1 < dist[x][y + 1])
    {
        dist[x][y + 1] = d + 1;
        calc_dist(x, y + 1);
    }
    int i = stations[x][y];
    if (i)
    {
        for (int j = 1; j <= ted[i]; j++)
        {
            if (stations[i][j] == x && d + 2 < dist[i][j])
            {
                dist[i][j] = d + 2;
                calc_dist(i, j);
            }
        }
    }
}

int main()
{
    input();

    int source[2], des[2];
    cin >> source[0] >> source[1] >> des[0] >> des[1];
    dist[source[0]][source[1]] = 0;
    calc_dist(source[0], source[1]);
    cout << dist[des[0]][des[1]] << endl;
}
