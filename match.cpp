#include "match.h"

#include <fstream>
#include <iostream>
#include <sstream>

Match::Match()
{

}

Match::~Match()
{

}

void Match::init()
{

}

void Match::writeCode(string dir, string file, std::vector<char> code)
{
    string filename = dir + ".iris";
    ofstream out(filename,ios::app|ios::binary);
    if(out.is_open())
    {
        out << dir << " " << file << " ";
        for(int i = 0;i < code.size();i++)
            out << code[i];
        out << '\n';
        out.close();
    }
    else
        cout << "write failed!"<<endl;
}

string Match::matchCode(vector<char> code)
{
    for(int i = 0;i < irisCodes.size();i++)
    {
        double count = 0;
        for(int j = 0;j < code.size();j++)
        {
            if(code[j] != irisCodes[i][j])
                count++;
        }
        double val = count / code.size();
        cout<<val<<endl;
        matchValues.push_back(val);
        if(val < matchValue){
            matchValue = val;
            message = ids[i] + " " + pictures[i];
        }
    }
    cout<<"best match:"<<message<<endl;
    return message;
}

void Match::loadCodes(string codePath)
{
    ifstream in(codePath,ios::in|ios::binary);
    string line,id,pic,tempcode;
    if(in.is_open())
    {
        while(getline(in,line))
        {
            vector<char> ch;
            istringstream iss(line);
            iss >> id;
            iss >> pic;
            iss >> tempcode;
            cout<<id<<" "<<pic<<" "<<tempcode.length()<<endl;
            for(int i = 0;i < tempcode.length();i++)
                ch.push_back(tempcode[i]);
            ids.push_back(id);
            pictures.push_back(pic);
            irisCodes.push_back(ch);
            ch.clear();
        }
        in.close();
    }
    else
        cout << "load failed!" <<endl;
}

void Match::clearMemorry()
{
    ids.clear();
    pictures.clear();
    irisCodes.clear();
    matchValues.clear();
}
