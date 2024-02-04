package com.example.mirror;

import android.graphics.Bitmap;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

public class Communicator extends Thread {
    private final ViewMediator mediator;

    public Communicator(final ViewMediator mediator) {
        this.mediator = mediator;
    }

    @Override
    public void run() {
        // create a server socket
        ServerSocket server;
        try {
            server = new ServerSocket(63200);
        } catch (IOException e) {
            mediator.setText("failed to create a server socket");
            return;
        }

        // show the waiting status
        mediator.setText("waiting a client connection");

        // wait a client connection
        Socket client;
        try {
            client = server.accept();
        } catch (IOException e) {
            mediator.setText("failed to accept a client connection");
            return;
        }

        // show the connected status
        mediator.setText("connected");

        // create a reader
        DataInputStream reader;
        try {
            reader = new DataInputStream(client.getInputStream());
        } catch (IOException e) {
            mediator.setText("failed to create a reader");
            return;
        }

        // get the size of bitmap
        byte[] bufSizeOfBitmap = new byte[8];
        try {
            final int readCount = reader.read(bufSizeOfBitmap, 0, bufSizeOfBitmap.length);
            if (readCount <= 0) {
                throw new IOException("");
            }
        } catch (IOException e) {
            mediator.setText("failed to get the size of bitmap");
            return;
        }
        IntBuffer sizeOfBitmapIntBuffer = ByteBuffer
                .wrap(bufSizeOfBitmap)
                .order(ByteOrder.LITTLE_ENDIAN)
                .asIntBuffer();
        final int width = sizeOfBitmapIntBuffer.get(0);
        final int height = sizeOfBitmapIntBuffer.get(1);

        // remove the text view
        mediator.hideText();

        // create a bitmap
        Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        mediator.setBitmap(bitmap);

        // loop
        byte[] bufPixels = new byte[width * height * 4];
        int[] pixels = new int[width * height];
        while (true) {
            try {
                reader.readFully(bufPixels, 0, bufPixels.length);
            } catch (IOException e) {
                break;
            }
            IntBuffer pixelsIntBuffer = ByteBuffer
                    .wrap(bufPixels)
                    .order(ByteOrder.LITTLE_ENDIAN)
                    .asIntBuffer();
            pixelsIntBuffer.get(pixels);
            bitmap.setPixels(pixels, 0, width, 0, 0, width, height);
            mediator.setBitmap(bitmap);
        }

        // show the end status
        mediator.hideImage();
        mediator.showText();
        mediator.setText("disconnected");
    }
}
