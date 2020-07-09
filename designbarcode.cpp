//
//  main.cpp
//  cellbarcode
//
//  Created by chentao on 2020/7/8.
//  Copyright © 2020 BioLab. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <getopt.h>
#include <fstream>

using std::string;
using std::vector;
using std::map;

/*
string randomSequence (int, string);
bool filters (const string);
vector<double> probs (map<string, double> prob, const string filters[], int index, int length);
 */

string randomSequence (int num, string nucleacid)
{
    string positionAcid;
    for (int j = 0; j < num; j++)
    {
        //srand((unsigned)time(NULL));
        int randomN = rand() % 4;
        positionAcid += nucleacid[randomN];
    }
    return positionAcid;
}


bool filters (const string sequences)
{
    int range = (int) sequences.length() / 3;
    for (int i = 1; i <= range; i++)
    {
        for (int j = 0; j < sequences.length() - 2*i; j++)
        {
            //std::cout << sequences.substr(j, i) << std::endl;
            bool test = sequences.substr(j, i) == sequences.substr(j+ 1*i,i) && sequences.substr(j, i) ==  sequences.substr(j + 2*i, i);
            if (test) return false;
        }
    }
    return true;
}

vector<double> probs (map<string, double> prob , const string filters[], int index, int length)
{
    vector< vector<double> > results(index,vector<double>(length, 0));
    vector<double> result(index, 0);
    for (int i = 0; i < length; i ++)
    {
        prob["A"] = 0; prob["G"] = 0; prob["C"] = 0; prob["T"] = 0;
        for (int j = 0; j < index; j++)
        {
            prob[filters[j].substr(i, 1)] += 1.0;
        }
        
        for (int j = 0; j < index; j++)
        {
            results[j][i] = abs(prob[filters[j].substr(i, 1)] / index - 0.25);
        }
    }
    
    for (int j = 0; j < index; j++)
    {
        for (int i = 0; i < results[j].size(); i++)
            result[j] += results[j][i];
    }
    return result;
}


struct param
{
    int length;
    int number;
    string preName;
} args;

const struct option loptions[] =
{
    {"help", no_argument, NULL, 'h'},
    {"length", required_argument, NULL, 'l'},
    {"number", required_argument, NULL, 'n'},
    {"preName", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}
};

int print_usage(void)
{
    std::cerr << std::endl;
    std::cerr << "About:        Generate sequence of length l and quantity n \n";
    std::cerr << "Usage:        Sequence [options] \n";
    std::cerr << "              -l, the length of sequence \n";
    std::cerr << "              -n, the number of sequence \n";
    std::cerr << "              -p, output filename prefix \n";
    return 1;
}


int main(int argc, char * argv[])
{
    int opt;
    while ((opt = getopt_long(argc, argv, "hl:n:p:", loptions, NULL)) > 0)
    {
        switch (opt)
        {
            case 'h': return print_usage();
            case 'l': args.length = atoi(optarg); break;
            case 'n': args.number = atoi(optarg); break;
            case 'p': args.preName = optarg; break;
            default: return print_usage();
        }
    }
    
    if (optind != 7) return print_usage();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << args.length << std::endl;
    std::cout << args.number << std::endl;
    
    std::cout << "----------------------------------------" << std::endl;
    
    
    int length = args.length;
    int number = args.number;
    
    string sequenceFile = "./" + args.preName + ".sequence.txt";
    string differentSummary = "./" + args.preName + ".diff.summary.txt";
    string siteFrequence = "./" + args.preName + ".siteFrequence.txt";
    
    string nucleacid = "ATGC";
    map<string, double> prob;
    vector<double> prob_vectors;
    
    
    string * presequence = new string[100*number];
    string * filter = new string[100*number];
    

    string poSequence;
    int index = 0;
    
    for (int i = 0; i < 10*number; i++)
    {
        poSequence = randomSequence(100 * number, nucleacid);
        for (int j = 0; j < length; j++)
        {
            presequence[i] += poSequence[j];
        }
        
        if (filters(presequence[i]))
        {
            filter[index] = presequence[i];
            index += 1;
        }
        
    }
    
    string filters[index];
    for (int i = 0; i < index; i++)
        filters[i] = filter[i];
    delete [] filter;
    
    
    prob_vectors = probs(prob, filters, index, length);
    
    vector<double> prob_vectors_copy = prob_vectors;
    sort(prob_vectors_copy.begin(), prob_vectors_copy.end());

    
    double threshold = prob_vectors_copy[number-1];
    
    vector<string> result(number, "");
    int count = 0;
    for (int i = 1; i < index; i++)
    {
        if (count > number) break;
        if (prob_vectors[i] <= threshold)
        {
            result[count] = filters[i];
            count += 1;
        }
    }
    
    std::fstream SequenceFile(sequenceFile, std::ios::out);
    for (int i = 0; i < number; i++)
        SequenceFile << result[i] << "\n";
    SequenceFile.close();
    
    
    map<int, int> frequence;
    for (int i = 0; i <= length; i++)
        frequence[i] = 0;
    
    for (int i = 0; i < number-1; i++)
    {
        for (int j = i+1; j < number; j++)
        {
            int Counts = 0;
            for (int position = 0; position < length; position++)
                if (result[i][position] == result[j][position])
                    Counts += 1;
            frequence[Counts] += 1;
        }
    }
    
    std::fstream DifferentSummary(differentSummary, std::ios::out);
    DifferentSummary << "Different base number" << "\t" << "Paired Sequence number\n";
    for (map<int, int>::iterator i = frequence.begin();
         i != frequence.end(); i++)
    {
        DifferentSummary << i->first << "\t" << i->second << "\n";
    }
    DifferentSummary.close();
    

    map<string, double> Nfrequence;
    std::fstream SiteFrequence(siteFrequence, std::ios::out);
    SiteFrequence << "Site" << "\t" << "A" << "\t" << "C" <<
                               "\t" << "T" << "\t" << "G" << "\n";
    
    for (int i = 0; i < length; i++)
    {
        Nfrequence["A"] = 0; Nfrequence["C"] = 0; Nfrequence["G"] = 0; Nfrequence["T"] = 0;
        for (int j = 0; j < number; j++)
        {
            Nfrequence[result[j].substr(i, 1)] += 1.0;
        }
        SiteFrequence << i+1 << "\t" << Nfrequence["A"]/number
                             << "\t" << Nfrequence["C"]/number
                             << "\t" << Nfrequence["T"]/number
                             << "\t" << Nfrequence["G"]/number
                             << "\t" << "\n";
        
    }
    SiteFrequence.close();

    //std::cout << filters[0] <<std::endl;
    //std::cout << index << std::endl;
    
    delete [] presequence;
    return 0;
}
