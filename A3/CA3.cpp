#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath> 

using namespace std;

// --- Structs ---
struct Person {
    string name;
    double netBalance = 0.0;
};

struct Transaction {
    string debtor;
    string creditor;
    double amount;
};

// --- Function Prototypes ---
vector<string> split(const string& s, char delimiter);
string trim(const string& str);
pair<string, double> parseNameAndAmount(string token);
int findPersonIndex(const vector<Person>& people, string name);

void readUsers(string filePath, vector<Person>& people);
void processExpenseLine(string line, vector<Person>& people);
void processExpenses(string filePath, vector<Person>& people);

void calculateEqually(vector<Person>& people, string payersStr, string borrowersStr);
void calculateUnequally(vector<Person>& people, string payersStr, string borrowersStr);
void calculateAdjustment(vector<Person>& people, string payersStr, string borrowersStr);

vector<Transaction> optimizeTransactions(vector<Person>& people);
void printTransactions(const vector<Transaction>& transactions);

// --- Main Function ---
int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    vector<Person> people;
    readUsers(argv[1], people);
    processExpenses(argv[2], people);

    vector<Transaction> transactions = optimizeTransactions(people);
    printTransactions(transactions);

    return 0;
}

// --- Helper Implementations ---

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    if (!s.empty() && s.back() == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

pair<string, double> parseNameAndAmount(string token) {
    vector<string> details = split(token, ':');
    string name = trim(details[0]);  
    double amount = 0.0;
    if (details.size() > 1) {
        amount = stod(details[1]);
    }
    return {name, amount};
}

int findPersonIndex(const vector<Person>& people, string name) {
    string trimmedName = trim(name);
    for (size_t i = 0; i < people.size(); ++i) {
        if (people[i].name == trimmedName) {
            return i;
        }
    }
    return -1;
}

// --- Logic Implementations ---

void calculateEqually(vector<Person>& people, string payersStr, string borrowersStr) {
    vector<string> payerTokens = split(payersStr, ';');
    double totalCost = 0.0;
    
    for (const string& token : payerTokens) {
        pair<string, double> p = parseNameAndAmount(token);
        int idx = findPersonIndex(people, p.first);
        if (idx != -1) {
            people[idx].netBalance += p.second;
            totalCost += p.second;
        }
    }

    vector<string> borrowerNames;
    if (trim(borrowersStr).empty()) {
        for (const auto& person : people) borrowerNames.push_back(person.name);
    } else {
        vector<string> tokens = split(borrowersStr, ';');
        for (const string& token : tokens) {
            pair<string, double> p = parseNameAndAmount(token);
            borrowerNames.push_back(p.first);
        }
    }

    int n = borrowerNames.size();
    if (n == 0) return;

    double rawShare = totalCost / n;
    double share = ((long long)(rawShare * 100)) / 100.0;
    
    double totalDistributed = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        int idx = findPersonIndex(people, borrowerNames[i]);
        if (idx != -1) {
            people[idx].netBalance -= share;
            totalDistributed += share;
        }
    }
    
    double remaining = totalCost - totalDistributed;
    int lastIdx = findPersonIndex(people, borrowerNames.back());
    if (lastIdx != -1) {
        people[lastIdx].netBalance -= remaining;
    }
}

void calculateUnequally(vector<Person>& people, string payersStr, string borrowersStr) {
    vector<string> payerTokens = split(payersStr, ';');
    for (const string& token : payerTokens) {
        pair<string, double> p = parseNameAndAmount(token);
        int idx = findPersonIndex(people, p.first);
        if (idx != -1) people[idx].netBalance += p.second;
    }

    vector<string> borrowerTokens = split(borrowersStr, ';');
    for (const string& token : borrowerTokens) {
        pair<string, double> p = parseNameAndAmount(token);
        int idx = findPersonIndex(people, p.first);
        if (idx != -1) people[idx].netBalance -= p.second;
    }
}

void calculateAdjustment(vector<Person>& people, string payersStr, string borrowersStr) {
    double totalCost = 0.0;
    vector<string> payerTokens = split(payersStr, ';');
    for (const string& token : payerTokens) {
        pair<string, double> p = parseNameAndAmount(token);
        int idx = findPersonIndex(people, p.first);
        if (idx != -1) {
            people[idx].netBalance += p.second;
            totalCost += p.second;
        }
    }

    double totalExtra = 0.0;
    vector<string> borrowerTokens = split(borrowersStr, ';');
    for (const string& token : borrowerTokens) {
        pair<string, double> p = parseNameAndAmount(token);
        int idx = findPersonIndex(people, p.first);
        if (idx != -1) {
            people[idx].netBalance -= p.second;
            totalExtra += p.second;
        }
    }

    double remainingCost = totalCost - totalExtra;
    int n = people.size();
    if (n == 0) return;

    double rawBaseShare = remainingCost / n;
    double baseShare = ((long long)(rawBaseShare * 100)) / 100.0;

    double totalDistributed = 0.0;
    for (int i = 0; i < n - 1; ++i) {
        people[i].netBalance -= baseShare;
        totalDistributed += baseShare;
    }

    double finalRemains = remainingCost - totalDistributed;
    people[n - 1].netBalance -= finalRemains;
}

void processExpenseLine(string line, vector<Person>& people) {
    vector<string> parts = split(line, ',');
    if (parts.size() < 2) return; 

    string type = trim(parts[0]);
    string payersStr = parts[1];
    string borrowersStr = (parts.size() > 2) ? parts[2] : ""; 

    if (type == "equally") calculateEqually(people, payersStr, borrowersStr);
    else if (type == "unequally") calculateUnequally(people, payersStr, borrowersStr);
    else if (type == "adjustment") calculateAdjustment(people, payersStr, borrowersStr);
}

void readUsers(string filePath, vector<Person>& people) {
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    if (file.good()) getline(file, line); 

    while (getline(file, line)) {
        string name = trim(line);
        if (!name.empty()) {
            Person p; p.name = name; p.netBalance = 0.0;
            people.push_back(p);
        }
    }
    file.close();
}

void processExpenses(string filePath, vector<Person>& people) {
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    if (file.good()) getline(file, line);

    while (getline(file, line)) {
        string cleanLine = trim(line);
        if (!cleanLine.empty()) processExpenseLine(cleanLine, people);
    }
    file.close();
}

// --- Optimization & Output ---

bool compareDebtors(const Person& a, const Person& b) {
    if (abs(a.netBalance) != abs(b.netBalance)) return abs(a.netBalance) > abs(b.netBalance);
    return a.name < b.name;
}

bool compareCreditors(const Person& a, const Person& b) {
    if (a.netBalance != b.netBalance) return a.netBalance > b.netBalance;
    return a.name < b.name;
}

vector<Transaction> optimizeTransactions(vector<Person>& people) {
    vector<Person> debtors, creditors;
    vector<Transaction> transactions;

    for (const auto& p : people) {
        if (p.netBalance < -0.01) debtors.push_back(p);
        else if (p.netBalance > 0.01) creditors.push_back(p);
    }

    sort(debtors.begin(), debtors.end(), compareDebtors);
    sort(creditors.begin(), creditors.end(), compareCreditors);

    size_t i = 0, j = 0;
    while (i < debtors.size() && j < creditors.size()) {
        Person& debtor = debtors[i];
        Person& creditor = creditors[j];

        double amount = min(abs(debtor.netBalance), creditor.netBalance);
        amount = ((long long)(amount * 100 + 0.5)) / 100.0; 

        if (amount > 0) transactions.push_back({debtor.name, creditor.name, amount});

        debtor.netBalance += amount;
        creditor.netBalance -= amount;

        if (abs(debtor.netBalance) < 0.01) i++;
        if (creditor.netBalance < 0.01) j++;
    }
    return transactions;
}

bool compareTransactions(const Transaction& a, const Transaction& b) {
    if (a.amount != b.amount) return a.amount > b.amount;
    if (a.debtor != b.debtor) return a.debtor < b.debtor;
    return a.creditor < b.creditor;
}

void printTransactions(const vector<Transaction>& transactions) {
    vector<Transaction> sortedTrans = transactions;
    sort(sortedTrans.begin(), sortedTrans.end(), compareTransactions);

    for (const auto& t : sortedTrans) {
        cout << t.debtor << " -> " << t.creditor << ": " << fixed << setprecision(2) << t.amount << endl;
    }
}