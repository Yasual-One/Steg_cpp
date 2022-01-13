/**
 * @file image.hpp
 * @author Maciej Kołodziejczyk
 * @version 1.0.0
 * @date 08-01-2021
 */
 
#ifndef image_hpp
#define image_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

/**
 * @struct BMPPixel
 * @brief BMPPixel struct used for storing BMP pixel data.
 * @details BMPPixel struct containing all three color values (R, G and B) of a single pixel;
 */
struct BMPPixel{
    int r, g, b;
    
    /**
     * @brief Default constructor
     * @details Create a BMPPixel with all values set to 0;
     * @see BMPPixel(int r, int g, int b);
     * @see ~BMPPixel();
     */
    BMPPixel();
    
    /**
     * @brief Constructor
     * @details Create a BMPPixel with from RGB values;
     * @param r Value of the red componen of a pixel
     * @param g Value of the red componen of a pixel
     * @param b Value of the red componen of a pixel
     * @see BMPPixel();
     * @see ~BMPPixel();
     */
    BMPPixel(int r, int g, int b);
    
    /**
     * @brief Destructor
     * @see BMPPixel(int r, int g, int b);
     * @see BMPPixel();
     */
    ~BMPPixel();
};

/**@class Image
 * @brief Image class used for Image manipulation.
 * @details Image class containing basic Image data and functions permitting reading, manipulating and writing image data
 */
class Image{
public:
    
    /**
     * @brief Default constructor
     * @details Create an Image object with height and width set to 0
     * @see Image(int width, int height);
     * @see Image~();
     */
    Image();
    
    /**
     *
     * @brief Constructor
     * @details Create an Image object with width and length
     * @param width Image width / Horizontal resolution
     * @param height Image height / Vertical resolution
     * @see Image()
     * @see Image~();
     */
    Image(int width, int height);
    
    /**
     * @brief Destructor
     * @see Image(int width, int height);
     * @see Image();
     */
    ~Image();
    
    /**
     * @brief Get PGM image value
     * @details Get the grayscale value of a specific pixel in the image (PGM Only)
     * @param x Horizontal location of a grayscale value
     * @param y Vertical location of a grayscale value
     * @return Grayscale at specified location in the image
     * @see void setGrayVal(int x, int y ,int val);
     * @see BMPPixel getColor(int x, int y) const;
     * @see void setColor(const BMPPixel& color, int x, int y);
     */
    int getGrayVal(int x, int y);
    
    /**
     * @brief Set PGM image value
     * @details Set the grayscale value of a specific pixel in the image (PGM Only)
     * @param x Horizontal location of a grayscale value
     * @param y Vertical location of a grayscale value
     * @param val Value to be set at specified location
     * @see int getGrayVal(int x, int y);
     * @see BMPPixel getColor(int x, int y) const;
     * @see void setColor(const BMPPixel& color, int x, int y);
     */
    void setGrayVal(int x, int y ,int val);
    
    /**
     * @brief Get RGB values of a pixel
     * @details Get RGB values of a specific pixel in the image (BMP only)
     * @param x Horizontal location of a pixel
     * @param y Vertical location of a pixel
     * @return BMPPixel object containing RGB data of a specifed pixel
     * @see int getGrayVal(int x, int y);
     * @see void setGrayVal(int x, int y ,int val);
     * @see void setColor(const BMPPixel& color, int x, int y);
     */
    BMPPixel getColor(int x, int y) const;
    
    /**
     * @brief Set RGB values of a pixel
     * @details Set RGB values of a specific pixel in the image (BMP only)
     * @param color BMPpixel object containing RGB data to set
     * @param x Horizontal location of a pixel
     * @param y Vertical location of a pixel
     * @see int getGrayVal(int x, int y);
     * @see void setGrayVal(int x, int y ,int val);
     * @see BMPPixel getColor(int x, int y) const;
     */
    void setColor(const BMPPixel& color, int x, int y);
    
    /**
     * @brief Get format identifier of the file
     * @details Get the format of the file at a provided file path, derived from file header
     * @param filePath Path to the file on the hard drive
     */
    void checkFileFormat(const char* filePath);
    
    std::string getExportFileName();
    
    /**
     * @brief Get information about the file and its contents
     * @details Get information included in file header and information header of a specified file, usually contains resolution, image size etc.
     * @param filePath Path to the file on the hard drive
     * @param showFileInfo Set true to show file info, set false to not show file info
     */
    void checkFileInfo(const char* filePath, bool showFileInfo);
    
    /**
     * @brief Read whole file
     * @details Read the specified in it's entirety and assign values to right variables
     * @param filePath Path to the file on the hard drive
     */
    void readFile(const char *filePath);
    
    /**
     * @brief Write image data into a specified file
     * @details Write all image data including file and information headers, pixel data etc. into a specified file
     * @param filePath Path to the file on the hard drive
     */
    void exportFile(const char *filePath);
    
    /**
     * @brief Check whether a message wil encrypt/encode
     * @details Check whether a specified message is going to fit inside the file.
     * @param message Message to encrypt.encode
     * @return vector containing a specified message transformed into series of bits
     */
    std::vector<int> checkIfMessageFits(std::string message); 
    
    /**
     * @brief Encrypt/Encode a message into a file
     * @details Encrypt/Encode a specified message after all the required transformations into the file
     * @param message Message to encrypt/encode
     */
    void encode(std::string message);
    
    /**
     * @brief Check whether any message is encrypted/encoded inside a file
     * @details Check whether any message is encrypted/encoded inside a file (This program preceds every encrypted/encoded message with  "####")
     * @return vector containing bits recovered from an image
     */
    std::vector<int> lookForMessage();
    
    /**
     * @brief Decode an encrypted/encoded message
     * @details Decode a message that has been encrypted/encoded inside an image
     */
    void decode();
    
private:
    unsigned int rows;
    unsigned int columns;
    unsigned int maxVal;
    std::string format;
    std::string comment;
    std::vector<int> valuesPGM;
    std::vector<BMPPixel> pixels;
    std::vector<int> headers;
    std::vector<unsigned char> headers2;
    const char * path;
    std::string exportFileName; 
};

#endif /* image_hpp */
