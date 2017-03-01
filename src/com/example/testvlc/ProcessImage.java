package com.example.testvlc;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Vector;

import android.media.Image;
import android.util.Log;

import org.opencv.core.*;
import org.opencv.highgui.Highgui;
import org.opencv.imgproc.Imgproc;

public class ProcessImage 
{
	private Image image=null;
	private File file;
	private String filePath = "/storage/emulated/0/blobtest/";
	private String lastPath=null;
	private DateFormat dateFormat;
	private Date date;
	private static final String TAG = "ProcessingAPI";
	 
	ProcessImage(Image input)
	{
		assert input != null;
		image=input;
		dateFormat = new SimpleDateFormat("hhmm_yyMMdd");
		date = new Date();
		Log.d(TAG, "Processing object created");
	}
	
	public int processframe() throws IOException
	{
		Log.d(TAG, "Strting Blob processing");
		Log.d(TAG, "Dimensions of image :: "+image.getWidth()+image.getHeight());
		save(image,"original_");
		
		/*Initialising all Mat*/
		Mat inp_image = Highgui.imread(lastPath);
		Mat grey_img = new Mat();
		
		save(inp_image,"convImage_");
		
		Imgproc.cvtColor(inp_image,grey_img,Imgproc.COLOR_RGB2GRAY); //Changing image to grey scale
		save(grey_img,"greyImage_");
		
		int[][] returnMatrix = new int[10][10];
		detector(grey_img, returnMatrix);
		save(grey_img,"detector_");
		
		return 42; //Change to actual ID after processing	
	}
	
	private void detector(Mat input, int[][] returnMatrix) 
	{
		Mat blurred_image = new Mat();
		Mat otsu = new Mat();
		Mat hierarchy = new Mat();
		
		List<MatOfPoint> contours_point = new ArrayList<MatOfPoint>();
		//List<MatOfPoint2f> contours_point2f;
		Vector<Point> center = new Vector<Point>();
		Vector<Float> radius = new Vector<Float>();
		
		Size window = new Size(100,100);
		Imgproc.blur(input, blurred_image, window);
		Imgproc.threshold(blurred_image, otsu, 0, 255, Imgproc.THRESH_BINARY | Imgproc.THRESH_OTSU);
		Imgproc.findContours(otsu.clone(), contours_point,hierarchy, Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_NONE);
		
		int count = contours_point.size();
				
		for( int i=0; i < count; i++)
		{
			Point c = new Point();
			float[] r = new float[count];
			MatOfPoint2f  contours_point2f = new MatOfPoint2f( contours_point.get(i).toArray() );
			Imgproc.minEnclosingCircle(contours_point2f, c, r);

			if (r[i] >= 50)
			{
				center.add(c);
				radius.add(r[i]);
			}
		}
		
		int count2 = center.size();
		Scalar red = new Scalar(255,255,255);

		//returnMatrix.resize(count2);
		

		for( int i = 0; i < count2; i++)
		{
			Core.circle(input, center.get(i), (int)radius.get(i).floatValue(), red, 3);
			returnMatrix[i][0] = (int) center.get(i).x;
			returnMatrix[i][1] = (int) center.get(i).y;
			returnMatrix[i][2] = (int)radius.get(i).floatValue();
		}
		Log.d(TAG, "Detected Circles"+count2);
	}

	private void save(Image imagetosave, String name) throws IOException 
    {
		ByteBuffer buffer = imagetosave.getPlanes()[0].getBuffer();
        byte[] bytes = new byte[buffer.capacity()];
        buffer.get(bytes);
       
        lastPath = filePath+dateFormat.format(date)+name+".jpg";
        file = new File(lastPath);	//With date tag
        //file = new File(filePath+name+".jpg"); //Without date tag
        OutputStream output = null;
        try
        {
            output = new FileOutputStream(file);
            output.write(bytes);
        }
        finally 
        {
            if (null != output)
                output.close();
        }
    }
	
	private void save(Mat image, String name)
	{
        lastPath = filePath+dateFormat.format(date)+name+".jpg";	//With date tag
        //String file = new filePath+name+".jpg"; //Without date tag
        Highgui.imwrite(lastPath,image);
	}
}
