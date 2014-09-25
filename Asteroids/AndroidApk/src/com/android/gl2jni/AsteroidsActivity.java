package com.android.asteroids;


import android.app.Activity;
import android.os.Bundle;


public class AsteroidsActivity extends Activity
{
    AsteroidsView mView;


    @Override protected void onCreate(Bundle bundle)
    {
        super.onCreate(bundle);
        mView = new AsteroidsView(getApplication());
		setContentView(mView);
    }


    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }


    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }
}
