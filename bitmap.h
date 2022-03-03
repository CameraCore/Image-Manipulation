#include <iostream>
//User additions here
#include <cstring>
#include <vector>
//end user additions
using namespace std;

//

//UA START
//An exception class for displaying specific messages
class Exception
{
	private:
		string message; //Error message
	public:
		string getMessage() const {return(message);} //Function to return a message

		//Constructor	
		Exception(const string& message) : message(message){};
		//Destructor
		~Exception(){};
		//Note: these are inline to avoid having to add them to the other cpp files.	
};
//UA END

class Bitmap
{
private:
    friend istream& operator>>(istream& in, Bitmap& b);
    friend ostream& operator<<(ostream& out, const Bitmap& b);
    //UA START
    //These are the bitmap header's fields

    //First Header
    //Holds the first two characters to confirm that BM is there 
    //NOTE: using character arrays here instead of vectors because these are set amounts that don't need to grow
    //Putting these fields into a vector would take more lines of code
    char * identifier; //Should be BM
    uint32_t size;
    char * garbage;
    uint32_t offset;

    //Second Header
    uint32_t secondSize;
    int width;
    int height;
    uint16_t colorPlanes; //Should always be 1
    uint16_t colorDepth; //Can either be 24(RGB) or 32(RGBA)
    uint32_t compressionMethod; //Should always be 0 or 3
    uint32_t sizeRaw;
    uint32_t horiRes; //These resolutions will always be 2835
    uint32_t vertRes; 
    uint32_t paletteColors; //These following two aren't used since palettes arent used for this project
    uint32_t importantColors;

    //Masks
    //Determines which RGBA values a pixel has
    uint32_t mask1; //Always the red mask
    uint32_t mask2; //Always the green mask
    uint32_t mask3; //Always the blue mask
    uint32_t mask4;
    uint32_t maskOrder; //The order of the masks
    char * colorSpace; //Unused in this project 

    //Pixel Data
    vector<uint32_t> pixelVector;


    //UA END

public:
    Bitmap(); //This is the constructor
    //Bitmap(const Bitmap&); //
    //Bitmap operator=(const Bitmap&); //Copy Constructor
    //Bitmap(Bitmap&&); //
    ~Bitmap(); //This is the destructor

    //UA START
    void cellShade(); //Rounds the color value of a pixel to whichever it's closest to 0, 128, or 255 
	uint32_t checkClosest(uint32_t & component);	//Checks which color the pixel component is closest too, then return the rounded value
    //UA END
    void grayScale(); //Averages the color values of a pixel, turning it into a gray color
    void pixelate(); //Splits the image into 16 x 16 blocks, and turns them into the average color
    void gaussian(); //Gaussian blurs the image by using a matrix
    void rotate90(); //Rotates the image 90 degrees clockwise
    void mirrorh(); //Mirrors (flips) the image horizontally across the y axis
    void mirrorv(); //Mirross (flips) the image vertically across the x axis
    void scaleUp(); //Doubles the size of the image
    void scaleDown(); //Halves the size of the image

};

void cellShade(Bitmap& b);
void grayscale(Bitmap& b);
void pixelate(Bitmap& b);
void blur(Bitmap& b);
void rot90(Bitmap& b);
void rot180(Bitmap& b);
void rot270(Bitmap& b);
void flipv(Bitmap& b);
void fliph(Bitmap& b);
void flipd1(Bitmap& b);
void flipd2(Bitmap& b);
void scaleUp(Bitmap& b);
void scaleDown(Bitmap& b);
