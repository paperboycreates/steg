// Purpose:  Main program for steganography encoder and decoder
// Author:   Jacob Sheets
// Date:     Apr 1 2019

#include "steg.h"

using namespace std;

// Error Menu
static void show_usage(string parm) {

     cerr << "Usage: " << parm << " <option(s)> SOURCES"
          << endl
          << "\tsteg -e <original image name> <modified image name> [input ASCII text file name\n"
          << "\tsteg -d <modified image name> [output ASCII text file name]\n"
          << endl
          << "Options:\n"
          << "\t-h,--help\t\t\tShow this help message\n"
          << "\t-d,--decode DECODE\tDecodes Secert if any in the PNG\n"
          << "\t-e,--encode ENCODE\tEncodes PNG and Secret txt"
          << endl;
}


// ASCII STRING TO BINARY
// http://www.cplusplus.com/forum/general/1764/
string A_to_B(const char* input) {
    int ascii;           // used to store the ASCII number of a character
    int length = strlen(input);//find the length of the user's input
    string convertedString = "";

    //repeat until user's input have all been read
    for(int x = 0; x < length; x++) {
        ascii = input[x];        //store a character in its ASCII number

        /* Refer to http://www.wikihow.com/Convert-from-Decimal-to-Binary for conversion method used
         * in this program*/

        char* binary_reverse = new char [9];       //dynamic memory allocation
        char* binary = new char [9];

        int y = 0;    //counter used for arrays

        //continue until ascii == 1
        while(ascii != 1) {
            //if ascii is divisible by 2
            if(ascii % 2 == 0) {
                binary_reverse[y] = '0';   //then put a zero
            }
            //if it isnt divisible by 2
            else if(ascii % 2 == 1) {
                binary_reverse[y] = '1';   //then put a 1
            }
            ascii /= 2;    //find the quotient of ascii / 2
            y++;    //add 1 to y for next loop
        }

        //when ascii is 1, we have to add 1 to the beginning
        if(ascii == 1) {
            binary_reverse[y] = '1';
            y++;
        }

        //add zeros to the end of string if not 8 characters (1 byte)
        if(y < 8) {
            //add until binary_reverse[7] (8th element)
            for(; y < 8; y++) {
                binary_reverse[y] = '0';
            }
        }
        //our array is reversed. put the numbers in the rigth order (last comes first)
        for(int z = 0; z < 8; z++) {
            binary[z] = binary_reverse[7 - z];
        }
        delete [] binary_reverse;     //free the memory created by dynamic mem. allocation
        convertedString += binary;   //display the 8 digit binary number
    }
    return convertedString;
}




int main(int argc, char* argv[]) {



    // Throw error if not enough arguments
    if (argc < 2) {
        show_usage("no params");
        return 0;
    }

    // Help arguments
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {

        show_usage(argv[1]);
        return 0;
    }

    // File Names
    string orginal_image;
    string modified_image;
    string text_file;

    string text;

    // File Streams
    ifstream in_textFS;
    ofstream out_textFS;

    if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--encode") == 0) {
    //**************************************************//
                        // ENCODE //
    //**************************************************//
        orginal_image  =  argv[2];
        modified_image =  argv[3];

        // Check to see if stdin or file
        if (argc < 4) {

            cout << "Please print your secret message surrounded & press enter " << endl;
            getline (cin, text);

        } else {

            // Open Text File
            text_file      =  argv[4];
            in_textFS.open(text_file);

            // Throw Error if text file does not open
            if (!in_textFS.is_open()) {
                cerr << text_file << "File Failed to open"<< endl;
                show_usage(text_file);
                return 1;
            }

            // Parse text File
            string temp;
            while(!in_textFS.eof()) {
                getline(in_textFS, temp);
                text += temp;
            }
        }

        // Add special character to end to find end of message '/'
        text += '/';

        // Encode Image with text
        steg photo(orginal_image, modified_image, A_to_B(text.c_str()));
        photo.encode();

        // encoding complete
        cout << modified_image << " has been modified" << endl;

    } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--decode") == 0) {
    //*******************************************************//
                            // DECODE //
    //*******************************************************//
        modified_image  =  argv[2];

        // Check to see if stdout or file
        if (argc > 3) {

            //open file
            text_file = argv[3];
            out_textFS.open(text_file);

            // Throw Error if text file does not open
            if (!out_textFS.is_open()) {
                cerr << text_file << "File Failed to open" << endl;
                show_usage(text_file);
                return 1;
            }
        }

        // Decode hidden text
        steg photo(modified_image);
        photo.decode();

        //output text file, or stdout
        if (out_textFS.is_open()) {

            out_textFS << photo.getSecret();

        } else {

            cout << photo.getSecret();
        }
    }
}



