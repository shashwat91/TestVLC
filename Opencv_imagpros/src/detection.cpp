#include "detection.hpp"

#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

void detector(Mat input, vector< vector<int> > &returnMatrix)
{
  Mat blurred_image;
  blur(input, blurred_image, Size(100, 100));

  Mat otsu;
  threshold(blurred_image, otsu, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  Mat cimg = input;

  vector<vector<Point> > contours;
  vector<Point2i> center;
  vector<int> radius;

  findContours(otsu.clone(), contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

  size_t count = contours.size();

  for( size_t i=0; i < count; i++)
  {
	  Point2f c;
	  float r;
	  minEnclosingCircle( contours[i], c, r);

	  if (r >= 50)
	  {
		  center.push_back(c);
		  radius.push_back(r);
	  }
  }
  int count2 = center.size();
  cv::Scalar red(255,255,255);

  returnMatrix.resize(count2);

  for( int i = 0; i < count2; i++)
  {
	  circle(input, center[i], radius[i], red, 3);
	  returnMatrix[i][0] = center[i].x;
	  returnMatrix[i][1] = center[i].y;
	  returnMatrix[i][2] = radius[i];
  }
}
  
Mat clahe(Mat input)
{
	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(10);
	clahe->setTilesGridSize(Size(8,8));

	Mat adaptive_equalized;
	clahe->apply(input,adaptive_equalized);

	return adaptive_equalized;
}

Mat adaptiveThreshold(Mat input)
{
	Mat adaptive;
	adaptiveThreshold(input, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 105, 2);
	return adaptive;
}

int avoidBlobOffset(Mat input, int centerRadius[3])
{
	float factor = 1;
	int newRadius = ceil(centerRadius[2] * factor);
	int top = centerRadius[1] - newRadius;
	int bottom = centerRadius[1] + newRadius;

	if(top < 0){
		top = 0;
	}
	if(bottom > input.rows){
		bottom = input.rows;
	}

	int column = centerRadius[0];

	while(1)
	{
		vector<int> pixels;
		vector<int> peaks;
		vector<int> falls;
		for(int i = top ; i< bottom ; i++)
		{
			pixels.push_back(input.at<unsigned char>(i, column));
		}

		for (unsigned int i = 0; i < pixels.size()-1; i++)
		{
			if(pixels[i] == 0 && pixels[i+1] == 255)
				peaks.push_back(i);
			else if(pixels[i] == 255 && pixels[i+1] == 0 && peaks.size() !=0)
				falls.push_back(i);
		}

		int noBlob = 0;

		for (unsigned int i = 0; i < falls.size(); i++)
		{
			int dif = falls[i] - peaks[i];
			if (dif > 16*3)
			{ //freq = 3000 -> 3*8
				column = column - 1;
				noBlob = 1;
			}
		}

		if(noBlob == 0)
			break;
	}

	return centerRadius[0] - column;
}

void getCorrectedPixelsOffset(Mat input, int centerRadius[3], int offset, vector<int>& correctPixels )
{
	float factor = 1;
	int newRadius = ceil(centerRadius[2] * factor);

	int top = centerRadius[1] - newRadius;
	int bottom = centerRadius[1] + newRadius;

	if(top < 0)
	{
		top = 0;
	}
	
	if(bottom > input.rows)
	{
		bottom = input.rows;
	}

	for(int i = top ; i< bottom ; i++)
	{
		correctPixels.push_back(input.at<unsigned char>(i, centerRadius[0] - offset) );
	}
}

void decodeBits(vector<int>& inputPixels, vector<int>& detectedBits)
{
	vector<int> peaks;
	vector<int> falls;

	int dif;
	
	for (unsigned int i = 0; i < inputPixels.size()-1; i++)
	{
		if(inputPixels[i] == 0 && inputPixels[i+1] == 255)
		{
			peaks.push_back(i);
		}
		else if(inputPixels[i] == 255 && inputPixels[i+1] == 0 && peaks.size() !=0)
		{
			falls.push_back(i);
		}
	}

	if(peaks.size() > falls.size())
		peaks.erase(peaks.begin() + peaks.size() - 1);

	int j = 1;
	int firstPreamble=0;
	int secondPreamble=0;

	for (unsigned int i = 0; i < peaks.size(); i++)
	{
		dif = falls[i] - peaks[i];
		if ( dif >= 2.5 *16 )
		{ //freq = 3000 -> 2.5*8
			if( j == 1 )
			{
				if(i == 0)
					continue;
				firstPreamble = i;
				j++;
			}
			else
			{
				secondPreamble = i;
				break;
			}
		}
	}

	if(secondPreamble == 0)
		return;

	j = 1;
	int addZero = 0;

	while(true)
	{
		dif = peaks[firstPreamble +j] - falls[firstPreamble + j -1];
		if(dif > 1.3 * 16 )
		{ //freq = 3000 -> 2.5*8
			if(j == 1 || (firstPreamble + j == secondPreamble))
			{
				detectedBits.push_back(0);
			}
			else
			{	
				detectedBits.push_back(0);
				detectedBits.push_back(0);
			}
		}
		else
		{
			if((j != 1 && firstPreamble + j != secondPreamble) || addZero == 1)
			{
				detectedBits.push_back(0);
			}
		}
		
		if (firstPreamble + j == secondPreamble)
			break;

		dif = falls[firstPreamble +j] - peaks[firstPreamble + j];

		if(dif > 1.5 * 16 )
		{ //freq = 3000 -> 2.5*8
			detectedBits.push_back(1);
			detectedBits.push_back(1);
			if(firstPreamble + j +1 == secondPreamble)
				addZero = 1;
		}
		else
			detectedBits.push_back(1);
		j++;
	}
}

void decodeBits_encoding1(){}

void decodeBits_encoding2(vector<int>& inputPixels, vector<int>& detectedBits)
{
	vector<int> peaks;
	vector<int> falls;
	vector<int> diff;
	int max_peak = 0;
	int min_peak = inputPixels.size();
	int th_peak;
	int dif;

	//Deceting rising and falling edges of data
	for (unsigned int i = 0; i < inputPixels.size()-1; i++)
	{
		if(inputPixels[i] == 0 && inputPixels[i+1] == 255)
		{
			peaks.push_back(i);
		}
		else if(inputPixels[i] == 255 && inputPixels[i+1] == 0 && peaks.size() !=0)
		{
			falls.push_back(i);
			dif = falls.back()-peaks.back();
			if(max_peak < dif)
				max_peak = dif;
			if(min_peak > dif)
				min_peak = dif;
			diff.push_back(dif); //May not be needed
		}
	}
	th_peak = (max_peak + min_peak)/2;
	//Removing last rising edge or half bit
	if(peaks.size() > falls.size())
		peaks.pop_back();
	//Deceting  silent zone or startbit of new packet
	vector<int> nPackets;
	for(unsigned int i=1; i<peaks.size(); ++i)
	{
		dif = peaks[i] - falls[i-1];
		cout<<peaks[i]<<" , "<<falls[i-1]<<" :: "<<dif;
		if(dif > 3*max_peak)
		{
			nPackets.push_back(i);
			cout<<"		--- StartBit";
		}
		cout<<endl;
	}
	
	// To print all rising and falling edges with difference.
	int j=0;
	for(unsigned int i=0; i<peaks.size(); ++i)
	{
		cout<<"Peak:: "<<setw(4)<<peaks[i]<<"\tFall:: "<<setw(4)<<falls[i]<<"\tDiff:: "<<setw(4)<<diff[i];
		if(peaks[i] == peaks[nPackets[j]])
		{
			cout<<" -- Start bit";
			j++;
		}
		cout<<endl;
	}
	cout<<max_peak<<" , "<<min_peak<<" , "<<th_peak<<endl;

	//Decoding bits from rising and falling edges
	for(unsigned i=1; i<peaks.size()-1; ++i)
	{
		if((peaks[i] - falls[i-1]) > (3*max_peak))
			cout<<"/";

		if(diff[i] > th_peak)
			cout<<"1";
		else//(diff[i] <= th_peak)
			cout<<"0";
	}
	cout<<endl;
}
