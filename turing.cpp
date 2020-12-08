#include "turing.h"

Turing::Turing(string tm, int m)
{
    model = m;
    analysistm(tm);
    checksyntax();
}

Turing::~Turing()
{
}

void Turing::checkerror(int errorcode)
{
    if (model == 1)
    {
        switch (errorcode)
        {
        case 0:
            cerr << "未定义状态集 Q" << endl;
            break;
        case 1:
            cerr << "未定义输入符号集 S" << endl;
            break;
        case 2:
            cerr << "未定义纸带符号集 G" << endl;
            break;
        case 3:
            cerr << "未定义初始状态 q0" << endl;
            break;
        case 4:
            cerr << "非法初始状态 q0 : " << start << endl;
            break; 
        case 5:
            cerr << "纸带条数N必须大于0" << endl;
            break;
        case 6:
            cerr << "输入符号集中含有不在纸带符号集中的符号" << endl;
            break;
        case 7:
            cerr << "含有" << endl;
            break;
        }
        
    }
    else
    {
        cerr << "syntax error" << endl;
    }
    exit(-1);
}

void Turing::checksyntax()
{
    if (Q.empty())
    {
        checkerror(0);
    }
    if (S.empty())
    {
        checkerror(1);
    }
    if (G.empty())
    {
        checkerror(2);
    }
    if (start.empty())
    {
        checkerror(3);
    }
    else if (Q.count(start) == 0)
    {
        checkerror(4);
    }
    if (N <= 0)
    {
        checkerror(5);
    }
    for (auto s : S)
    {
        if (G.count(s) == 0)
        {
            checkerror(6);
        }
    }
    for (auto f : F)
    {
        if (Q.count(f) == 0)
        {
            checkerror(6);
        }
    }
    for (auto m : move)
    {
        if (Q.count(m.nowstate) == 0)
        {
            checkerror(7);
        }
        if (Q.count(m.nextstate) == 0)
        {
            checkerror(8);
        }
        if (m.directions.size() != N || m.nowsymbols.size() != N || m.outsymbols.size() != N)
        {
            checkerror(9);
        }
        for (auto inputsymbol : m.nowsymbols)
        {
            if (G.count(inputsymbol) == 0)
            {
                checkerror(10);
            }
        }
        for (auto outputsymbol : m.outsymbols)
        {
            if (G.count(outputsymbol) == 0)
            {
                checkerror(11);
            }
        }
    }
    for (size_t i = 0; i < move.size(); i++)
    {
        for (size_t j = i + 1; j < move.size(); j++)
        {
            if (move[i].nowstate == move[j].nowstate && move[i].nowsymbols == move[j].nowsymbols)
            {
                if (move[i].directions != move[j].directions || move[i].nextstate != move[j].nextstate || move[i].outsymbols != move[j].outsymbols)
                {
                    checkerror(12);
                }
            }
        }
    }
}

void Turing::analysistm(string tm)
{
    ifstream fin(tm);
    if (!fin.is_open())
    {
        cerr << "Could not open file" << tm << endl;
        exit(-1);
    }
    int linecnt = 0;
    while (!fin.eof())
    {
        string line;
        getline(fin, line);
        linecnt++;
        int semposition = line.find(';');
        if (semposition != line.npos)
        {
            line = string(line.begin(), line.begin() + semposition);
        }
        if (line == "")
        {
            continue;
        }
        if (
            analysis_state(line) ||
            analysis_input(line) ||
            analysis_G(line) ||
            analysis_q0(line) ||
            analysis_B(line) ||
            analysis_finalstate(line) ||
            analysis_N(line) ||
            analysis_delta(line))
        {
            continue;
        }
        analysiserror(0, line);
    }
    fin.close();
}

void Turing::analysiserror(int errorcode, string line)
{
    if (model == 1)
    {
        switch (errorcode)
        {
        case 0:
            cerr << "" << endl;
            break;
        case 1:
            cerr << "" << endl;
            break;
        }
    }
    else
    {
        cerr << "syntax error" << endl;
    }
    exit(-1);
}

bool Turing::analysis_state(string line)
{
    string patternline = "#Q\\s=\\s\\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (!Q.empty())
        {
            analysiserror(1, line);
        }
        string statesline(line.begin() + 6, line.end() - 1);
        regex split(",");
        regex_token_iterator<string::iterator> rend;
        regex_token_iterator<string::iterator> state(statesline.begin(), statesline.end(), split, -1);
        while (state != rend)
        {
            string s = *state;
            if (Q.count(s) == 0)
            {
                Q.insert(s);
            }
            else
            {
                analysiserror(12, line);
            }
            state++;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_input(string line)
{
    string patternline = "#S\\s=\\s\\{.(,.)*\\}(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (!S.empty())
        {
            analysiserror(2, line);
        }
        string inputline(line.begin() + 6, line.end() - 1);
        regex split(",");
        regex_token_iterator<string::iterator> rend;
        regex_token_iterator<string::iterator> input_symbol(inputline.begin(), inputline.end(), split, -1);
        while (input_symbol != rend)
        {
            string s = *input_symbol;
            if (s[0] >= 32 && s[0] <= 126 && s[0] != '_' && s[0] != ' ' && s[0] != ',' && s[0] != '*' && s[0] != ';' && s[0] != '{' && s[0] != '}')
            {
                if (S.count(s[0]) == 0)
                {
                    S.insert(s[0]);
                    input_symbol++;
                }
                else
                {
                    analysiserror(4, line);
                }
            }
            else
            {
                analysiserror(3, line);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_G(string line)
{
    string patternline = "#G\\s=\\s\\{.(,.)*\\}(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (!G.empty())
        {
            analysiserror(5, line);
        }
        string outputline(line.begin() + 6, line.end() - 1);
        regex split(",");
        regex_token_iterator<string::iterator> rend;
        regex_token_iterator<string::iterator> output_symbol(outputline.begin(), outputline.end(), split, -1);
        while (output_symbol != rend)
        {
            string s = *output_symbol;
            if (s[0] >= 32 && s[0] <= 126 && s[0] != ' ' && s[0] != ',' && s[0] != '*' && s[0] != ';')
            {
                if (G.count(s[0]) == 0)
                {
                    G.insert(s[0]);
                    output_symbol++;
                }
                else
                {
                    analysiserror(7, line);
                }
            }
            else
            {
                analysiserror(6, line);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_q0(string line)
{
    string patternline = "#q0\\s=\\s\\w+(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (start != "")
        {
            analysiserror(8, line);
        }
        start = string(line.begin() + 6, line.end());
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_B(string line)
{
    string patternline = "#B\\s=\\s_(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (B != 0)
        {
            analysiserror(9, line);
        }
        B = '_';
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_finalstate(string line)
{
    string patternline = "#F\\s=\\s\\{[a-zA-Z0-9_]+(,[a-zA-Z0-9_]+)*\\}(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (!F.empty())
        {
            analysiserror(10, line);
        }
        string fsline(line.begin() + 6, line.end() - 1);
        regex split(",");
        regex_token_iterator<string::iterator> rend;
        regex_token_iterator<string::iterator> state(fsline.begin(), fsline.end(), split, -1);
        while (state != rend)
        {
            string s = *state;
            if (F.count(s) == 0)
            {
                F.insert(s);
            }
            else
            {
                analysiserror(11, line);
            }
            state++;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_N(string line)
{
    string patternline = "#N\\s=\\s[0-9]+(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        if (N != 0)
        {
            analysiserror(13, line);
        }
        string num(line.begin() + 5, line.end());
        N = stoi(num);
        return true;
    }
    else
    {
        return false;
    }
}

bool Turing::analysis_delta(string line)
{
    string patternline = "\\w+\\s\\w+\\s\\w+\\s[lr\\*]+\\s\\w+(\\s)*";
    regex pattern(patternline);
    if (regex_match(line, pattern))
    {
        regex split(" ");
        regex_token_iterator<string::iterator> rend;
        regex_token_iterator<string::iterator> item(line.begin(), line.end(), split, -1);
        delta temp;
        string s = *item;
        temp.nowstate = s;
        item++;
        s = *item;
        for (size_t i = 0; i < s.size(); i++)
        {
            temp.nowsymbols.push_back(s[i]);
        }
        item++;
        s = *item;
        for (size_t i = 0; i < s.size(); i++)
        {
            temp.outsymbols.push_back(s[i]);
        }
        item++;
        s = *item;
        for (size_t i = 0; i < s.size(); i++)
        {
            temp.directions.push_back(s[i]);
        }
        item++;
        s = *item;
        temp.nextstate = s;
        move.push_back(temp);
        return true;
    }
    else
    {
        return false;
    }
}

Turing::delta::delta()
{
    nowstate = "";
    nextstate = "";
}

Turing::delta::delta(const delta &obj)
{
    nowstate = obj.nowstate;
    nowsymbols = obj.nowsymbols;
    directions = obj.directions;
    nextstate = obj.nextstate;
    outsymbols = obj.outsymbols;
}

void Turing::run(string input)
{
    checkinput(input);
    vector<vector<char>> tapes(N);
    vector<vector<int>> realindex(N);
    string nowstate = start;
    int temp = 0;
    for (auto s : input)
    {
        tapes[0].push_back(s);
        realindex[0].push_back(temp++);
    }
    for (size_t i = 0; i < N; i++)
    {
        if (tapes[i].empty())
        {
            tapes[i].push_back('_');
            realindex[i].push_back(0);
        }
    }
    vector<int> index(N, 0);

    int step = 0;
    while (true)
    {
        if (model == 1)
        {
            verbose(step, realindex, index, nowstate, tapes);
        }
        if (F.count(nowstate) == 1)
        {
            printtape(tapes[0]);
            break;
        }
        bool transition = false;
        for (auto m : move)
        {
            if (m.nowstate == nowstate)
            {
                bool match = true;
                for (size_t i = 0; i < N; i++)
                {
                    if (tapes[i][index[i]] != m.nowsymbols[i])
                    {
                        match = false;
                        break;
                    }
                }
                if (!match)
                {
                    continue;
                }
                step++;
                transition = true;
                for (size_t i = 0; i < N; i++)
                {
                    tapes[i][index[i]] = m.outsymbols[i];
                    if (m.directions[i] == '*')
                    {
                        continue;
                    }
                    if (index[i] == 0 && m.directions[i] == 'l')
                    {
                        tapes[i].insert(tapes[i].begin(), '_');
                        realindex[i].insert(realindex[i].begin(), (*realindex[i].begin()) + 1);
                    }
                    else if (index[i] == (int)tapes[i].size() - 1 && m.directions[i] == 'r')
                    {
                        tapes[i].push_back('_');
                        realindex[i].push_back(realindex[i][realindex[i].size() - 1] + 1);
                        index[i]++;
                    }
                    else
                    {
                        if (m.directions[i] == 'l')
                        {
                            index[i]--;
                        }
                        if (m.directions[i] == 'r')
                        {
                            index[i]++;
                        }
                    }
                }
                nowstate = m.nextstate;
                break;
            }
        }
        if (!transition)
        {
            printtape(tapes[0]);
            break;
        }
    }
}

void Turing::verbose(int step, vector<vector<int>> &realindex, vector<int> &index, string nowstate, vector<vector<char>> &tape)
{
    cout << "Step   : " << step << endl;
    for (size_t i = 0; i < N; i++)
    {
        cout << "Index" << i << " : ";
        int beginindex = 0, endindex = tape[i].size() - 1;
        while (beginindex < tape[i].size() && tape[i][beginindex] == '_')
        {
            beginindex++;
        }
        if (beginindex == tape[i].size())
        {
            cout << realindex[i][index[i]] << endl;
            cout << "Tape" << i << "  : "
                 << "_" << endl;
            cout << "Head" << i << "  : "
                 << "^" << endl;
        }
        else
        {
            while (endindex >= 0 && tape[i][endindex] == '_')
            {
                endindex--;
            }
            beginindex = min(beginindex, index[i]);
            endindex = max(endindex, index[i]);
            for (size_t j = beginindex; j <= endindex; j++)
            {
                cout << realindex[i][j] << " ";
            }
            cout << endl;
            cout << "Tape" << i << "  : ";
            for (size_t j = beginindex; j <= endindex; j++)
            {
                cout << tape[i][j] << " ";
                size_t blanknum = to_string(realindex[i][j]).size() - 1;
                for (size_t k = 0; k < blanknum; k++)
                {
                    cout << " ";
                }
            }
            cout << endl;
            cout << "Head" << i << "  : ";
            for (size_t j = beginindex; j < index[i]; j++)
            {
                size_t blanknum = to_string(realindex[i][j]).size();
                for (size_t k = 0; k < blanknum; k++)
                {
                    cout << " ";
                }
                cout << " ";
            }
            cout << "^" << endl;
        }
    }
    cout << "State  : " << nowstate << endl;
    cout << "---------------------------------------------" << endl;
}

bool Turing::checkinput(string input)
{
    for (size_t i = 0; i < input.size(); i++)
    {
        if (S.count(input[i]) == 0)
        {
            if (model == 0)
            {
                cerr << "illegal input" << endl;
                exit(-1);
            }
            else
            {
                cerr << "==================== ERR ====================" << endl;
                cerr << "error:\'" << input[i] << "\' was not declared in the set of input symbols" << endl;
                cerr << "Input: " << input << endl;
                string blank(7 + i, ' ');
                blank += "^";
                cerr << blank << endl;
                cerr << "==================== END ====================" << endl;
                exit(-1);
            }
        }
    }
    if (model == 1)
    {
        cout << "Input : " << input << endl;
        cout << "==================== RUN ====================" << endl;
    }
    return true;
}

void Turing::printtape(vector<char> tape)
{
    size_t length = tape.size();
    int begin = 0, end = length - 1;
    for (; begin < length; begin++)
    {
        if (tape[begin] != '_')
        {
            break;
        }
    }
    for (; end >= 0; end--)
    {
        if (tape[end] != '_')
        {
            break;
        }
    }
    if (model == 0)
    {
        for (int i = begin; i <= end; i++)
        {
            cout << tape[i];
        }
        cout << endl;
    }
    else
    {
        cout << "Result  : ";
        for (int i = begin; i <= end; i++)
        {
            cout << tape[i];
        }
        cout << endl;
        cout << "==================== END ====================" << endl;
    }
}