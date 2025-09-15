#include <iostream>
#include <vector>
#include <string>

using namespace std;

void input(vector<string> &name, vector<int> &NumOfLine, int NumOfItems)
{
    string storage;
    int d;

    for (int i = 1; i <= NumOfItems; i++)
    {
        cin >> storage;
        name.push_back(storage);

        cin >> d;
        NumOfLine.push_back(d);  
    }
}

void rec(const vector<string> &name, const vector<int> &NumOfLine, int currentLevel, int parentIndex, int MaxDeep)
{
    for (int i = 0; i < name.size(); i++)
    {
        if (NumOfLine[i] == parentIndex)
        {
            for (int j = 0; j < currentLevel  && j < MaxDeep; j++)
                cout << "|   ";
            cout << "|__" << name[i] << endl;
            rec(name, NumOfLine, currentLevel + 1, i + 1, MaxDeep);
        }
    }
}

int main()
{
    vector<string> name;
    vector<int> NumOfLine;
    int NumOfItems, MaxDeep;

    cin >> NumOfItems;
    cin >> MaxDeep;

    input(name, NumOfLine, NumOfItems);

    rec(name, NumOfLine, 0, 0, MaxDeep);

    return 0;
}
