/**
 * @file image.cpp
 * @author Maciej Kołodziejczyk
 * @version 1.0.0
 * @date 08-01-2021
 */
 

#include "image.hpp"
#include <sstream>
#include <bitset>

using namespace std;

BMPPixel::BMPPixel() : r(0), g(0), b(0){
    
}

BMPPixel::BMPPixel(int r, int g, int b) : r(r), g(g), b(b){
    
}

BMPPixel::~BMPPixel(){
    
}

Image::Image() : rows(0), columns(0){
    
}

Image::Image(int width, int height){
    
}

Image::~Image(){
    
}

///Calculate the index at which a value of specified coordinates resides and get it. This is due to holding values in a vector and not in a matrix. (FOR PGM FILES)
int Image::getGrayVal(int x, int y){
    return valuesPGM[y * columns + x];
}

///Calculate the index at which a value of specified coordinates resides and set it to specified value. This is due to holding values in a vector and not in a matrix. (FOR PGM FILES)
void Image::setGrayVal(int x, int y ,int val){
    valuesPGM[y * columns + x] = val;
}

///Calculate the index at which a value of specified coordinates resides and get it. This is due to holding values in a vector and not in a matrix. (FOR BMP FILES)
BMPPixel Image::getColor(int x, int y) const{
    return pixels[y * columns + x];
}

///Calculate the index at which a value of specified coordinates resides and set it to specified value. This is due to holding values in a vector and not in a matrix. (FOR BMP FILES)
void Image::setColor(const BMPPixel& color, int x, int y){
    pixels[y * columns + x].r = color.r;
    pixels[y * columns + x].g = color.g;
    pixels[y * columns + x].b = color.b;
}

///Read first two bytes of a specified file in order to extract the format identifier
void Image::checkFileFormat(const char* filePath){
    
    path = filePath;
    
    ifstream reader;
    reader.open(filePath, ios::in | ios::binary);
    
    if (!reader.is_open()){
        cout << "ERROR(50) - File could not be read" << endl;;
        cout << "for help check program_name -h\n" << endl;
        exit(50);
    }
    
    const int fileHeaderSize = 8;
    unsigned char fileHeader[fileHeaderSize];
    
    reader.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    
    reader.close();
    
    if(fileHeader[0] == 'B' || fileHeader[1] == 'M'){
        format = "BM";
        exportFileName = "/ENCODED.bmp";
    }else if(fileHeader[0] == 'P' || fileHeader[1] == '2'){
        format = "P2";
        exportFileName = "/ENCODED.pgm";
    }else if(fileHeader[1] == 'P' || fileHeader[2] == 'N' || fileHeader[3] == 'G'){
        format = "PNG";
        exportFileName = "/ENCODED.png";
    }else if(fileHeader[0] == 0 || fileHeader[1] == 0 || fileHeader[2] == 0 || fileHeader[3] == 1){
        format = "ICO";
        exportFileName = "/ENCODED.ico";
    }else{
        cout << "ERROR(54) - The specified file is not supported" << endl;;
        cout << "for help check program_name -h\n" << endl;
        exit(54);
    }
}

string Image::getExportFileName(){
    return exportFileName; 
}

///Read file header and information header of a specified file in order to present the information about a file
void Image::checkFileInfo(const char* filePath, bool showFileInfo){
    checkFileFormat(filePath);
    if(format == "BM") {
        
        ifstream reader;
        reader.open(filePath, ios::in | ios::binary);
        
        if (!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        
        vector<int> tempHeaders;
        
        for (int i = 0; i < 54; i++) {
            tempHeaders.push_back(reader.get());
        }
        
        int fileSize = tempHeaders[2] + (tempHeaders[3] << 8) + (tempHeaders[4] << 16) + (tempHeaders[5] << 24);
        columns = tempHeaders[18] + (tempHeaders[19] << 8) + (tempHeaders[20] << 16) + (tempHeaders[21] << 24);
        rows = tempHeaders[22] + (tempHeaders[23] << 8) + (tempHeaders[24] << 16) + (tempHeaders[25] << 24);
        
        pixels.resize(columns * rows);
        
        if(showFileInfo == true){
        cout << "File format: BMP" << endl;
        cout << "Image Resolution: " << columns << 'x' << rows << endl;
        cout << "Ammount of pixels: " << columns*rows << endl;
        cout << "File Size: " << fileSize << " bytes" << endl;
        };
        
        reader.close();
    }else if (format == "P2"){
        
        fstream reader;
        reader.open(filePath, ios::in);
        
        if(!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        
        string temp;
        string part1;
        string part2;
        
        getline(reader, format);
        getline(reader, temp);
        
        
        if (temp.at(0) == '#') {
            comment = temp;
            reader >> columns;
            reader >> rows;
        }else{
            part1 = temp.substr(0, temp.find(' '));
            temp.erase(0, temp.find(' ') + 1);
            part2 = temp.substr(0, temp.find(' '));
            stringstream firstVal(part1);
            stringstream secondVal(part2);
            firstVal >> columns;
            secondVal >> rows;
        }
        
        reader >> maxVal;
        if (showFileInfo == true) {
            cout << "File format: PGM" << endl;
            cout << "Image Resolution: " << columns << 'x' << rows << endl;
            cout << "Ammount of pixels: " << columns*rows << endl;
            cout << "File Size: " << (format.size() + comment.size() + (rows*columns*4) + maxVal) << " bytes" << endl;
        }
        
        reader.close();
        
    }else if(format == "PNG"){
        
        ifstream reader;
        reader.open(filePath, ios::in | ios::binary);
        
        if (!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        vector<int> tempHeaders;
        
        reader.ignore(8);
        
        for (int i = 0; i < 4; i++) {
            tempHeaders.push_back(reader.get());
        }
        
        unsigned int chunkLength = tempHeaders[3] + (tempHeaders[2] << 8) + (tempHeaders[1] << 16) + (tempHeaders[0] << 24);
        
        for (int i = 0; i < 4; i++) {
            tempHeaders.push_back(reader.get());
        }
        
        string chunkType;
        
        chunkType.push_back((char) tempHeaders[4]);
        chunkType.push_back((char) tempHeaders[5]);
        chunkType.push_back((char) tempHeaders[6]);
        chunkType.push_back((char) tempHeaders[7]);
        
        if (chunkType == "IHDR") {
            for (int i = 0; i < chunkLength; i++) {
                tempHeaders.push_back(reader.get());
            }
        }
        
        reader.close();
        
        columns = tempHeaders[11] + (tempHeaders[10] << 8) + (tempHeaders[9] << 16) + (tempHeaders[8] << 24);
        rows = tempHeaders[15] + (tempHeaders[14] << 8) + (tempHeaders[13] << 16) + (tempHeaders[12] << 24);
        
        
        if (showFileInfo == true) {
            cout << "File format: PNG" << endl;
            cout << "Image Resolution: " << columns << 'x' << rows << endl;
            cout << "Ammount of pixels: " << columns*rows << endl;
        }
        
    }else if(format == "ICO"){
        
        ifstream reader;
        reader.open(filePath, ios::in | ios::binary);
        
        if (!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        vector<int> tempHeaders;
        
        for (int i = 0; i < 6; i++) {
            tempHeaders.push_back(reader.get());
        }
        int ammountOfImages = tempHeaders[4] + (tempHeaders[5] << 8);
        
        for (int i = 0; i < 16*ammountOfImages; i++) {
            tempHeaders.push_back(reader.get());
        }
        
        if(tempHeaders[6] == 0){
            columns = 256;
        }else{
            columns = tempHeaders[6];
        }
        
        if(tempHeaders[7] == 0){
            rows = 256*2;
        }else{
            rows = tempHeaders[7]*2;
        }
        
        pixels.resize(columns * rows*2);
            
        int fileSize = tempHeaders[14] + (tempHeaders[15] << 8) + (tempHeaders[16] << 16) + (tempHeaders[17] << 24);
        
        if (showFileInfo == true) {
            cout << "File format: ICO" << endl;
            cout << "Image Resolution: " << columns << 'x' << rows << endl;
            cout << "Ammount of pixels: " << columns*rows << endl;
            cout << "File Size: " << fileSize << " bytes" << endl;
        }
        reader.close();
        
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///Read the file from the beginning to the end and assign read values to the right variables
void Image::readFile(const char *filePath){
    checkFileInfo(filePath, false);
    
    if(format == "BM"){
        ifstream reader;
        reader.open(filePath, ios::in | ios::binary);
        
        if (!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        
        for (int i = 0; i < 54; i++) {
            headers.push_back(reader.get());
        }
        
        const int paddingAmount = (((4 - (columns * 3) % 4) % 4));
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                unsigned char color[3];
                reader.read(reinterpret_cast<char*>(color), 3);
                
                pixels[y * columns + x].r = (color[2]);
                pixels[y * columns + x].g = (color[1]);
                pixels[y * columns + x].b = (color[0]);
            }
            
            reader.ignore(paddingAmount);
        }
        
        reader.close();
        
    }else if (format == "P2"){
        fstream reader;
        reader.open(filePath, ios::in);
        
        if(!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        
        string temp;
        string part1;
        string part2;
        
        getline(reader, format);
        getline(reader, temp);
        
        
        if (temp.at(0) == '#') {
            comment = temp;
            reader >> columns;
            reader >> rows;
        }else{
            part1 = temp.substr(0, temp.find(' '));
            temp.erase(0, temp.find(' ') + 1);
            part2 = temp.substr(0, temp.find(' '));
            stringstream firstVal(part1);
            stringstream secondVal(part2);
            firstVal >> columns;
            secondVal >> rows;
        }
        
        reader >> maxVal;
        
        valuesPGM.resize(columns * rows);
        
        //READ ALL GREYVALUES INTO VECTOR
        for (unsigned int i = 0; i < columns * rows; i++) {
            reader >> valuesPGM[i];
        }
        
        reader.close();
    }else if(format == "PNG"){
        
        std::ifstream reader(filePath, std::ios::binary);
        std::vector<unsigned char> bytes(
            (std::istreambuf_iterator<char>(reader)),
            (std::istreambuf_iterator<char>()));
        reader.seekg(0, std::ios::end);
        reader.close();
        
        for (int i = 0; i < bytes.size(); i++) {
            headers2.push_back(bytes.at(i));
        }
        
        reader.close();
        
    }else if(format == "ICO"){
        
        ifstream reader;
        reader.open(filePath, ios::in | ios::binary);
        
        if (!reader.is_open()){
            cout << "ERROR(50) - File could not be read" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(50);
        }
        
        for (int i = 0; i < 6; i++) {
            headers.push_back(reader.get());
        }
        int ammountOfImages = headers[4] + (headers[5] << 8);
        
        for (int i = 0; i < 16*ammountOfImages+16; i++) {
            headers.push_back(reader.get());
        }
        
        const int paddingAmount = (((4 - (columns * 3) % 4) % 4));
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                unsigned char color[3];
                reader.read(reinterpret_cast<char*>(color), 3);
                
                pixels[y * columns + x].r = (color[2]);
                pixels[y * columns + x].g = (color[1]);
                pixels[y * columns + x].b = (color[0]);
            }
            
            reader.ignore(paddingAmount);
        }
        
        reader.close();
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///Take all the modified data about an image and write it to a file. Can also act as an image copier if no changes were made to the original file.
void Image::exportFile(const char *filePath){
    
    path = filePath;
    if(format == "BM"){
        ofstream writer;
        writer.open(path, ios::out | ios::binary);
        
        if(!writer.is_open()){
            cout << "ERROR(51) - File could not be created" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(51);
        }
        
        unsigned char padding[3] = {0,0,0};
        const int paddingAmount = ((4 - (columns * 3) % 4) %4);

        for (int i = 0; i < headers.size(); i++) {
            writer.put(headers.at(i));
        }
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
                unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
                unsigned char b = static_cast<unsigned char>(getColor(x, y).b);
                
                unsigned char pixel[] = {b, g, r};
                
                writer.write(reinterpret_cast<char*>(pixel), 3);
                
            }
            
            writer.write(reinterpret_cast<char*>(padding), paddingAmount);
        }
        
        writer.close();
    
    }else if (format == "P2"){
        fstream writer;
        writer.open(filePath, ios::out);
        
        if(!writer.is_open()){
            cout << "ERROR(51) - File could not be created" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(51);
        }
        
        writer << "P2" << endl;
        writer << "#Exported by Maciej Kołodziejczyk" << endl;
        writer << columns << ' ' << rows << endl;
        writer << maxVal << endl;
        
        //TODO: CHECK
        for (unsigned int i=0, j=0; i < rows * columns; i++, j++) {
            writer << valuesPGM[i] << " ";
            if(j == 16){
                writer << endl;
                j = -1;
            }
        }
        
        writer.close();
    }else if(format == "PNG"){
        
        ofstream writer;
        writer.open(path, ios::out | ios::binary);
        
        if(!writer.is_open()){
            cout << "ERROR(51) - File could not be created" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(51);
        }

        for (int i = 0; i < headers2.size(); i++) {
            writer.put(headers2.at(i));
        }
        
        writer.close();
        
    }else if(format == "ICO"){
            ofstream writer;
            writer.open(path, ios::out | ios::binary);
            
            if(!writer.is_open()){
                cout << "ERROR(51) - File could not be created" << endl;;
                cout << "for help check program_name -h\n" << endl;
                exit(51);
            }
            
            unsigned char padding[3] = {0,0,0};
            const int paddingAmount = ((4 - (columns * 3) % 4) %4);

            for (int i = 0; i < headers.size(); i++) {
                writer.put(headers.at(i));
            }
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
                unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
                unsigned char b = static_cast<unsigned char>(getColor(x, y).b);
                
                unsigned char pixel[] = {b, g, r};
                
                writer.write(reinterpret_cast<char*>(pixel), 3);
                
            }
            
            writer.write(reinterpret_cast<char*>(padding), paddingAmount);
        }
            
            
            writer.close();
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///Take the message, split it into chars, extract all the bits of each char and store them in a vector that is later returned;
vector<int> Image::checkIfMessageFits(string message){

    
    if(format == "BM"){
        vector<char> charsOfMessage(message.begin(), message.end());
        vector<int> bitsOfMessage;
        
        //convert chars of message into bits and store in a vector
        for (int i = 0; i < charsOfMessage.size(); i++) {
            for (int n = 7; n >= 0; n--) {
                bitset<8> letter (charsOfMessage.at(i));
                bitsOfMessage.push_back(letter.test(n));
            }
        }
        
        if((bitsOfMessage.size() > (pixels.size()*3))){
            cout << "ERROR(52) - Message is too big to encode in this file" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(52);
        }else{
            return bitsOfMessage;
        }
    }else if (format == "P2"){
        vector<char> charsOfMessage(message.begin(), message.end());
        vector<int> bitsOfMessage;
        
        for (int i = 0; i < charsOfMessage.size(); i++) {
            for (int n = 7; n >= 0; n--) {
                bitset<8> letter (charsOfMessage.at(i));
                bitsOfMessage.push_back(letter.test(n));
            }
        }
        
        if((bitsOfMessage.size() > valuesPGM.size())){
            cout << "ERROR(52) - Message is too big to encode in this file" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(52);
        }else{
            return bitsOfMessage;
        }
    }else if(format == "PNG"){
        vector<char> charsOfMessage(message.begin(), message.end());
        vector<int> bitsOfMessage;
        
        //convert chars of message into bits and store in a vector
        for (int i = 0; i < charsOfMessage.size(); i++) {
            for (int n = 7; n >= 0; n--) {
                bitset<8> letter (charsOfMessage.at(i));
                bitsOfMessage.push_back(letter.test(n));
            }
        }
        
        if((bitsOfMessage.size() > 512)){
            cout << "ERROR(52) - Message is too big to encode in this file" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(52);
        }else{
            return bitsOfMessage;
        }
    }else if(format == "ICO"){
        vector<char> charsOfMessage(message.begin(), message.end());
        vector<int> bitsOfMessage;
        
        //convert chars of message into bits and store in a vector
        for (int i = 0; i < charsOfMessage.size(); i++) {
            for (int n = 7; n >= 0; n--) {
                bitset<8> letter (charsOfMessage.at(i));
                bitsOfMessage.push_back(letter.test(n));
            }
        }
        
        if((bitsOfMessage.size() > (pixels.size()*3))){
            cout << "ERROR(52) - Message is too big to encode in this file" << endl;;
            cout << "for help check program_name -h\n" << endl;
            exit(52);
        }else{
            return bitsOfMessage;
        }
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///Take the message, use the above function (checkIfMessageFits(string message)) to split it into chars, extract all the bits of each char and store them in a vector that is later returned, and use the returned vector to change the Least Significant Bit of every pixel value to every bit of the message.
void Image::encode(string message){
    
    if(format == "BM"){
        string delimiter = "####";
        message = delimiter + message + delimiter + ' ';
        vector<int> bitsOfMessage;
        
        bitsOfMessage = checkIfMessageFits(message);
        
        //ENCODE MESSAGE
        int rVal;
        int gVal;
        int bVal;
        int bitOfMessage = 0;
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                if(bitOfMessage >= bitsOfMessage.size() - 3){
                    break;
                }
                rVal = (getColor(x, y).r);
                bitset<8> tempR (rVal);
                rVal = (static_cast<int>(tempR.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                gVal = (getColor(x, y).r);
                bitset<8> rempG (gVal);
                gVal = (static_cast<int>(rempG.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                bVal = (getColor(x, y).r);
                bitset<8> tempB (bVal);
                bVal = (static_cast<int>(tempB.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                BMPPixel color(rVal, gVal, bVal);
                setColor(color, x, y);
                
            }
        }
    }else if (format == "P2"){
        string delimiter = "####";
        message = delimiter + message + delimiter + ' ';
        vector<int> bitsOfMessage;
        
        bitsOfMessage = checkIfMessageFits(message);
        
        int val;
        int bitOfMessage = 0;
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                if(bitOfMessage >= bitsOfMessage.size()-1){
                    break;
                }
                val = getGrayVal(x, y);
                bitset<8> tempVal (val);
                val = (static_cast<int>(tempVal.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                setGrayVal(x, y, val);
            }
        }
    }else if(format == "PNG"){
        
        // one endcout << headers2[headers2.size()-529] << endl;
        // another end cout << headers2[headers2.size()-17] << endl;
        string delimiter = "####";
        message = delimiter + message + delimiter + ' ';
        vector<int> bitsOfMessage;
        bitsOfMessage = checkIfMessageFits(message);
        
        //Create a separate custom chunk that holds only encoded data;
        int cCL1 = 512 >> 24;
        int cCL2 = 512 >> 16;
        int cCL3 = 512 >> 8;
        int cCL4 = 512;
        
        headers2.insert(headers2.end()-12, cCL1);
        headers2.insert(headers2.end()-12, cCL2);
        headers2.insert(headers2.end()-12, cCL3);
        headers2.insert(headers2.end()-12, cCL4);
        headers2.insert(headers2.end()-12, 'e');
        headers2.insert(headers2.end()-12, 'n');
        headers2.insert(headers2.end()-12, 'C');
        headers2.insert(headers2.end()-12, 'd');
        for (int i = 0; i < 512; i++) {
            headers2.insert(headers2.end()-12, 128);
        }
        headers2.insert(headers2.end()-12, 0);
        headers2.insert(headers2.end()-12, 0);
        headers2.insert(headers2.end()-12, 0);
        headers2.insert(headers2.end()-12, 0);
        
        //encode
        for (int i = 0, j = 529; i < bitsOfMessage.size(); i++, j--) {
            char val = headers2[headers2.size()-j];
            bitset<8> tempVal (val);
            val = (static_cast<char>(tempVal.set(0,bitsOfMessage.at(i)).to_ulong()));
            headers2[headers2.size()-j] = val;
        }
        
    }else if(format == "ICO"){
        string delimiter = "####";
        message = delimiter + message + delimiter + ' ';
        vector<int> bitsOfMessage;
        
        bitsOfMessage = checkIfMessageFits(message);
        
        //ENCODE MESSAGE
        int rVal;
        int gVal;
        int bVal;
        int bitOfMessage = 0;
        
        for (int y = 0; y < rows*2; y++) {
            for (int x = 0; x < columns; x++) {
                if(bitOfMessage >= bitsOfMessage.size() - 3){
                    break;
                }
                rVal = (getColor(x, y).r);
                bitset<8> tempR (rVal);
                rVal = (static_cast<int>(tempR.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                gVal = (getColor(x, y).r);
                bitset<8> rempG (gVal);
                gVal = (static_cast<int>(rempG.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                bVal = (getColor(x, y).r);
                bitset<8> tempB (bVal);
                bVal = (static_cast<int>(tempB.set(0,bitsOfMessage.at(bitOfMessage)).to_ulong()));
                bitOfMessage++;
                
                BMPPixel color(rVal, gVal, bVal);
                setColor(color, x, y);
                
            }
        }
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///For each pixel of the image, extract its value as bits, store them in a vector that is later returned and check the first 4 bytes for the identifier that signifies that there is a hidden message inside (in this case the 4 first bytes shoudl equate to 4 chars "####")
vector<int> Image::lookForMessage(){
    
    if(format == "BM"){
        vector<int> recoveredBits;
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                bitset<8> temp_r(getColor(x, y).r);
                recoveredBits.push_back(temp_r.test(0));
                
                bitset<8> temp_g(getColor(x, y).g);
                recoveredBits.push_back(temp_g.test(0));
                
                bitset<8> temp_b(getColor(x, y).b);
                recoveredBits.push_back(temp_b.test(0));
            }
        }
        
        bitset<8> test;
        for (int i = 0; i < 4; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            if(test.to_ulong() != 35){
                cout << "ERROR(53) - No encoded message found" << endl;;
                cout << "for help check program_name -h\n" << endl;
                exit(53);
            }
        }
        recoveredBits.erase(recoveredBits.begin(), recoveredBits.begin()+32);
        
        return recoveredBits;
    }else if (format == "P2"){
        vector<int> recoveredBits;
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                bitset<8> tempVal (getGrayVal(x, y));
                recoveredBits.push_back(tempVal.test(0));
            }
        }
        
        bitset<8> test;
        for (int i = 0; i < 4; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            if(test.to_ulong() != 35){
                cout << "ERROR(53) - No encoded message found" << endl;;
                cout << "for help check program_name -h\n" << endl;
                exit(53);
            }
        }
        recoveredBits.erase(recoveredBits.begin(), recoveredBits.begin()+32);
        
        return recoveredBits;
    }else if(format == "PNG"){
        vector<int> recoveredBits;
        
        for (int i = 0, j = 529; i < 512; i++, j--) {
            char val = headers2[headers2.size()-j];
            bitset<8> tempVal (val);
            recoveredBits.push_back(tempVal.test(0));
        }
        
        bitset<8> test;
        for (int i = 0; i < 4; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            if(test.to_ulong() != 35){
                cout << "ERROR(53) - No encoded message found" << endl;;
                cout << "for help check program_name -h\n" << endl;
                exit(53);
            }
        }
        recoveredBits.erase(recoveredBits.begin(), recoveredBits.begin()+32);
        
        return recoveredBits;
        
    }else if(format == "ICO"){
        vector<int> recoveredBits;
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                bitset<8> temp_r(getColor(x, y).r);
                recoveredBits.push_back(temp_r.test(0));
                
                bitset<8> temp_g(getColor(x, y).g);
                recoveredBits.push_back(temp_g.test(0));
                
                bitset<8> temp_b(getColor(x, y).b);
                recoveredBits.push_back(temp_b.test(0));
            }
        }
        
        bitset<8> test;
        for (int i = 0; i < 4; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            if(test.to_ulong() != 35){
                cout << "ERROR(53) - No encoded message found" << endl;;
                cout << "for help check program_name -h\n" << endl;
                exit(53);
            }
        }
        recoveredBits.erase(recoveredBits.begin(), recoveredBits.begin()+32);
        
        return recoveredBits;
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

///Use the above function (lookForMessage()) to check if there is anything to decode, if yes use the returned vector containing all the bits of image data to decode the message. The decoding stos after '#' char is found 4 times in a row.
void Image::decode(){
    
    if(format == "BM"){
        vector<int> recoveredBits = lookForMessage();
        
        //decode message
        bitset<8> test;
        vector<char> recoveredChars;
        int counter = 0;
        for (int i = 0; i < recoveredBits.size()/8; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            recoveredChars.push_back(test.to_ulong());
            if(test.to_ulong() == 35) {
                counter++;
            }else{
                counter = 0;
            }
            if (counter >= 4) {
                break;
            }
        }
        
        string message(recoveredChars.begin(), recoveredChars.end()-4);
        cout << "Hidden message is: " << message << endl;
    }else if (format == "P2"){
        vector<int> recoveredBits = lookForMessage();
        
        bitset<8> test;
        vector<char> recoveredChars;
        int counter = 0;
        for (int i = 0; i < recoveredBits.size()/8; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            recoveredChars.push_back(test.to_ulong());
            if(test.to_ulong() == 35) {
                counter++;
            }else{
                counter = 0;
            }
            if (counter >= 4) {
                break;
            }
        }
        
        string message(recoveredChars.begin(), recoveredChars.end()-4);
        cout << "Hidden message is: " << message << endl;
    }else if(format == "PNG"){
        vector<int> recoveredBits = lookForMessage();
        
        //decode message
        bitset<8> test;
        vector<char> recoveredChars;
        int counter = 0;
        for (int i = 0; i < recoveredBits.size()/8; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            recoveredChars.push_back(test.to_ulong());
            if(test.to_ulong() == 35) {
                counter++;
            }else{
                counter = 0;
            }
            if (counter >= 4) {
                break;
            }
        }
        
        string message(recoveredChars.begin(), recoveredChars.end()-4);
        cout << "Hidden message is: " << message << endl;
    }else if(format == "ICO"){
        vector<int> recoveredBits = lookForMessage();
        
        //decode message
        bitset<8> test;
        vector<char> recoveredChars;
        int counter = 0;
        for (int i = 0; i < recoveredBits.size()/8; i++) {
            for (int y = 0; y < 8; y++) {
                test.set(7-y, recoveredBits.at(y+(i*8)));
            }
            recoveredChars.push_back(test.to_ulong());
            if(test.to_ulong() == 35) {
                counter++;
            }else{
                counter = 0;
            }
            if (counter >= 4) {
                break;
            }
        }
        
        string message(recoveredChars.begin(), recoveredChars.end()-4);
        cout << "Hidden message is: " << message << endl;
    }else{
        cout << "ERROR(58) - Unknown error" << endl;;
        exit(58);
    }
}

