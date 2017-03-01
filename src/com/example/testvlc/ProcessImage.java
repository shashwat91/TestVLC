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
		Log.d(TAG, "Processing object created");
	}
	
	public int processframe() throws IOException
	{
		Log.d(TAG, "Strting Blob processing");
		Log.d(TAG, "Dimensions of image :: "+image.getWidth()+image.getHeight());
		//ImageUtils conv = new ImageUtils();
		//Mat inp_image = conv.imageToMat(image);
		save(image,"original_");
		
		Mat inp_image = Highgui.imread(lastPath);
		
		save(inp_image,"convImage_");
		//Imgproc.cvtColor(inp_image,gray_img,Imgproc.COLOR_RGB2GRAY);
		return 42; //Change to actual ID after processing	
	}
	
	private void save(Image imagetosave, String name) throws IOException 
    {
		ByteBuffer buffer = imagetosave.getPlanes()[0].getBuffer();
        byte[] bytes = new byte[buffer.capacity()];
        buffer.get(bytes);
        dateFormat = new SimpleDateFormat("hhmmss_yyyyMMdd");
        date = new Date();
        lastPath = filePath+name+dateFormat.format(date)+".jpg";
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
		dateFormat = new SimpleDateFormat("hhmmss_yyyyMMdd");
        date = new Date();
        lastPath = filePath+name+dateFormat.format(date)+".jpg";	//With date tag
        //String file = new filePath+name+".jpg"; //Without date tag
        Highgui.imwrite(lastPath,image);
	}
	
	private Mat img2mat(Image inp)
	{
		Mat mat = new Mat(inp.getHeight(), inp.getWidth(), CvType.CV_8UC3);
		ByteBuffer buffer = inp.getPlanes()[0].getBuffer();
        byte[] bytes = new byte[buffer.capacity()];
        buffer.get(bytes);
        mat.put(0, 0, bytes);
		return mat;
	}
}
