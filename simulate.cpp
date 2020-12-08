#include<iostream>
#include<vector>
#include<cstdio>
#include<string>
#include<cstdlib>
#include"turing.h"
using namespace std;

int help(){
    cout << "turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
    cout << "turing 接收两个参数，tm和input，其中tm为图灵机程序，文件名为XXXX.tm，input为图灵机输入" <<endl;
    cout << "图灵机程序具体语法参考https://ptc-nju.github.io/" << endl;
    cout << "指定[-v|--verbose]时为verbose模式，在该模式下会对图灵机程序中的语法详细错误报错以及输出每一步的运行过程" << endl;
    return 0;
}

int simulate(string tm,string str , int model){
    Turing turing(tm, model);
    turing.run(str);
    return 0;
}


int main(int argc,char**argv){
    if(argc == 2){
        string s = argv[1];
        if(s == "-h" || s == "--help"){
            return help();
        }                                                                                                                            
    }
    else if(argc == 3){
        string tm = argv[1];
        if(tm =="-h" || tm =="--help" || tm =="-v" || tm == "--verbose"){
            cerr << "turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
            exit(-1);
        }
        string str = argv[2];
        size_t tmlength = tm.size();
        if(tmlength <= 3 || tm[tmlength-1] != 'm'|| tm[tmlength-2] != 't'|| tm[tmlength-3] != '.'){
            cerr << "Invalid tm \"" << tm <<"\", should be of form xxx.tm"<<endl;
            exit(-1);
        }
        return simulate(tm, str, 0);
    }
    else if(argc == 4){
        string s = argv[1];
        if(s == "-v"||s == "--verbose"){
            string tm = argv[2];
            string input = argv[3];
            size_t tmlength = tm.size();
            if(tmlength <= 3 || tm[tmlength-1] != 'm'|| tm[tmlength-2] != 't'|| tm[tmlength-3] != '.'){
                cerr << "Invalid tm \"" << tm <<"\", should be of form xxx.tm" << endl;
                exit(-1);
            }
            return simulate(tm,input,1);
        }

    }
    cerr << "turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
    exit(-1);
}