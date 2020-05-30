// Purpose:  cpp for steganography encoder and decoder object
// Author:   Jacob Sheets
// Date:     Apr 1 2019

#ifndef STAG_STEG_H
#define STAG_STEG_H

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "lodepng.h"

using namespace std;

class steg {


    public:

        steg();

        steg(string filename_o, string filename_mod, string text);

        steg(string filename_mod);


        //getters
        string getSecret() {return secret_text;}

        //encode
        void encode();

        //decode
        void decode();

    private:

        //pixels
        vector<unsigned char> image;
        unsigned width, height;

        string secret_text;

        string org_filename;
        string mod_filename;
};

#endif //STAG_STEG_H
