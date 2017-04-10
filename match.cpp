#include "match.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <io.h>
#include <stdio.h>

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

void Match::writeCodeAll(string dir, string file, vector<char> code)
{
    string filename = "template.iris";
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
    string result = "match_result.txt";
    if(access(result.c_str(),F_OK) == 0)
        remove(result.c_str());
    ofstream out(result,ios::app|ios::binary);
    double matchValue = 1.0;
    if(out.is_open())
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
            out<<ids[i]<<" "<<pictures[i]<<"    match value:  "<<val<<'\n';
            matchValues.push_back(val);
            if(val < matchValue){
                matchValue = val;
                stringstream ssm;
                ssm << val;
                string valstr = ssm.str();
                message = ids[i] + " " + pictures[i] + " " + valstr;
            }
        }
        out<<"best match:"<<message<<'\n';
        out.close();
        return message;
    }
    else
        cout<<"match result write failed!"<<endl;

}

bool Match::batchMatch(string path,string target,vector<char> sampleCode)
{
    string isMatch = "false";
    ofstream out(path,ios::app|ios::binary);
    if(out.is_open())
    {
        double bestMatch = 1.0;
        string matchstr;
        for(int i = 0;i < irisCodes.size();i++)
        {
            double count = 1.0;
            for(int j = 0;j < sampleCode.size();j++)
                if(sampleCode[j] != irisCodes[i][j])
                    count++;
            double val = count / sampleCode.size();
            if(val < bestMatch)
            {
                bestMatch = val;
                stringstream ssm;
                ssm << val;
                string valstr = ssm.str();
                if(target == ids[i])
                    isMatch = "true";
                matchstr = "target:" + target + " match:" + ids[i] + " matchValue:" + valstr + " isMatch:" + isMatch;
            }
        }
        out << matchstr << '\n';
        out.close();

    }
    else
        cout<<"failed !!!"<<endl;
    if(isMatch == "true")
        return true;
    else
        return false;
}

void Match::loadCodes(string codePath)
{
    ifstream in(codePath,ios::in|ios::binary);
    string line,id,pic,tempcode;
    int count = 0;
    if(in.is_open())
    {
        while(getline(in,line))
        {
            vector<char> ch;
            istringstream iss(line);
            iss >> id;
            iss >> pic;
            iss >> tempcode;
            for(int i = 0;i < tempcode.length();i++)
                ch.push_back(tempcode[i]);
            ids.push_back(id);
            pictures.push_back(pic);
            irisCodes.push_back(ch);
            ch.clear();
            count++;
        }
        in.close();
        cout<<"total numbers:"<<count<<endl;
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
