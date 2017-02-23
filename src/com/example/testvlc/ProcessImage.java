package com.example.testvlc;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.media.Image;

public class ProcessImage 
{
	private Image image=null;
	private File file;
	String filePath = "/storage/emulated/0/blobtest/";
	DateFormat dateFormat;
	Date date;
	 
	ProcessImage(Image input)
	{
		image=input;
	}
	
	public int processframe()
	{
		try{
		save(image,"test_name");
		}
		//Catch for save function
		catch (FileNotFoundException e) {
        	e.printStackTrace();
    	}
    	catch (IOException e) {
        	e.printStackTrace();
    	}
		
		return 42; //Change to actual ID after processing	
	}
	
	private void save(Image imagetosave, String name) throws IOException 
    {
		ByteBuffer buffer = imagetosave.getPlanes()[0].getBuffer();
        byte[] bytes = new byte[buffer.capacity()];
        buffer.get(bytes);
        dateFormat = new SimpleDateFormat("yyyyMMdd_mmss");
        date = new Date();
        final File file = new File(filePath+name+dateFormat.format(date)+".jpg");
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
	
}
