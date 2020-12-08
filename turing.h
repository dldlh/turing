#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<istream>
#include<algorithm>
#include<regex>
#include<unordered_set>
#include<unordered_map>
using namespace std;

class Turing
{
public:
    class delta{
        public:
        string nowstate;
        vector<char>nowsymbols;
        vector<char>outsymbols;
        vector<char>directions;
        string nextstate;
        delta();
        delta(const delta& obj);
    };
    Turing(string tm , int m);
    void run(string input);
    ~Turing();

private:
    int model;
    unordered_set<string>Q;
    unordered_set<char>S;
    unordered_set<char>G;
    string start;
    char B = 0;
    unordered_set<string>F;
    int N = 0;
    vector<delta>move;
    void verbose(int step, vector<vector<int>>&realindex, vector<int>&index, string nowstate,vector<vector<char>>&tape);
    void analysistm(string line);
    bool analysis_state(string line);
    bool analysis_input(string line);
    bool analysis_G(string line);
    bool analysis_q0(string line);
    bool analysis_B(string line);
    bool analysis_finalstate(string line);
    bool analysis_N(string line);
    bool analysis_delta(string line);
    void analysiserror(int errorcode,string line);
    void checkerror(int errorcode);
    void checksyntax();
    void printtape(vector<char>tape);
    bool checkinput(string input);
};

