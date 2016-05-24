package ar.uba.fi.drtinder;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Base64;
import android.widget.ImageView;

import com.bumptech.glide.Glide;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.RestTemplate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Locale;

/**
 * ImageResourcesHandler.java
 * <p>
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses
 */

/**
 *
 */
public final class ImageResourcesHandler {

    public static final int RES_USER_IMG = 0;
    private static final String USER_IMAGE_URL = "http://demo2753541.mockable.io/users/image/";
    private static HashMap<String, String> cacheMap = new HashMap<>();
    private static HashMap<String, Integer> fetchingMap = new HashMap<>();

    private ImageResourcesHandler() {
    }

    private static String getUrlByType(Integer type) {
        switch (type) {
            case RES_USER_IMG:
                return USER_IMAGE_URL;
            default:
                return "";
        }
    }

    private static String getCacheKey(int resourceType, String resId) {
        return String.format(Locale.ENGLISH, "%d::%s", resourceType, resId);
    }

    static void prefetch(String imageId, int resourceType, Context context) {
        DrTinderLogger.log(DrTinderLogger.NET_INFO, "Prefetching: " + imageId);
        String cacheKey = getCacheKey(resourceType, imageId);
        if (cacheMap.containsKey(cacheKey) || fetchingMap.containsKey(cacheKey)) {
            return;
        }

        FetchImageTask task = new FetchImageTask(resourceType, imageId, null, context);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    static void fillImageResource(String imageId, int resourceType, ImageView imgView,
                                  Context context) {
        DrTinderLogger.log(DrTinderLogger.INFO, "Filling resource with: " + imageId);
        FetchImageTask task = new FetchImageTask(resourceType, imageId, imgView, context);
        task.execute();
    }

    private static Bitmap getImage(String imageUrl) {
        DrTinderLogger.log(DrTinderLogger.NET_INFO, "Begin fetch " + imageUrl);
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
        try {
            String result = restTemplate.getForObject(imageUrl, String.class, "Android");
            DrTinderLogger.log(DrTinderLogger.NET_INFO, "End fetch " + imageUrl);
            byte[] imageString = Base64.decode(result, Base64.DEFAULT);
            return BitmapFactory.decodeByteArray(imageString, 0, imageString.length);
        } catch (HttpClientErrorException e) {
            return null;
        }
    }

    private static void cacheImgFile(String cacheKey, byte[] dataArray, Context context) {
        if (context == null) {
            DrTinderLogger.log(DrTinderLogger.WARN, "Context is null @cacheImgFile");
            return;
        }

        String cachePath = context.getFilesDir().getAbsolutePath() + File.separator + cacheKey;

        FileOutputStream file;
        try {
            file = new FileOutputStream(cachePath);
        } catch (FileNotFoundException e) {
            return; //TODO: Check
        }

        try {
            file.write(dataArray);
            file.close();
        } catch (IOException e) {
            return; //TODO: Check
        }

        cacheMap.put(cacheKey, cachePath);
        fetchingMap.remove(cacheKey);
    }

    private static Bitmap recoverCachedImg(String cacheKey) {
        String path = cacheMap.get(cacheKey);
        try {
            FileInputStream input = new FileInputStream(path);
            //return ByteStreams.toByteArray(input);
            return null; //FIXME
        } catch (FileNotFoundException e) {
            return null;
        } catch (IOException ignored) {
            return null; //TODO: Check
        }
    }

    private static class FetchImageTask extends AsyncTask<Void, Void, Boolean> {

        private final String mImageUrl;
        private final String mCacheKey;
        private final ImageView mImageView;
        private final Context mContext;
        private Bitmap mImageBitmap;

        FetchImageTask(int resourceType, String resId, ImageView imageView, Context context) { //FIXME Names
            String url = getUrlByType(resourceType);
            this.mImageUrl = url + resId;
            this.mImageView = imageView;
            this.mContext = context;
            this.mCacheKey = getCacheKey(resourceType, resId);
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            while (fetchingMap.containsKey(mCacheKey)) {
                try {
                    Thread.sleep(1000, 0);
                } catch (InterruptedException e) {
                    DrTinderLogger.log(DrTinderLogger.WARN, "Fetch end wait interrupted"); //TODO: Check
                }
            }

            //if (cacheMap.containsKey(mCacheKey)) {
            //    mImageBitmap = recoverCachedImg(mCacheKey);
            //} else {
            //fetchingMap.put(mCacheKey, 0);
            mImageBitmap = getImage(mImageUrl);
            if (mImageBitmap == null) {
                    return false;
                //    }
            }
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (mImageView != null) {
                if (!success) {
                    Glide.with(mContext).load(R.drawable.not_found).centerCrop().into(mImageView);
                    return;
                }
                //Glide.with(mContext).load(mImageBitmap).centerCrop().into(mImageView);
                mImageView.setImageBitmap(mImageBitmap);
                DrTinderLogger.log(DrTinderLogger.INFO, "Loaded image " + mImageUrl);
            }
            //cacheImgFile(mCacheKey, mImageBitmap, mContext);
        }

    }
}
