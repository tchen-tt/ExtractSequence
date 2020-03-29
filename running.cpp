//
//  main.cpp
//  running
//
//  Created by chentao on 2020/3/10.
//  Copyright © 2020 BioLab. All rights reserved.
//

#include <iostream>
#include <string>
#include <getopt.h>
#include <stdlib.h>
#include <fstream>


using std::string;

struct param
{
    string inputbam;
    string outbam;
    string cellbarcode;
    string barcodefile;
} args;

const struct option loptions[] =
{
    {"help", no_argument, NULL, 'h'},
    {"inputbam", required_argument, NULL, 'i'},
    {"outbam", required_argument, NULL, 'o'},
    {"filebarcode", required_argument, NULL, 'c'},
    {NULL, 0, NULL, 0}
};


int print_usage(void)
{
    std::cerr << std::endl;
    std::cerr << "About:       Corresponding cell sequences were extracted according to barcode\n";
    std::cerr << "Usage:       running [options]\n";
    std::cerr << "             -i,  inputbam all aligment bam files\n";
    std::cerr << "             -o,  outbam outputbamfiles directory\n";
    std::cerr << "             -f,  cell barcode file\n";
    return 1;
}


int runningCommands(const param & arg)
{
    string commands = "samtools view -h ";
    commands += arg.inputbam;
    commands += " | sed -n \"/^@\\|";
    commands += arg.cellbarcode;
    commands += "/p\" | ";
    commands += "samtools view -b > ";
    commands = commands + args.outbam + "/" +args.cellbarcode + ".bam";
    //commands += arg.outbam;
    //std::cout << commands << std::endl;
    int states = system(commands.c_str());
    return states;
}



int main(int argc, char * argv[]) {
    int opt;
    while ((opt = getopt_long(argc, argv, "hi:o:f:", loptions, NULL)) > 0 )
    {
        switch (opt)
        {
            case 'h': return print_usage();
            case 'i': args.inputbam = optarg; break;
            case 'o': args.outbam = optarg; break;
            case 'f': args.barcodefile = optarg; break;
            default: return print_usage();
        }
    }
    
    if (optind == 1) return print_usage();
    
    std::fstream barcode(args.barcodefile, std::ios::in);
    
    if (!barcode.is_open()) return print_usage();
    
    int resulting = 0;
    while (!barcode.eof()) {
        std::getline(barcode, args.cellbarcode, '\n');
        if (args.cellbarcode.length() > 0)
        {
            resulting = runningCommands(args);
            if (resulting != 0)
            {
                std::cerr << "Cell barcode " << args.cellbarcode << " error\n";
            }
            if (resulting == 0)
                std::cout << args.cellbarcode << " have done.\n";
        }
    }
    barcode.close();
    return 0;
}
