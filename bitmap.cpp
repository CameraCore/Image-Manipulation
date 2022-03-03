#include <iostream>
#include "bitmap.h"


void cellShade(Bitmap& b) {

	cout << "Cell shading the image..." << endl;
	//Call helper function
	b.cellShade();	

}
uint32_t Bitmap::checkClosest(uint32_t & component){

	//Set the pixel's color component to a value depending on how close it is to 0, 128, or 255
	if(component < 64)
		component = 0;

	else if(component >= 64 && component < 192)
		component = 128;

	else if(component > 192)
		component = 255;

	//Return the value after rounding it
	return component;
		

}
void Bitmap::cellShade(){

	//These are variables to hold the pixel's color components once isolated
        uint32_t red;
        uint32_t green;
        uint32_t blue;
	for(int i = 0; i < pixelVector.size() ; i++){
		//IGNORE the alpha pixel
		
		//Bitshift (Push) the isolated component to the rightmost bits, for reading into the rounding function
		//The masks protect the value being isolated from turning into 0
	//	if(compressionMethod == 3){
			red = (pixelVector.at(i) & mask1) >> 24;
			green = (pixelVector.at(i) & mask2) >> 16;
			blue = (pixelVector.at(i) & mask3) >> 8;
	//	}
	/*	else{ //24-bit
			red = (pixelVector.at(i)) >> 16;
			green = (pixelVector.at(i) & 0x00FF00) >> 8;
			blue = (pixelVector.at(i) & 0x0000FF) >> 0;
		}*/
		//Make sure to bitshift it back to the appropriate place, you can then just add the values up like 100 + 10 + 1
	//	if(compressionMethod == 3)
			pixelVector.at(i) = (checkClosest(red) << 24) + (checkClosest(green) << 16) + (checkClosest(blue) << 8);
	//	else //24-bit
	//		pixelVector.at(i) = (checkClosest(red) << 16) + (checkClosest(green) << 8) + (checkClosest(blue) << 0);
	}
}
void grayscale(Bitmap& b) {
	
	cout << "Grayscaling the image..." << endl;
	//Call helper function
	b.grayScale();

}
void Bitmap::grayScale(){
	
	//These are variables to hold the pixel's color components once isolated
        uint32_t red;
        uint32_t green;
        uint32_t blue;
	for(int i = 0; i < pixelVector.size() ; i++){
		//IGNORE the alpha pixel
		
		//Bitshift (Push) the isolated component to the rightmost bits, for reading into the rounding function
		if(compressionMethod == 3){
			red = (pixelVector.at(i) & mask1) >> 24;
			green = (pixelVector.at(i) & mask2) >> 16;
			blue = (pixelVector.at(i) & mask3) >> 8;
		}
		else{ //24-bit
			red = (pixelVector.at(i) ) >> 16;
			green = (pixelVector.at(i) & 0x00FF00) >> 8;
			blue = (pixelVector.at(i) & 0x0000FF) >> 0;
		}
		//Set all of these components to the average
		red = (red + green + blue)/3;
		green = red;
		blue = red;
		//Make sure to bitshift it back to the appropriate place, you can then just add the values up like 100 + 10 + 1
		if(compressionMethod == 3)
			pixelVector.at(i) = (red << 24) + (green << 16) + (blue << 8);
		else //24-bit
			pixelVector.at(i) = (red << 16) + (green << 8) + (blue << 0);
	}

}
void pixelate(Bitmap& b) {
	
	cout << "Pixelating the image..." << endl;
	//Call helper function
	b.pixelate();	

}
void Bitmap::pixelate(){

	//This vector will hold a copy of the data, so that the pixelate isn't continuously applied
	vector<uint32_t> copyVector;

	//32-bit
		for(int i = 0; i < pixelVector.size() ; i++){
			
			copyVector.push_back(pixelVector.at(i));

		}
	uint32_t red = 0;
        uint32_t green = 0;
        uint32_t blue = 0;
	//These values hold the component totals for averaging later
	int rtotal = 0;
	int gtotal = 0;
	int btotal = 0;
	int chunkTotal = 0; //This variable keeps track of how much of the chunk was used
	int total = 0; //This variable holds the total color value of a chunk
/*
	//This is the "matrix" we'll be using to hold a pixelated chunk
       	vector<uint32_t> chunk;
	for(int i = 0; i < 16 * 16; ++i){
		chunk.push_back(1);
	}
	cout << "Size of chunk is: " << chunk.size() << endl;
	
	for(int i = 0; i < pixelVector.size(); ++i){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;
		
		for(int j = 0; j < 16 ; ++j){
			//Check if the pixel in the matrix is out of bounds in the right of the 16 x 16 area
			if(y + (j-16) >= height)
				continue; //Go to the next iteration of the for loop
				
			//This for loop keeps track of the chunk's column
			for(int k = 0; k < 16 ; ++k){

				//Check if the pixel in the matrix is out of bounds in the top of the 16 x 16 area
				if(x + (k-16) >= width){
					continue; //go to the next iteration of the for loop
				}
				//Add the individual pixel's data to the chunk's total
				//j and k will be in bounds since it past the bounds check
				//Remember that j controls the row we're on
				if(compressionMethod == 3){
					red = (pixelVector.at(i + ((j-16)*width) + (k-16)) & mask1) >> 24;
					green = (pixelVector.at(i + ((j-16)*width) + (k-16)) & mask2) >> 16;
					blue = (pixelVector.at(i + ((j-2)*width) + (k-16)) & mask3) >> 8;
				}
				else{ //24-bit
					red = (pixelVector.at(i + ((j-16)*width) + (k-16))) >> 16;
					green = (pixelVector.at(i + ((j-16)*width) + (k-16)) & 0x00FF00) >> 8;
					blue = (pixelVector.at(i + ((j-16)*width) + (k-16)) & 0x0000FF) >> 0;

				}
			
				//The chunk will be from 0 to 255
				//We want to save the color component to the chunk
				rtotal += red * chunk.at((j * 16) + k);
				gtotal += green * chunk.at((j * 16) + k);
				btotal += blue * chunk.at((j * 16) + k);
				//If we got out of bounds, wel will not add 1 to the number of chunk slots to divide by
				chunkTotal += chunk.at((j * 16) + k);
			}
		}

		//Averaging here
		if(compressionMethod == 3)
			total += ((rtotal/chunkTotal) << 24) + ((gtotal/chunkTotal) << 16) + ((btotal/chunkTotal) << 8); 
		else //24-bit
			total += ((rtotal/chunkTotal) << 16) + ((gtotal/chunkTotal) << 8) + ((btotal/chunkTotal) << 0); 

		//AFTER COLLECTING CHUNK DATA, GO OVER AND RECOLOR
		
		//This for loop keeps track of the chunk's rows
		for(int j = 0; j < 16 ; ++j){
			//Check if the pixel in the matrix is out of bounds in the right of the 16 x 16 area
			if(y + (j-16) >= height)
				continue; //Go to the next iteration of the for loop

			//This for loop keeps track of the chunk's column
			for(int k = 0; k < 16 ; ++k){

				//Check if the pixel in the matrix is out of bounds in the top of the 16 x 16 area
				if(x + (k-16) < 0 || x + (k-2) >= width){
					continue; //go to the next iteration of the for loop
				}
				//copyVector.at(i + j + k * width) = total;
			}
		}
		//Reset the total for the next chunk
		total = 0;
		rtotal = 0;
		gtotal = 0;
		btotal = 0;
		chunkTotal = 0;

		//If i mod height is 0, that means the we've reached the last chunk on the horizontal axis, so we need to add width to wrap around
//		if(x == 0)
//			i+=(width * 15);
	}
	//Set the original vector to the copy vector
	pixelVector = copyVector;
*/

	//This for loop keeps track of the 16 x 16 chunk we're on
	//We want the size of the vector minus the last row near the top since the chunk will fill it in
	for(int i = 0; i < pixelVector.size(); i+=16 ){
		
		//This for loop keeps track of the chunk's column
		for(int j = 0; j < 16 ; ++j){
		
			//This for loop keeps track of the chunk's row
			for(int k = 0; k < 16; ++k){
			
				//Add the individual pixel's data to the chunk's total
				//i is the chunk
				//j is the chunk column
				//k is the chunk row, which goes up by adding width * row number
				if(compressionMethod == 3){
					red = (pixelVector.at(i + j + (k * width)) & mask1) >> 24;
					green = (pixelVector.at(i + j + (k * width)) & mask2) >> 16;
					blue = (pixelVector.at(i + j + (k * width)) & mask3) >> 8;
				}
				else{ //24-bit
					red = (pixelVector.at(i + j + (k * width))) >> 16;
					green = (pixelVector.at(i + j + (k * width)) & 0x00FF00) >> 8;
					blue = (pixelVector.at(i + j + (k * width)) & 0x0000FF) >> 0;
				}

				rtotal += red;
				gtotal += green;
				btotal += blue;

			}
		}

		//Averaging here
		if(compressionMethod == 3)
			total += ((rtotal/256) << 24) + ((gtotal/256) << 16) + ((btotal/256) << 8); 
		else //24-bit
			total += ((rtotal/256) << 16) + ((gtotal/256) << 8) + ((btotal/256) << 0); 

		//AFTER COLLECTING CHUNK DATA, GO OVER AND RECOLOR
		
		//This for loop keeps track of the chunk's rowa
		for(int j = 0; j < 16 ; ++j){
		
			//This for loop keeps track of the chunk's column
			for(int k = 0; k < 16; ++k){
					pixelVector.at( i + j + (k * width) ) = total; //Average value is the total/chunk size

			}
		}
		//Reset the total for the next chunk
		total = 0;
		rtotal = 0;
		gtotal = 0;
		btotal = 0;

		//If i mod height is 0, that means the we've reached the last chunk on the horizontal axis, so we need to add width to wrap around
		if((i + 16) % width == 0)
			i+=(width * 15);
	}

}
void blur(Bitmap& b) {

	cout << "Gaussian blurring the image..." << endl;
	//Call helper function
	b.gaussian();
}
void Bitmap::gaussian(){
	//This is the "matrix" we'll be using
       	vector<uint32_t> gaussianVector {1, 4,  6,  4,  1,
					 4, 16, 24, 16, 4,
					 6, 24, 36, 24, 6,
					 4, 16, 24, 16, 4,
					 1, 4,  6,  4,  1};
	//This vector will hold a copy of the data, so that the blur isn't continuously applied
	vector<uint32_t> copyVector;

	//32-bit
	//if(compressionMethod == 3)
		for(int i = 0; i < pixelVector.size() ; i++){
			
			copyVector.push_back(pixelVector.at(i));

		}
	/*else{ //24-bit
		for(int i = 0; i < pixelVector.size() ; i++){
			
			if(i == 0)
				cout << hex << "The value in pixel is: " << pixelVector.at(i) << endl;

			copyVector.push_back(pixelVector.at(i));
			if(i == 0)
				cout << hex << "The value in copy is: " << copyVector.at(i) << endl;


		}
	}*/

	uint32_t red = 0;
        uint32_t green = 0;
        uint32_t blue = 0;
	//These values hold the component totals for later
	uint32_t rtotal = 0;
	uint32_t gtotal = 0;
	uint32_t btotal = 0;
	uint32_t gausstotal = 0; //This variable holds the gauss sum to divide a pixel by
	uint32_t total = 0; //This variable holds the total color value of a chunk


	//This for loop keep's track of the chunk we're on
	//Basically, every single pixel on the image, and then an area around the pixel
	for(int i = 0; i < pixelVector.size(); ++i){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		
		//This for loop keeps track of the chunk's row
		for(int j = 0; j < 5 ; ++j){

			//Check if the pixel in the matrix is out of bounds in the left or ride side of the 5 x 5
			if(y + (j-2) < 0 || y + (j-2) >= height)
				continue; //Go to the next iteration of the for loop if t
			
			//This for loop keeps track of the chunk's column
			for(int k = 0; k < 5 ; ++k){

				//Check if the pixel in the matrix is out of bounds in the bottom or top of the 5 x 5
				if(x + (k-2) < 0 || x + (k-2) >= width){
					continue; //go to the next iteration of the for loop
				}
				//Add the individual pixel's data to the chunk's total
				//j and k will be in bounds since it past the bounds check
				//Remember that j controls the row we're on
				if(compressionMethod == 3){
					red = (pixelVector.at(i + ((j-2)*width) + (k-2)) & mask1) >> 24;
					green = (pixelVector.at(i + ((j-2)*width) + (k-2)) & mask2) >> 16;
					blue = (pixelVector.at(i + ((j-2)*width) + (k-2)) & mask3) >> 8;
				}
				else{ //24-bit
					red = (pixelVector.at(i + ((j-2)*width) + (k-2))) >> 16;
					green = (pixelVector.at(i + ((j-2)*width) + (k-2)) & 0x00FF00) >> 8;
					blue = (pixelVector.at(i + ((j-2)*width) + (k-2)) & 0x0000FF) >> 0;

				}
			
				//The Gaussian vector will be from 0 to 24
				//We want to multiply each value by the value in the gauss vector before dividing later
				rtotal += red * gaussianVector.at((j * 5) + k);
				gtotal += green * gaussianVector.at((j * 5) + k);
				btotal += blue * gaussianVector.at((j * 5) + k);
				gausstotal += gaussianVector.at((j * 5) + k);

			}

		}
			//Time to divide the color values by the 1/256 in the gaussian matrix (Or less if there were out of bounds values)
			if(compressionMethod == 3)
				total = ((rtotal/gausstotal) << 24) + ((gtotal/gausstotal) << 16) + ((btotal/gausstotal) << 8); 
			else //24-bit
				total = ((rtotal/gausstotal) << 16) + ((gtotal/gausstotal) << 8) + ((btotal/gausstotal) << 0);

			//Set the current pixel in the whole image to this
			//
			copyVector.at(i) = total;	
			//Reset totals
			rtotal = 0;
			gtotal = 0;
			btotal = 0;
			gausstotal = 0;

	}
	//Set the original vector to the new vector
	pixelVector = copyVector;

}
void rot90(Bitmap& b) {

	cout << "Rotating the image by 90 degrees..." << endl;
	//Call helper function
	b.rotate90();

}
void Bitmap::rotate90(){
	//This vector will hold a copy of the data, so that the rotation isn't continuously applied
	vector<uint32_t> copyVector;

	for(int i = 0; i < pixelVector.size() ; i++){
		
		copyVector.push_back(pixelVector.at(i));

	}

	//Basically, we want to redraw the vector using the original as a reference
	//We draw the new vector from the bottom left of the image to the top right
	//While referencing pixels from the bottom right to the top left
	for(int i = 0; i < pixelVector.size(); i++){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		//These will be the new x and y values after putting them through
		//the matrix for rotating by 90 degrees
		int newX = y;
		int newY = width - (x + 1);
	
		//Multiply newY by width to reclaim the index
		copyVector.at(newX + newY * height) = pixelVector.at(i);
	}

	//Set the original vector to the new vector
	pixelVector = copyVector;
	
	
	//Change the header so that the width and height are swapped during the rotation
	//This should not affect images that have the same width and height
	uint32_t temp = 0; //This holds the width so the height can reference it
	temp = width;
	width = height;
	height = temp;
	
}
void rot180(Bitmap& b) {

	cout << "Rotating the image by 180 degrees..." << endl;
	//It's just rotating 90 degrees twice
	b.rotate90();
	b.rotate90();
}
void rot270(Bitmap& b) {

	cout << "Rotating the image by 270 degrees..." << endl;
	//It's just rotating 90 degrees thrice
	b.rotate90();
	b.rotate90();
	b.rotate90();
}

void flipv(Bitmap& b) {
	
	cout << "Flipping the image vertically..." << endl;
	//Call the helper function
	b.mirrorv();

}
void Bitmap::mirrorv(){
	//This vector will hold a copy of the data, so that the mirroring isn't continuously applied
	vector<uint32_t> copyVector;

	for(int i = 0; i < pixelVector.size() ; i++){
		
		copyVector.push_back(pixelVector.at(i));

	}
	for(int i = 0; i < pixelVector.size(); i++){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		int newX = x; //Redundant , but used here for matrix usage
		int newY = -y + height - 1;
			
			//All of the y values would be flipped and have height readded to avoid going out of bounds
			//All of the x values stay in the same place
			copyVector.at(i) = pixelVector.at( newX + newY * width );
			

	}
	//Set the original vector to the new vector
	pixelVector = copyVector;
	
}

void fliph(Bitmap& b) {
	cout << "Flipping the image horizontally..." << endl;
	//Call the helper function
	b.mirrorh();
}
void Bitmap::mirrorh(){
	//This vector will hold a copy of the data, so that the mirroring isn't continuously applied
	vector<uint32_t> copyVector;

	for(int i = 0; i < pixelVector.size() ; i++){
		
		copyVector.push_back(pixelVector.at(i));

	}
	for(int i = 0; i < pixelVector.size(); i++){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		int newX = -x + width - 1;
		int newY = y; //Redudant, but used here for the matrix usage	
			
			//We just flip the x values and add width - 1 for the offset
			//All of the y values stay at the same height * width for row control
			copyVector.at(i) = pixelVector.at( newX + newY * width );

	}
	//Set the original vector to the new vector
	pixelVector = copyVector;
}
void flipd1(Bitmap& b) {

	cout << "Flipping the image y=x..." << endl;
	//This is just a combination of rotation and mirror
	b.rotate90();
	b.mirrorh();

}
void flipd2(Bitmap& b) {

	cout << "Flipping the image x=y..." << endl;
	//This is just a combination of rotation and mirror
	b.rotate90();
	b.mirrorv();

}
void scaleUp(Bitmap& b) {

	cout << "Scaling the image up..." << endl;
	//Call the helper function
	b.scaleUp();

}
void Bitmap::scaleUp(){

	//This vector will hold a copy of the data, so that the scaling isn't continuously applied
	vector<uint32_t> copyVector;

	for(int i = 0; i < pixelVector.size() ; i++){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		copyVector.push_back(pixelVector.at(i));
		copyVector.push_back(pixelVector.at(i));
		//Push an additional two pixels for every row 
		if(x == width - 1){
			for(int j = 0; j < width ; j++){
				copyVector.push_back(pixelVector.at(j + width * y));
				copyVector.push_back(pixelVector.at(j + width * y));
			}
		}

	}
	//Set the original vector to the new vector
	pixelVector = copyVector;
	
	//Make sure to change the width and height and size of the file
	width = width * 2;
	height = height * 2;
	size = size * 2;
	sizeRaw = sizeRaw * 2;
}
void scaleDown(Bitmap& b) {

	cout << "Scaling the image down..." << endl;
	//Call the helper function
	b.scaleDown();

}
void Bitmap::scaleDown(){

	//This vector will hold a copy of the data, so that the scaling isn't continuously applied
	vector<uint32_t> copyVector;

	for(int i = 0; i < pixelVector.size() ; i++){
		//Horizontal position of the image
		//Once x reaches the image width, x turns to 0
		int x = i % width;
		//Vertical position of the image
		//Once y reaches the image width, y increases by 1
		int y = i / width;

		//We are skipping every other row and column
		if(x % 2 == 0 && y % 2 == 0)
		copyVector.push_back(pixelVector.at(i));

	}
	//Set the original vector to the new vector
	pixelVector = copyVector;
	
	//Make sure to change the width and height and size of the file
	width = width / 2;
	height = height / 2;
	size = size / 2;
	sizeRaw = sizeRaw / 2;
}


istream& operator>>(istream& in, Bitmap& b) {
	
	//Header 1	
	in.read((char*)b.identifier, 2);    //Read in the first two characters NOTE: character arrays do not need to be referenced with &
	cout << "Identifier contains: " << b.identifier << endl;
	if(strncmp(b.identifier, "BM", 2)){ //Check if the first two characters are indeed BM
					    //Strncmp should return a 0 if it matches, and wont throw an exception

		throw (Exception(" Invalid Bitmap file: Expected BM")); //If tag isn't BM, throw an exception
	}
	in.read((char*)&b.size, 4);
	cout << "The file size is: " << b.size << endl;
	in.read((char*)b.garbage,4);
	in.read((char*)&b.offset,4);
	cout << "The file offset is: " << b.offset << endl;

	//Header 2
	in.read((char*)&b.secondSize, 4);
	cout << "The second header size is: " << b.secondSize << endl;
	in.read((char*)&b.width, 4);
	cout << "The height is: " << b.width << endl;
	in.read((char*)&b.height, 4);
	cout << "The width is: " << b.height << endl;
	in.read((char*)&b.colorPlanes, 2);
	cout << "The color planes is: " << b.colorPlanes << endl;
	if(b.colorPlanes != 1){
		throw (Exception(" Invalid Bitmap file: Color Planes is not 1"));} //Color Plane amount should alwaus be 1
	in.read((char*)&b.colorDepth, 2);
	cout << "The color depth is: " << b.colorDepth << endl;
	in.read((char*)&b.compressionMethod, 4);
	if(b.compressionMethod != 0 && b.compressionMethod != 3){
		throw (Exception(" Invalid Bitmap file: Expected Compression Method 0 or 3"));} //Compression method should only be 0 or 3
	cout << "The compression method is: " << b.compressionMethod << endl;
	in.read((char*)&b.sizeRaw, 4);
	cout << "The raw bitmap size is: " << b.sizeRaw << endl;
	in.read((char*)&b.horiRes, 4);
	in.read((char*)&b.vertRes, 4);
	cout << "Horizontal and Vertical resolutions are: " << b.horiRes << " and " << b.vertRes << endl;
 	in.read((char*)&b.paletteColors, 4);
	in.read((char*)&b.importantColors, 4);

	//Masks
	//NOTE: These would only be included if compression method is 3!
	if(b.compressionMethod == 3){
		in.read((char*)&b.mask1, 4);
		in.read((char*)&b.mask2, 4);
		in.read((char*)&b.mask3, 4);
		in.read((char*)&b.mask4, 4);
		in.read((char*)&b.maskOrder, 4);
		in.read((char*)b.colorSpace, 64);
	}

	//Reading in the pixel data
	//Set the vector's size to height x width of the image, because that's how many pixels there are
	
	uint32_t temp;
	uint32_t pad;

	for(int i = 0; i < (b.height * b.width) ; ++i){

		//32-bit
		if(b.colorDepth == 32) {

			in.read((char*)&temp, 4);
			b.pixelVector.push_back(temp);
		}
		//24-bit 
		else { //b.colorDepth == 24
			
				
			in.read((char*)&temp, 3);
			temp << 8;
			temp += 0xFF;
			//if(i == 0)
			//cout <<  "Temp currently has: " << temp << endl;
			//in.read((char*)&pad, 1);
			b.pixelVector.push_back(temp);
		}


	}
	cout << "The vector's current size is " << b.pixelVector.size() << endl;


	return in;
}

ostream& operator<<(ostream& out, const Bitmap& b) {

	//Header 1
	out.write((char*)b.identifier, 2);
	out.write((char*)&b.size, 4);
	out.write((char*)b.garbage, 4);
        out.write((char*)&b.offset, 4);

	//Header 2
       	out.write((char*)&b.secondSize, 4);
       	out.write((char*)&b.width, 4);
       	out.write((char*)&b.height, 4);
	out.write((char*)&b.colorPlanes, 2);
	out.write((char*)&b.colorDepth, 2);
	out.write((char*)&b.compressionMethod, 4);
       	out.write((char*)&b.sizeRaw, 4);
       	out.write((char*)&b.horiRes, 4);
	out.write((char*)&b.vertRes, 4);
       	out.write((char*)&b.paletteColors, 4);
       	out.write((char*)&b.importantColors, 4);

	//Masks
	//NOTE: These would only be included if compression method is 3!
	if(b.compressionMethod == 3){
		out.write((char*)&b.mask1, 4);
		out.write((char*)&b.mask2, 4);
		out.write((char*)&b.mask3, 4);
		out.write((char*)&b.mask4, 4);
		out.write((char*)&b.maskOrder, 4);
		out.write((char*)b.colorSpace, 64);
	}

	//Pixel Data
	char padding[3] = {0, 0, 0};		
	for(int i = 0; i < (b.width * b.height) ; ++i){

		//32-bit
		if(b.colorDepth == 32)	
			out.write((char*)&b.pixelVector.at(i), 4);
		//24-bit
		else {//b.colorDepth == 24
				uint32_t temp = b.pixelVector.at(i);
			        temp >> 8;

				//cout << hex << "Pixel Vector is: " << b.pixelVector.at(i) << " temp is: " << temp << endl;
				
				out.write((char*)&temp, 3);
				out.write(padding, 3);
				//out.write((char*)&b.pixelVector.at(i), 1);
				//out.write(padding,1);
		}

	}

	return out;
} 

//This is the contructor
Bitmap::Bitmap() {
	//Allocate the character arrays
	identifier = new char [3]; //this field should only hold 2 characters plus the null character	
	garbage = new char [5]; //this field should only hold 4 characters plus the null character
	colorSpace = new char [65]; //this field should only hold 64 characters plus the null character
	/* unimplemented */}
//UA START
Bitmap::~Bitmap() {
	//Deallocate the character arrays
	identifier = nullptr;
	delete [] identifier;
	garbage = nullptr;
	delete [] garbage;
	colorSpace = nullptr;
	delete [] colorSpace;
}
//UA END
