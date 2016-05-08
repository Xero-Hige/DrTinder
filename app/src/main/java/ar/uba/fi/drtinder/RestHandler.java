package ar.uba.fi.drtinder;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Base64;
import android.widget.ImageView;

import com.bumptech.glide.Glide;
import com.google.common.io.ByteStreams;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;

/**
 * RestHandler.java
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
 * FIXME: Rename class
 */
public class RestHandler {

    static final String USER_IMAGE_URL = "http://demo2753541.mockable.io/users/image/";

    static final int RES_USER_IMG = 0;

    static final HashMap<String, String> cacheMap = new HashMap<>();

    static private String getUrlByType(Integer type) {
        switch (type) {
            case RES_USER_IMG:
                return USER_IMAGE_URL;
            default:
                return "";
        }
    }

    static private String getCacheKey(int resourceType, String resId) {
        return String.format("%d::%s", resourceType, resId);
    }

    static void fillImageResource(String imageId, int resourceType, ImageView imgView, Context context) {
        String cacheKey = getCacheKey(resourceType, imageId);

        if (cacheMap.containsKey(cacheKey)) {
            byte[] img = recoverCachedImg(cacheKey);
            Glide.with(context).load(img).centerCrop().into(imgView);
            return;
        }

        FetchImageTask task = new FetchImageTask(resourceType, imageId, imgView, context);
        task.execute();
    }

    private static byte[] getBase64Img(String imageUrl) {
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
        String result = restTemplate.getForObject(imageUrl, String.class, "Android");

        return Base64.decode(result, Base64.DEFAULT);
    }

    private static void cacheImgFile(String cacheKey, byte[] dataArray, Context context) {
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
    }

    private static byte[] recoverCachedImg(String cacheKey) {
        String path = cacheMap.get(cacheKey);
        try {
            FileInputStream input = new FileInputStream(path);
            return ByteStreams.toByteArray(input);
        } catch (FileNotFoundException e) {
            return null;
        } catch (IOException ignored) {
            return null; //TODO: Check
        }
    }

    private static class FetchImageTask extends AsyncTask<Void, Void, Boolean> {

        private final String imageUrl;
        private final String cacheKey;
        private final ImageView imageView;
        private final Context context;
        private byte[] imageArray;

        FetchImageTask(int resourceType, String resId, ImageView imageView, Context context) { //FIXME Names
            String url = getUrlByType(resourceType);
            this.imageUrl = url + resId;
            this.imageView = imageView;
            this.context = context;
            this.cacheKey = getCacheKey(resourceType, resId);
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            imageArray = getBase64Img(imageUrl);
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) return;
            Glide.with(context).load(imageArray).centerCrop().into(imageView);

            cacheImgFile(cacheKey, imageArray, context);
        }

    }
}
