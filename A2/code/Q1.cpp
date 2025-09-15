#include <bits/stdc++.h>

using namespace std;

long long int Checkdigit(long long int n)
{
    return floor(log10(n) + 1);
}
long long int MakeMin(long long int &n)
{
    long long int deflow, defup, test = 1, low, up, examp, sign;
    examp = abs(n);
    long long int temp = Checkdigit(examp);
    if (n < 0)
    {
        if (temp != 1)
        {
            for (long long int i = 1; i < temp; i++)
            {
                test = (test * 10) + 1;
                low = test;
            }
        }
        else
        {
            low = test;
        }

        up = (low * 10) + 1;  

        deflow = abs(low - n);
        defup = abs(up - n);
        if (deflow > defup)
        {
            n = abs(n + up);
            return temp + 1;
        }
        else
        {
            n = abs(n + low);
            return temp;
        }
    }
    else
    {
        if (temp != 1)
        {
            for (long long int i = 1; i < temp; i++)
            {
                test = (test * 10) + 1;
                low = test;
            }
             
        }
        else
        {
            low = test;
        }
        up = (low * 10) + 1;  
        deflow = abs(low - n);
        defup = abs(up - n);
        if (deflow > defup)
        {
            n = abs(n - up);
            return temp + 1;
        }
        else
        {
            n = abs(n - low);
            return temp;
        }
    }
}
long long int counter(long long int &n, long long int count)
{
    if (n == 0)
        return count;
    else
    {
        long long int test = count + MakeMin(n);
        return counter(n, count) + test;
    }
}
 int main()
{
    long long int n, count = 0;
    cin >> n;
    count = counter(n, count);
    cout << count  << endl;
}