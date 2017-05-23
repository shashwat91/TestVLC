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
  threshold(blurred_image, otsu, 0, 255,  CV_THRESH_OTSU);

  vector<vector<Point> > contours;

  // 	Original method by using contors.
  // void findContours(InputOutputArray image, OutputArrayOfArrays contours, int mode, int method, Point offset=Point())
	/*Mat cimg = otsu;
	Mat clone = input.clone();
	vector<Point2i> center;
	vector<int> radius;
	findContours(cimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Scalar red(255,0,0);
	drawContours( clone, contours, -1, red,5); //--Drawing detected contors
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


	returnMatrix.resize(count2);

	for( int i = 0; i < count2; i++)
	{
	circle(input, center[i], radius[i],red, 5);

	returnMatrix[i][0] = center[i].x;
	returnMatrix[i][1] = center[i].y;
	returnMatrix[i][2] = radius[i];
	}*/


  //	Try-1 using find circular grids
  // bool findCirclesGrid(InputArray image, Size patternSize, OutputArray centers, int flags=CALIB_CB_SYMMETRIC_GRID, const Ptr<FeatureDetector>& blobDetector=new SimpleBlobDetector() )
/*  Size patternsize(1,1);
	vector<Point2f> centers;
	cout<<findCirclesGrid(input, patternsize, centers )<<"	"<<centers.size()<<endl;
*/

  //	Try-2 using hough circles
  // void HoughCircles(InputArray image, OutputArray circles, int method, double dp, double minDist, double param1=100, double param2=100, int minRadius=0, int maxRadius=0 )
/*	vector<Vec3f> circles;
	GaussianBlur( clone, clone, cv::Size(9, 9), 2, 2 );
	HoughCircles(clone, circles, CV_HOUGH_GRADIENT, 2, 100, 100, 100, 1, clone.rows/2 );
	cout<<"no of hough circles::"<<circles.size()<<endl;
	// Plotting circles detected by HoughCircles functions.
	std::vector<cv::Vec3f>::const_iterator itc= circles.begin();
	while (itc!=circles.end())
	{

	cv::circle(clone, Point((*itc)[0], (*itc)[1]),	(*itc)[2],	Scalar(255,0,0),	2);
	++itc;
	}
*/

  //	Try-3 using curve approximation
  // void approxPolyDP(InputArray curve, OutputArray approxCurve, double epsilon, bool closed)
/*  vector<vector<Point> > points;
	points.resize(contours.size());
	Scalar red(255,0,0);
	for(int i=0; i<contours.size(); ++i)
	{
		approxPolyDP(Mat(contours[i]), points[i], 50, true);
		cout<<"curve ["<<i+1<<"] = "<<points[i].size()<<endl;
	}
	drawContours( clone, points, -1, red,5);
*/

  //	Working Method to detect circles using average of centers
	/*findContours(otsu, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Scalar red(255,0,0);
	size_t count = contours.size();
	float rMax=0;
	Point2f cAvg;
	cAvg.x=0;cAvg.y=0;

	for( size_t i=0; i < count; i++)
	{
		Point2f c;
		float r;
		minEnclosingCircle( contours[i], c, r);
		cout<<"circle i :: "<<c.x<<" , "<<c.y<<" - "<<r<<endl;
		cAvg.x +=c.x;
		cAvg.y +=c.y;
		if(r>rMax)
			rMax = r;
	}

	if(count!=0)
	{
		cAvg.x = cAvg.x/count;
		cAvg.y = cAvg.y/count;
		cout<<"Average circle :: "<<cAvg.x<<" , "<<cAvg.y<<endl;
		circle(input, cAvg, rMax,red, 5);
		returnMatrix.resize(1);
		returnMatrix[0][0] = cAvg.x;
		returnMatrix[0][1] = cAvg.y;
		returnMatrix[0][2] = rMax;
	}*/

  //	New method of detection of circle using weighted avg of countors
	findContours(otsu, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Scalar red(255,0,0);
	size_t count = contours.size();
	vector<Moments> mu(count);
	vector<Point2f> mc(count);
	unsigned long totalArea=0;
	Point2f center;
	int rMax=0;
	float r;
	

	for( size_t i=0; i < count; i++)
	{
		minEnclosingCircle( contours[i], center, r);
		if(r > rMax)
			rMax=r;
		mu[i] = moments( contours[i], false );
		if(mu[i].m00 == 0)
			continue;
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		// cout<<"mass center countour "<<i+1<<" : "<<mc[i]<<"  , area :: "<<mu[i].m00<<endl;
		totalArea+=mu[i].m00;
	}
	center.x=0; center.y=0;
	for(size_t i=0; i < count; ++i)
	{
		float per = mu[i].m00 / totalArea;
		// cout<<i+1<<"  = "<<per<<endl;
		center.x += mc[i].x * per;
		center.y += mc[i].y * per;
	}
	cout<<"center :: "<<center<<endl;

	if(count!=0)
	{
		circle(input, center, rMax,red, 5);
		returnMatrix.resize(1);
		returnMatrix[0][0] = center.x;
		returnMatrix[0][1] = center.y;
		returnMatrix[0][2] = rMax;
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
		// cout<<peaks[i]<<" , "<<falls[i-1]<<" :: "<<dif;
		if(dif > 3*max_peak)
		{
			nPackets.push_back(i);
			// cout<<"		--- StartBit";
		}
		// cout<<endl;
	}
	
	// To print all rising and falling edges with difference.
	// int j=0;
	// for(unsigned int i=0; i<peaks.size(); ++i)
	// {
	// 	cout<<"Peak:: "<<setw(4)<<peaks[i]<<"\tFall:: "<<setw(4)<<falls[i]<<"\tDiff:: "<<setw(4)<<diff[i];
	// 	if(peaks[i] == peaks[nPackets[j]])
	// 	{
	// 		cout<<" -- Start bit";
	// 		j++;
	// 	}
	// 	cout<<endl;
	// }
	// cout<<max_peak<<" , "<<min_peak<<" , "<<th_peak<<endl;

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
