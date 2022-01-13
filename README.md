# What is Steg_cpp
Steg_cpp is a simple c++ steganography program allowing users to do the following:
1. Check basic information about an image file (resolution, file size etc.).
2. Hide messages inside images.
3. Read messages hidden in images.

**!!! PLEASE NOTE !!!** 

<code>THE OUTPUT IMAGE IS SAVED IN THE SAME FOLDER WHERE TERMINAL IS CURRENTLY OPEN E.G. IF YOU CD INTO **C:\Users\Username\Documents** AND RUN THIS PROGRAM THE OUTPUT FILE WILL BE SAVED AS **C:\Users\Username\Documents\ENCODED.extension**</code>

This program was created as a final project from the first semester of my C++ programming course at Polish-Japanese Institute of Technology.

# Requirements
### Windows:
**g++** or **gcc** installed

### MacOS:
**g++** or **clang++** installed

### Linux (NOT TESTED):
**g++** or **gcc** installed

To check if you have g++, gcc or clang++ installed simply start cmd or terminal and type in: <code>g++ --version</code> or <code>gcc --version</code> or <code>clang++ --version</code>

# Usage
## Supported Filetypes:
.png

.bmp

.ico (Sometimes works, sometimes doesn't. Requires fixing).

.pgm

## Available commands
<code>-i</code> - to check basic information about the file. Requires providing the path to the file.

example:
```
Steg_cpp -i "C:/Users/<username>/Pictures/image.png"
```

<code>-e</code> - to hide a message inside an image. Requires providing the path to the file and a message to be hidden.

example
```
Steg_cpp -e "C:/Users/<username>/Pictures/image.png" "Message to hide"
```

<code>-d</code> - to read a message that has been previously hidden. Requires providing the path to the file.

example
```
Steg_cpp -d "C:/Users/<username>/Pictures/image.png"
```

<code>-c</code> - to check whether the message will fit inside the image. Requires providing the path to the file and a message to be hidden. Unnecessary as -e automatically does this check but required by my professor.

```
Steg_cpp -c "C:/Users/<username>/Pictures/image.png" "Message to hide"
```

<code>-h</code> - to display the help box containing all the commands and error codes.
```
Steg_cpp -h
```
# Plans for the future:
I am planning on expanding this project. Below is a simple checklist showing what has been done and what is yet to come.

### Done:
- [X] - Support for PNG files
- [X] - Support for BMP files
- [X] - Support for PGM files

### To do:
- [ ] - Fixing ICO support
- [ ] - Support for JPEG files
- [ ] - Support for GIF files
- [ ] - Support for TIFF files
- [ ] - Check file info should tell the user if there is a message hidden
- [ ] - Program should support multiple flags e.g <code>-i -e "path" "message"</code> or <code>-ie "path" "message"</code> should show info about the file and hide the message in a file. 
- [ ] - Code cleanup
- [ ] - Logic optimization
