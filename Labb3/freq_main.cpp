#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <iomanip>
#include "BinarySearchTree.h"

using namespace std;

class Row {
public:
    string word;
    int frequency;

    Row(string s, int f) : word{s}, frequency{f} {}

    bool operator<(const Row & rhs) const
    {
        return word < rhs.word;
    }
};

ostream & operator<<(ostream & out, const Row row)
{
    out << right << setw(15) << row.word << " " << row.frequency;
    return out;
}

int main( )
{
    BinarySearchTree<Row> T;
    ifstream file("words.txt");

    if (!file)
    {
        cout << "couldn't open file words.txt" << endl;
        return 1;
    }

    vector<string> V1 = { istream_iterator<string>{file}, istream_iterator<string>{} };
    file.close();

    for(auto j: V1) {
        Row row(j, 1);
        auto tmp = T.contains(row);
        if (tmp != T.end()) {
            // word already in frequenct table
            tmp->frequency++;
        } else {
            T.insert( row );
        }
    }

    // Print frequency table
    for(auto r : T) {
        cout << r << endl;
    }

    return 0;
}

