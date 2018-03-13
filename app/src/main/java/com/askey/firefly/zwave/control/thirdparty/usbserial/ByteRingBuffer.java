package com.askey.firefly.zwave.control.thirdparty.usbserial;

import android.util.Log;

public class ByteRingBuffer {

    private byte[]               rBuf;
    private int                  rBufSize;
    private int                  rBufPos;
    private int                  rBufUsed;

    private final String TAG  = "ByteRingBuffer";

    public ByteRingBuffer (int size)
    {
        if (size <= 0)
        {
            throw new IllegalArgumentException();
        }

        rBufSize = size;
        rBuf = new byte[rBufSize];
    }

    public void resize (int newSize)
    {
        if (newSize <= 0)
        {
            throw new IllegalArgumentException();
        }

        if (newSize < rBufUsed)
        {
            discard(rBufUsed - newSize);
        }

        byte[] newBuf = new byte[newSize];

        int newBufUsed = read(newBuf, 0, newSize);

        rBuf = newBuf;
        rBufSize = newSize;
        rBufPos = 0;
        rBufUsed = newBufUsed;
    }

    public int getSize()
    {
        return rBufSize;
    }

    public int getFree()
    {
        return rBufSize - rBufUsed;
    }

    public int getUsed()
    {
        return rBufUsed;
    }

    public void clear()
    {
        rBufPos = 0;
        rBufUsed = 0;
    }

    public void discard (int len)
    {
        if (len < 0)
        {
            throw new IllegalArgumentException();
        }

        int trLen = Math.min(len, rBufUsed);
        rBufPos = clip(rBufPos + trLen);
        rBufUsed -= trLen;
    }

    public int write (byte[] buf, int pos, int len)
    {
        if (len < 0) {
            throw new IllegalArgumentException();
        }

        if (rBufUsed == 0) {
            rBufPos = 0;
        }

        int p1 = rBufPos + rBufUsed;

        if (p1 < rBufSize) {
            int trLen1 = Math.min(len, rBufSize - p1);
            append(buf, pos, trLen1);
            int trLen2 = Math.min(len - trLen1, rBufPos);
            append(buf, pos + trLen1, trLen2);
            return trLen1 + trLen2;
        } else {
            int trLen = Math.min(len, rBufSize - rBufUsed);
            append(buf, pos, trLen);
            return trLen;
        }
    }

    public int write (byte[] buf)
    {
        return write(buf, 0, buf.length);
    }

    private void append (byte[] buf, int pos, int len)
    {
        if (len == 0)
        {
            return;
        }

        if (len < 0)
        {
            throw new AssertionError();
        }

        int p = clip(rBufPos + rBufUsed);
        System.arraycopy(buf, pos, rBuf, p, len);
        rBufUsed += len;
    }

    public int read (byte[] buf, int pos, int len)
    {
        if (len < 0)
        {
            throw new IllegalArgumentException();
        }

        int trLen1 = Math.min(len, Math.min(rBufUsed, rBufSize - rBufPos));
        remove(buf, pos, trLen1);
        int trLen2 = Math.min(len - trLen1, rBufUsed);
        remove(buf, pos + trLen1, trLen2);
        return trLen1 + trLen2;
    }

    public int read (byte[] buf)
    {
        int len =  read(buf, 0, buf.length);
        return len;
    }

    private void remove (byte[] buf, int pos, int len)
    {
        if (len == 0)
        {
            return;
        }

        if (len < 0)
        {
            throw new AssertionError();
        }

        System.arraycopy(rBuf, rBufPos, buf, pos, len);
        rBufPos = clip(rBufPos + len);
        rBufUsed -= len;
    }

    private int clip (int p)
    {
        return (p < rBufSize) ? p : (p - rBufSize);
    }
}
