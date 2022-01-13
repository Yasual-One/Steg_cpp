/**
 * @file main.cpp
 * @author Maciej Kołodziejczyk
 * @version 1.0.0
 * @date 08-01-2021
 */
 
#include "image.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(int argc, char * argv[]) {
    
    // Implementation of flags and their aliases
    int argument = 0;
    if(argc < 2){
        argument = 5;
    }else{
        if((strcmp(argv[1], "-i") == 0 || (strcmp(argv[1], "--info") == 0))){
            argument = 1;
        }else if((strcmp(argv[1], "-e") == 0 || (strcmp(argv[1], "--encrypt") == 0))){
            argument = 2;
        }else if((strcmp(argv[1], "-d") == 0 || (strcmp(argv[1], "--decrypt") == 0))){
            argument = 3;
        }else if((strcmp(argv[1], "-c") == 0 || (strcmp(argv[1], "--check") == 0))){
            argument = 4;
        }else if((strcmp(argv[1], "-h") == 0 || (strcmp(argv[1], "--help") == 0))){
            argument = 5;
        }
    }

    //Prepare an Image object to perform actions on
    Image image;
    
    //Execution of flags and aliases.
    switch (argument) {
        default:
            printf("ERROR(55) - Unknown option -%c\n", (*argv)[1]);
            std::cout << "for help check program_name -h\n" << std::endl;
            exit(55);
        case 1:
            if(argc != 3){
                std::cout << "ERROR(56) - Incorrect usage" << std::endl;
                std::cout << "for help check program_name -h\n" << std::endl;
                exit(56);
            }else{
                std::string arg = argv[2];
                const char * filePath = arg.c_str();
                image.checkFileInfo(filePath, true);
                exit(0);
            }
        case 2:
            if(argc != 4){
                std::cout << "ERROR(56) - Incorrect usage" << std::endl;
                std::cout << "for help check program_name -h\n" << std::endl;
                exit(56);
            }else{
                std::string arg1 = argv[2];
                const char * filePath = arg1.c_str();
                image.readFile(filePath);
                std::string arg2 = argv[3];
                image.encode(arg2);
                
                //Get current path and create export file name;
                std::string currentPath = std::filesystem::current_path();
                std::string exportFileName = image.getExportFileName();
                std::string temp = currentPath + exportFileName;
                const char * exportPath = temp.c_str();
                
                image.exportFile(exportPath);
                exit(0);
            }
        case 3:
            if(argc != 3){
                std::cout << "ERROR(56) - Incorrect usage" << std::endl;
                std::cout << "for help check program_name -h\n" << std::endl;
                exit(56);
            }else{
                std::string arg1 = argv[2];
                const char * filePath = arg1.c_str();
                image.readFile(filePath);
                image.decode();
                exit(0);
            }
        case 4:
            if(argc != 4){
                std::cout << "ERROR(56) - Incorrect usage" << std::endl;
                std::cout << "for help check program_name -h\n" << std::endl;
                exit(56);
            }else{
                std::string arg1 = argv[2];
                const char * filePath = arg1.c_str();
                image.readFile(filePath);
                std::string arg2 = argv[3];
                image.checkIfMessageFits(arg2);
                exit(0);
            }
        case 5:
            if(argc > 2){
                std::cout << "ERROR(56) - Incorrect usage" << std::endl;
                std::cout << "for help check program_name -h\n" << std::endl;
                exit(56);
            }
            //Program Info
            std::cout << std::endl;
            std::cout << "__________________________________________________________________________________________________" << std::endl;
            std::cout << "PROJECT STEGANOGRAPHY v1.0.0:" << std::endl;
            std::cout << "Created by: Maciej Kolodziejczyk S20661" << std::endl;
            std::cout << "__________________________________________________________________________________________________" << std::endl;
            //Supported File Types
            std::cout << "SUPPORTED FILE TYPES:" << std::endl;
            std::cout << "BMP" << std::endl;
            std::cout << "PGM" << std::endl;
            std::cout << "PNG" << std::endl;
            std::cout << "ICO" << std::endl;
            std::cout << "__________________________________________________________________________________________________" << std::endl;
            //Commands
            std::cout << "AVAILABLE COMMANDS:" << std::endl;
            std::cout << "-i --info \t|\t Information about file \t|\t Steg_cpp -i " << '"' << "filepath" << '"' << std::endl;
            std::cout << "-e --encode \t|\t Encode a message \t\t|\t Steg_cpp -e " << '"' << "filepath" << '"' << ' ' << '"' << "message" << '"' << std::endl;
            std::cout << "-d --decode \t|\t Decode a message \t\t|\t Steg_cpp -d " << '"' << "filepath" << '"' << std::endl;
            std::cout << "-c --check \t|\t Check if message will encode \t|\t Steg_cpp -c " << '"' << "filepath" << '"' << ' '  << '"' << "message" << '"' << std::endl;
            std::cout << "-h --help \t|\t Help (This Screen) \t\t|\t Steg_cpp -h" << std::endl;
            std::cout << "__________________________________________________________________________________________________" << std::endl;
            //Error Codes
            std::cout << "ERROR CODES:" << std::endl;
            std::cout << "50 - File could not be read" << std::endl;
            std::cout << "51 - File could not be created" << std::endl;
            std::cout << "52 - Message is too big to encode in this file" << std::endl;
            std::cout << "53 - No encoded message found" << std::endl;
            std::cout << "54 - The specified file is not supported" << std::endl;
            std::cout << "55 - Unknown option" << std::endl;
            std::cout << "56 - Incorrect usage" << std::endl;
            std::cout << "__________________________________________________________________________________________________" << std::endl;
    }
}
