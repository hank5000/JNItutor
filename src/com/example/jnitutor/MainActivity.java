package com.example.jnitutor;

import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.os.Build;

import com.example.jnitutor.SomethingClass;

public class MainActivity extends ActionBarActivity {
//
	private native void CallJNIInt(int input_int);
	private native void CallJNIString(String input_string);
	private native void CallJNIandCallback(String input);
	private native void CallJNIandFillUpSomething(SomethingClass SomethingC);

	int tag = 0;
	SomethingClass mSomethingC = null;
	String JNItag = "JNItutor";

    static {
    	System.loadLibrary("JNItutor");
    }
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		if (savedInstanceState == null) {
			getSupportFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
	}

	public void FunctionA(View view)
	{
		CallJNIInt((int)50);
		tag++;
	}
	
	public void FunctionB(View view)
	{
		CallJNIString("From Java to C");
		tag++;
	}
	
	public void FunctionC(View view)
	{
		Log.d(JNItag,"Call to JNI from java and JNI callback to java FunctionD");
		CallJNIandCallback("Call to JNI");
	}
	
	public void FunctionD()
	{
		Log.d(JNItag,"FunctionD is called "+tag);
	}
	
	public void FunctionE(View view)
	{
		Log.d(JNItag,"Send 'SomethingC' to C, and Create/Fill in JNI side");
		mSomethingC = new SomethingClass();
		CallJNIandFillUpSomething(mSomethingC);
		if(mSomethingC!=null)
		{
			// print mesg
			Log.d(JNItag,"back java side and print the value which is filled in JNI side");
	
			Log.d(JNItag,"mInt : "+mSomethingC.mInt);
			Log.d(JNItag,"mIntArray[0] : "+mSomethingC.mIntArray[0]);
			Log.d(JNItag,"mIntArray[1] : "+mSomethingC.mIntArray[1]);
			Log.d(JNItag,"mString : "+mSomethingC.mString);
			Log.d(JNItag,"mOtherClass.mOtherInt : "+mSomethingC.mOtherClass.mOtherInt);
		}
		else
		{
			Log.d(JNItag,"mSomethingC is still null");
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);
			return rootView;
		}
	}

}
