package ar.uba.fi.drtinder;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Base64;
import android.util.Log;
import android.widget.ImageView;

import com.bumptech.glide.Glide;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

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

    static final String USER_IMAGE = "http://demo2753541.mockable.io/users/image/";

    static byte[] getUserImage(String userId) {
        String url = USER_IMAGE + userId;
        Log.i("INFO:  ", url);

        return getBase64Img(url);
    }

    static void fillImageResource(String imageId, String resourceType, ImageView imgView, Context context) {
        String url = resourceType + imageId;
        Log.i("INFO Loading: ", url);

        FetchImageTask task = new FetchImageTask(url, imgView, context);
        task.execute();
    }

    private static byte[] getBase64Img(String imageUrl) {
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
        String result = restTemplate.getForObject(imageUrl, String.class, "Android");

        return Base64.decode(result, Base64.DEFAULT);
    }

    private static class FetchImageTask extends AsyncTask<Void, Void, Boolean> {


        private final String imageUrl;
        private final ImageView imageView;
        private final Context context;
        private byte[] imageArray;

        FetchImageTask(String resourceUrl, ImageView imageView, Context context) { //FIXME Names
            this.imageUrl = resourceUrl;
            this.imageView = imageView;
            this.context = context;
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

        }
    }
}
