// md_html.cpp

#include <getopt.h>     
#include <string>
#include <iostream>     // For the std::cin and std::cout. I don't like, but... oh well
#include <fstream>      // Create, write and read files
#include "markdown.h" // Markdown parser
#include "Html.h"
#include "Component.h"

void Usage()
{   puts("Usage: md_html --input filename.md [--help] [--verbose] [--style style.css] [--input filename.md]");
}

int main(int argc, char *argv[]) {
    int c;
    std::string styleFile, inputFile, outputFile, stdinMarkdown;
    bool verbose_flag = false;
    while (1) {
        static struct option long_options[] = {
            {"input",       required_argument,  0, 'i'},
            {"verbose",     no_argument,        0, 'v'},
            {"help",        no_argument,        0, 'h'},
            {"style",       required_argument,  0, 's'}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long(argc, argv, "vhs:i:", long_options, &option_index);
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c) {
            case 'v':
                verbose_flag = true;
                break;
            case 'h':
                Usage();
                exit(0);
            case 's':
                styleFile = optarg;
                break;
            case 'i':
                inputFile = optarg;
                break;
            default:
                Usage();
                exit(1);
        }
    }
    if (verbose_flag)
        puts("Verbose flag is set");
    if (optind+1 < argc) {
        perror("Too many arguments\n");
        exit(2);
    }
    if(inputFile.empty()){
        printf("Input: stdin\nPress Ctrl+D (or Ctrl+Z on Windows) to end.\n");

        // Read from stdin and load it into a variable
        std::string line;
        do{
            std::getline(std::cin, line);
            stdinMarkdown += line + "\n";
        }while(!std::cin.eof());
    }else{
        std::ifstream inputStream(inputFile);

        if(inputStream.is_open()){
            std::string line;
            do{
                std::getline(inputStream, line);
                stdinMarkdown += line + "\n";
            }while(!inputStream.eof());

            inputStream.close();
        }else{
            printf("%s\n", outputFile.c_str());
            perror("Unable to open the output file");
            exit(3);
        }

        if(verbose_flag)
            printf("Input: %s\n", inputFile.c_str());
    }

    //printf("%s\n", stdinMarkdown.c_str());

    Html *output = new Html(markdown(stdinMarkdown));

    if(!styleFile.empty() && verbose_flag){
        printf("Style: %s\n", styleFile.c_str());
    }

    if(optind < argc){
        outputFile = std::string(argv[optind]);
        if(verbose_flag)
            printf("Output: %s\n", outputFile.c_str());

        std::ofstream outputStream (outputFile);
        if(outputStream.is_open()){
            outputStream << output->render();
            outputStream.close();
        }else{
            printf("%s\n", outputFile.c_str());
            perror("Unable to open the output file");
            exit(3);
        }
    }else{
        if(verbose_flag)
            printf("Output: stdout\n\n");

        printf("%s\n", output->render().c_str());
    }
    exit(0);
}
