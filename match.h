#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <string>

using namespace std;

class Match
{
public:
    Match();
    ~Match();

    void init();



    vector<string> ids;
    vector<string> pictures;
    vector<vector<char> > irisCodes;

    string message;
    double matchValue = 1.0;
    vector<double> matchValues;

    void writeCode(string dir,string file,vector<char> code);
    string matchCode(vector<char> code);
    void loadCodes(string codePath);
    void clearMemorry();
};

#endif // MATCH_H
