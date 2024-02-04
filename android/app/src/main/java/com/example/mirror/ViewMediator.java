package com.example.mirror;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.activity.ComponentActivity;

public class ViewMediator {
    private final ComponentActivity context;
    private final TextView textView;
    private final ImageView imageView;

    public ViewMediator(final ComponentActivity context) {
        this.context = context;

        // create a frame layout
        FrameLayout layout = new FrameLayout(context);
        layout.setBackgroundColor(Color.BLACK);
        context.setContentView(layout);

        // create a text view
        textView = new TextView(context);
        textView.setTextColor(Color.WHITE);
        textView.setGravity(Gravity.CENTER);

        // create an image view
        imageView = new ImageView(context);

        // add views
        layout.addView(imageView);
        layout.addView(textView);
    }

    public void setText(String text) {
        context.runOnUiThread(() -> textView.setText(text));
    }

    public void hideText() {
        context.runOnUiThread(() -> textView.setVisibility(View.INVISIBLE));
    }

    public void showText() {
        context.runOnUiThread(() -> textView.setVisibility(View.VISIBLE));
    }

    public void setBitmap(Bitmap bitmap) {
        context.runOnUiThread(() -> imageView.setImageBitmap(bitmap));
    }

    public void hideImage() {
        context.runOnUiThread(() -> imageView.setVisibility(View.INVISIBLE));
    }

    public void showImage() {
        context.runOnUiThread(() -> imageView.setVisibility(View.VISIBLE));
    }
}
