#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void course_Present(vector<string>& first_subject_name, vector<int>& first_subject_unit, vector<string>& first_subject_prerequisite, int m) {
    string storage;
    int storage2;

    for (int i = 0; i < m; i++) {
        cin >> storage;
        first_subject_name.push_back(storage);

        cin >> storage2;
        first_subject_unit.push_back(storage2);

        cin >> storage;
        first_subject_prerequisite.push_back(storage);
    }
}

void course_Taken(vector<string>& second_subject_name, vector<int>& second_subject_unit, vector<float>& second_subject_score, int n) {
    string storage;
    int storage1;
    float storage2;

    for (int i = 0; i < n; i++) {
        cin >> storage;
        second_subject_name.push_back(storage);

        cin >> storage1;
        second_subject_unit.push_back(storage1);

        cin >> storage2;
        second_subject_score.push_back(storage2);
    }
}

void choosing_Course(vector<string>& third_name, int k) {
    string storage;

    for (int i = 0; i < k; i++) {
        cin >> storage;
        third_name.push_back(storage);
    }
}

double calculate_GPA(vector<int>& second_subject_unit, vector<float>& second_subject_score, int n) {
    double unit_sum = 0;
    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += second_subject_score[i] * second_subject_unit[i];
        unit_sum += second_subject_unit[i];
    }
    return sum / unit_sum;
}

int offer(vector <string>& first_subject_name, vector <string>& third, int& k, int& m)
{
    for (int i = 0; i < k; i++)
    {
        bool found = false;
        for (int j = 0; j < m; j++)
        {
            if (first_subject_name[j] == third[i])
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Course Not Offered!" << endl;
            return 1; 
        }
    }
    return 0;  
}

 
int prerequisite(int n, int m, int k, vector<string> third_name, vector<string> first_subject_name, vector<string> first_subject_prerequisite, vector<string> second_subject_name, vector<float> second_subject_score)
{
    string word;
    int a = 0, b = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (third_name[i] == first_subject_name[j])
            {
                stringstream inp(first_subject_prerequisite[j]);
                while (getline(inp, word, ','))
                {
                    a++;
                    for (int z = 0; z < n; z++)
                    {
                        if (second_subject_name[z] == word && second_subject_score[z] >= 10)
                        {
                            b++;
                            break;  
                        }
                    }
                }
                if (a != b)
                {
                    cout << "Prerequisite Not Met!" << endl;
                    return 1;
                }
            }
        }
    }

    return 0;
}

int unit_check(double gpa, vector <string>& first_subject_name, vector <string>& third_name, vector <int>& first_subject_unit, int m, int k)
{
    int unit_sum = 0;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < m; j++) {
            if (first_subject_name[j] == third_name[i]) {
                unit_sum += first_subject_unit[j];
                break;
            }
        }
    }

    if (gpa > 17) {
        if (unit_sum >= 12 && unit_sum <= 24) {
            cout << "OK!" << endl;
        } else if (unit_sum > 24) {
            cout << "Maximum Units Validation Failed!" << endl;
        } else if (unit_sum < 12) {
            cout << "Minimum Units Validation Failed!" << endl;
        }
    } else if (gpa < 12) {
        if (unit_sum >= 12 && unit_sum <= 14) {
            cout << "OK!" << endl;
        } else if (unit_sum > 14) {
            cout << "Maximum Units Validation Failed!" << endl;
        } else if (unit_sum < 12) {
            cout << "Minimum Units Validation Failed!" << endl;
        }
    } else {
        if (unit_sum >= 12 && unit_sum <= 20) {
            cout << "OK!" << endl;
        } else if (unit_sum > 20) {
            cout << "Maximum Units Validation Failed!" << endl;
        } else if (unit_sum < 12) {
            cout << "Minimum Units Validation Failed!" << endl;
        }
    }
    return 0;

}

int test(vector <string>& third_name, vector <string>& second_subject_name, vector <float>& second_subject_score, int& n, int& k)
{
    for (int i = 0; i < k; i++)
    {
        bool test = false;
        for (int j = 0; j < n; j++)
        {
            if (third_name[i] == second_subject_name[j] && second_subject_score[j] >= 10)
            {
                test = true;
                break;
            }
        }
        if (test)
        {
            cout << "Course Already Taken!" << endl;
            return 1; 
        } 
    }
    return 0;
}

int main() {
    vector<string> first_subject_name;
    vector<int> first_subject_unit;
    vector<string> first_subject_prerequisite;

    vector<string> second_subject_name;
    vector<int> second_subject_unit;
    vector<float> second_subject_score;

    vector<string> third_name;

    int m, n, k;

    cin >> m;
    course_Present(first_subject_name, first_subject_unit, first_subject_prerequisite, m);

    cin >> n;
    course_Taken(second_subject_name, second_subject_unit, second_subject_score, n);

    cin >> k;
    choosing_Course(third_name, k);

    double gpa = calculate_GPA(second_subject_unit, second_subject_score, n);

    if(test(third_name,second_subject_name,second_subject_score,n,k)== 1)
    {
        return 1;
    }

    if (offer(first_subject_name, third_name, k, m) == 1) 
    {
        return 1; 
    }

    if(prerequisite(n,m,k,third_name,first_subject_name,first_subject_prerequisite,second_subject_name,second_subject_score)==1)
    {
        return 1;
        
    }

    if(unit_check(gpa, first_subject_name, third_name, first_subject_unit, m, k) == 1)
    {
        return 1;
    }

    return 0;
}
