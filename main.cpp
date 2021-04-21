#include <iostream>
#include <fstream>
#include <amp.h>
#include <bitset>
#include <tuple>
#include <vector>


std::vector<std::tuple<int, unsigned char, int>> readfile(const std::string& filename){

    std::ifstream infile;
    infile.open(filename, std::ios::binary | std::ios::in);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(infile), {});

    std::vector<std::tuple<int, unsigned char, int>>  file_content;

    unsigned char temp;

    for (int i = 0; i < buffer.size(); i++){

        //std::cout <<  (int) buffer[i] << "\n";

        file_content.push_back(std::make_tuple(i, buffer[i], 0));

    }

    return file_content;

}

int writefile(std::vector<std::tuple<int, unsigned char, int>> content, std::string filename){

    std::ofstream outfile (filename);

    for (int i = 0; i < content.size(); i++){

        outfile << std::get<1>(content[i]);

    }

    outfile.close();

    return 0;

}

int encrypt()
{

    std::string filename;

    std::cout << "\nPlease Enter a file name: ";
    std::cin >> filename;

    std::cout << "\nPlease wait while I am loading your file\n";

    std::vector<std::tuple<int, unsigned char, int>> content = readfile(filename);

    std::cout << "\nFile has been loaded\n";

    std::vector<std::tuple<int, unsigned char, int>> encrypted;


//    std::cout << "\nPreview of content:\n";
//    for (int i = 0; i < 20; i++){
//
//        std::cout << (int) std::get<1>(content[i]) << std::endl;
//
//    }

    std::cout << "\nPlease wait while I am encrypting your file\n";
    for (int i = 0; i < content.size(); i++){

        encrypted.emplace_back(i, std::get<1>(content[i])+1, 0); //TODO this will later be replaced by more complex algorithm

    }
    std::cout << "\nEncryption Done\n";

//    std::cout << "\n\nPreview of Encrypted File:\n";
//    for (int i = 0; i < 20; i++){
//
//        std::cout << (int) std::get<1>(encrypted[i]) << std::endl;
//
//    }

    writefile(encrypted, "encrypted.max");

    return 0;

}

int decrypt()
{

    std::string filename;

    std::cout << "\nPlease Enter a file name: ";
    std::cin >> filename;

    std::cout << "\nPlease wait while I am loading your file\n";

    std::vector<std::tuple<int, unsigned char, int>> content = readfile(filename);

    std::cout << "\nFile has been loaded\n";

    std::vector<std::tuple<int, unsigned char, int>> decrypted;


//    std::cout << "\nPreview of content:\n";
//    for (int i = 0; i < 20; i++){
//
//        std::cout << (int) std::get<1>(content[i]) << std::endl;
//
//    }

    std::cout << "\nPlease wait while I am decrypting your file\n";
    for (int i = 0; i < content.size(); i++){

        decrypted.emplace_back(i, std::get<1>(content[i])-1, 0); //TODO this will later be replaced by more complex algorithm

    }
    std::cout << "\nDecryption Done\n";

//    std::cout << "\n\nPreview of Encrypted File:\n";
//    for (int i = 0; i < 20; i++){
//
//        std::cout << (int) std::get<1>(encrypted[i]) << std::endl;
//
//    }

    writefile(decrypted, "decrypted.max");

    return 0;

}

int main()
{

    int menu_choice = 0;

    std::cout << "Placeholder Menu\n 1. Encrypt\n2.Decrypt\n";

    std::cin >> menu_choice;

    if (menu_choice == 1){

        encrypt();

    }

    else if (menu_choice == 2){

        decrypt();

    }

    else{

        return 0;

    }

}
