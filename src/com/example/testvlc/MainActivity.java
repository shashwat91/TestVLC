package com.example.testvlc;

import java.util.Arrays;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Size;
import android.view.*;
import android.widget.Button;
import android.widget.Toast;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.*;
import android.hardware.camera2.params.StreamConfigurationMap;

public class MainActivity extends Activity
{
	CameraManager manager;
	CameraCharacteristics characteristics;
	CameraDevice.StateCallback callback;
	
	private Button btn_opencamera;
	private String[] cameraID;
	private static final int REQUEST_CAMERA_PERMISSION = 200;
	private TextureView textureView;
	private Size imageDimension;
	private Handler mBackgroundHandler;
	protected CaptureRequest.Builder captureRequestBuilder;
	protected CameraDevice cameraDevice;
	protected CameraCaptureSession cameraCaptureSessions;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		btn_opencamera = (Button) findViewById(R.id.btn_takepicture);
		manager = (CameraManager)getSystemService(Context.CAMERA_SERVICE);
		openCamera();
		btn_opencamera.setOnClickListener(new View.OnClickListener() 
        {
            public void onClick(View arg0) 
            {
                System.out.println("button pressed");
                //openCamera();
            }
         });
	}

	@Override
    public void onDestroy() 
    {
		super.onDestroy();
    }
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings)
		{
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	@Override
	public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults)
	{
        if (requestCode == REQUEST_CAMERA_PERMISSION)
        {
           if (grantResults[0] == PackageManager.PERMISSION_DENIED)
           {
                Toast.makeText(MainActivity.this, "Sorry!!!, you can't use this app without granting permission", Toast.LENGTH_LONG).show();
                finish();
           }
        }
    }
	
	private void openCamera()
	{
		try
		{
			cameraID = manager.getCameraIdList();
			characteristics = manager.getCameraCharacteristics(cameraID[0]);
			StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
            assert map != null;
            imageDimension = map.getOutputSizes(SurfaceTexture.class)[0];
			/*int[] modes_available;
			modes_available = characteristics.get(CameraCharacteristics.CONTROL_AE_AVAILABLE_MODES);
			for(int i=0; i<modes_available.length; ++i)
				System.out.println(modes_available[i]);*/
			
			//Asking for user permissions
			if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED)
			{
				ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.CAMERA}, REQUEST_CAMERA_PERMISSION);
			}
			
			callback = new CameraDevice.StateCallback() 
			{
				public void onOpened(CameraDevice camera)
				{
					System.out.println("camera opened");
				}
				public void onDisconnected(CameraDevice camera){}
				public void onError(CameraDevice camera, int error){}
			};
			
			manager.openCamera(cameraID[0],callback ,null);
			createCameraPreview();
		}
		catch (CameraAccessException e) 
		{
			System.out.println("exception in getting cameraID");
			e.printStackTrace();
		}
		catch (Exception e1)
		{
			e1.printStackTrace();
		}
	}
	
	protected void createCameraPreview() {
        try 
        {
        	System.out.println("in create camera preview method");
            SurfaceTexture texture = textureView.getSurfaceTexture();
            assert texture != null;
            texture.setDefaultBufferSize(imageDimension.getWidth(), imageDimension.getHeight());
            Surface surface = new Surface(texture);
            captureRequestBuilder = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            captureRequestBuilder.addTarget(surface);
            cameraDevice.createCaptureSession(Arrays.asList(surface), new CameraCaptureSession.StateCallback()
            {
            	@Override
            	public void onConfigured(@NonNull CameraCaptureSession cameraCaptureSession)
            	{
            		//The camera is already closed
            		if (null == cameraDevice) 
            		{
            			return;
            		}
            		// When the session is ready, we start displaying the preview.
            		cameraCaptureSessions = cameraCaptureSession;
            		// updatePreview();
            	}
            	@Override
            	public void onConfigureFailed(@NonNull CameraCaptureSession cameraCaptureSession)
            	{
            		Toast.makeText(MainActivity.this, "Configuration change", Toast.LENGTH_SHORT).show();
            	}
              }, null);
        }
        catch (CameraAccessException e)
        {
                 e.printStackTrace();
        }
        captureRequestBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
        try
		{
			cameraCaptureSessions.setRepeatingRequest(captureRequestBuilder.build(), null, mBackgroundHandler);
		}
        catch (CameraAccessException e)
		{
			e.printStackTrace();
		}
    }
}