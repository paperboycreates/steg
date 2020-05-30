// Purpose:  Header for steganography encoder and decoder object
// Author:   Jacob Sheets
// Date:     Apr 1 2019

#include "steg.h"

steg::steg() {

    org_filename = "";
    mod_filename = "";
    secret_text  = "";
}

steg::steg(string filename_o, string filename_mod, string text) {

    org_filename  =  filename_o;
    mod_filename  =  filename_mod;
    secret_text   =  text;
}

steg::steg(string filename_mod) {

    org_filename = "";
    mod_filename = filename_mod;
    secret_text  = "";
}

void steg::encode() {

    //decode orginal image
    unsigned error = lodepng::decode(image, width, height, org_filename);

    //if there's an error, display it
    if(error) cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;

    //walk through secret text and insert into pixels
    int secret_index = 0;
    while (secret_index != secret_text.length()) {

        char temp = secret_text.at(secret_index);
        int n = temp - '0';
        image.at(secret_index) ^= (-n ^ image.at(secret_index)) & (1UL << 0);
        secret_index++;
    }

    //encode modified image
    error = lodepng::encode(mod_filename, image, width, height);

    //if there's an error, display it
    if(error) cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << endl;
}

void steg::decode() {

    //decode modified iamge
    unsigned error = lodepng::decode(image, width, height, mod_filename);

    //if there's an error, display it
    if (error) cerr << "encoder error " << error << ": " << lodepng_error_text(error) << endl;


    bool endCharacter = false;
    int index = 0;
    string charTest = "";

    //walk through and grab all hidden bits
    while(!endCharacter) {

        bitset<8> temp(image[index]);
        int eightBit = temp.test(0); // jake said 0 not 7
        charTest +=  to_string(eightBit);

        //check to see if at end of the list
        if (charTest.length() == 8) {
            bitset<8> endChar (charTest);
            unsigned long endLong = endChar.to_ulong();
            unsigned char c = static_cast<unsigned char>(endLong);
            if (c == '/') {
                endCharacter = true;
            } else {
                secret_text += c;
                charTest = "";
            }
        }
        index++;
    }
}




