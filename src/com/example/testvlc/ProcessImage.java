package com.example.testvlc;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

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
		Mat grey_img = null;
		
		save(inp_image,"convImage_");
		
		Imgproc.cvtColor(inp_image,grey_img,Imgproc.COLOR_RGB2GRAY); //Changing image to grey scale
		save(grey_img,"greyImage_");
		
		return 42; //Change to actual ID after processing	
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
