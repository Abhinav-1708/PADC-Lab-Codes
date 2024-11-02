#include <iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int> &temp)
{
    for (int i = 0; i < temp.size() - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < temp.size() - i - 1; j++)
        {
            if (temp[j] > temp[j + 1])
            {
                swap(temp[j], temp[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}
int main()
{
    vector<int> temp = {19, 2, 72, 3, 18, 57, 603, 490, 45, 101};
    bubbleSort(temp);

    for (int x : temp)
    {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}