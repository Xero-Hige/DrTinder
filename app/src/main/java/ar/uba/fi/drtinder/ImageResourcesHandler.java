package ar.uba.fi.drtinder;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Base64;
import android.widget.ImageView;

import com.bumptech.glide.Glide;
import com.google.common.io.ByteStreams;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.HttpServerErrorException;
import org.springframework.web.client.RestTemplate;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Locale;
import java.util.concurrent.CountDownLatch;

/**
 * @author Xero-Hige
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * Resource Handler that manages requests and cache of image resources
 */
public final class ImageResourcesHandler {

    /**
     * TODO
     */
    public static final int RES_USER_IMG = 0;
    /**
     * TODO
     */
    public static final int RES_INTEREST_IMG = 1;


    private static final String USER_IMAGE_URL = "http://demo2753541.mockable.io/users/image/";
    private static final String INTER_IMAGE_URL = "http://demo2753541.mockable.io/users/image/";
    private static HashMap<Integer, String> cacheMap = new HashMap<>();
    private static HashMap<Integer, CountDownLatch> fetchingMap = new HashMap<>();

    /**
     * TODO
     */
    private ImageResourcesHandler() {
    }

    /**
     * TODO
     *
     * @param type
     * @return
     */
    private static String getUrlByType(Integer type) {
        switch (type) {
            case RES_USER_IMG:
                return USER_IMAGE_URL;
            case RES_INTEREST_IMG:
                return INTER_IMAGE_URL;
            default:
                return "";
        }
    }

    /**
     * TODO
     * @param resourceType
     * @param resId
     * @return
     */
    private static int getCacheKey(int resourceType, String resId) {
        return String.format(Locale.ENGLISH, "%d::%s", resourceType, resId).hashCode();
    }

    /**
     * Prefetch resources from server (if needed) in order to improve network resources use.
     * @param imageId: Server id of the fetching resource
     * @param resourceType: Type of the resource (One of listed const types)
     * @param context: Context of the calling activity
     */
    static void prefetch(String imageId, int resourceType, Context context) {
        DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "Prefetching: " + imageId);
        Integer cacheKey = getCacheKey(resourceType, imageId);
        if (cacheMap.containsKey(cacheKey) || fetchingMap.containsKey(cacheKey)) {
            return;
        }

        FetchImageTask task = new FetchImageTask(resourceType, imageId, null, context);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    /**
     * TODO
     * @param imageId
     * @param resourceType
     * @param imgView
     * @param context
     */
    static void fillImageResource(String imageId, int resourceType, ImageView imgView,
                                  Context context) {
        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Filling resource with: " + imageId);
        FetchImageTask task = new FetchImageTask(resourceType, imageId, imgView, context);
        task.execute();
    }

    /**
     * TODO
     * @param byteImage
     * @return
     */
    private static Bitmap convertToBitmap(byte[] byteImage) {
        return BitmapFactory.decodeByteArray(byteImage, 0, byteImage.length);
    }

    /**
     * TODO
     * @param cacheKey
     * @return
     */
    private static Bitmap recoverCachedImg(Integer cacheKey) {
        String path = cacheMap.get(cacheKey);
        try {
            FileInputStream input = new FileInputStream(path);
            byte[] byteImage = ByteStreams.toByteArray(input);
            return convertToBitmap(byteImage);
        } catch (FileNotFoundException e) {
            return null;
        } catch (IOException ignored) {
            return null; //TODO: Check
        }
    }

    /**
     * TODO
     * @param cacheKey
     */
    private static void addToFetching(Integer cacheKey) {
        fetchingMap.put(cacheKey, new CountDownLatch(1));
    }

    /**
     * TODO
     * @param cacheKey
     */
    private static void removeFromFetching(Integer cacheKey) {
        if (!fetchingMap.containsKey(cacheKey)) {
            return;
        }
        fetchingMap.get(cacheKey).countDown();
    }

    /**
     * TODO
     * @param imageId
     * @param resourceType
     * @param context
     */
    public static void freeCachedResource(String imageId, int resourceType, Context context) {
        int cacheKey = getCacheKey(resourceType, imageId);
        if (!cacheMap.containsKey(cacheKey) || fetchingMap.containsKey(cacheKey)) {
            return;
        }

        cacheMap.remove(cacheKey);

        String cachePath = context.getFilesDir().getAbsolutePath() + File.separator + cacheKey;
        File cachedImage = new File(cachePath);
        boolean success = cachedImage.delete();
        if (success) {
            DrTinderLogger.writeLog(DrTinderLogger.INFO, String.format(Locale.ENGLISH,
                    "Removed image with id %s from cache", imageId));
        } else {
            DrTinderLogger.writeLog(DrTinderLogger.WARN, String.format(Locale.ENGLISH,
                    "Failed to remove image with id %s from cache", imageId));
        }
    }

    /**
     * TODO
     * @param context
     */
    public static void clearCache(Context context) {
        boolean operationSuccess = true;
        for (int cacheKey : cacheMap.keySet()) {
            String cachePath = context.getFilesDir().getAbsolutePath() + File.separator + cacheKey;
            File cachedImage = new File(cachePath);
            boolean success = cachedImage.delete();
            if (!success) {
                DrTinderLogger.writeLog(DrTinderLogger.INFO, String.format(Locale.ENGLISH,
                        "Failed to remove image with cache id : %d", cacheKey));
            }
            operationSuccess = operationSuccess && success;
        }
        if (operationSuccess) {
            DrTinderLogger.writeLog(DrTinderLogger.INFO, "Successfully cleared cache");
        } else {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Cache clear failed");
        }
        cacheMap.clear();
        fetchingMap.clear();
    }

    /**
     * TODO
     */
    private static class FetchImageTask extends AsyncTask<Void, Void, Boolean> {

        private final String mImageUrl;
        private final Integer mCacheKey;
        private final ImageView mImageView;
        private final Context mContext;
        private Bitmap mImageBitmap;

        /**
         * TODO
         * @param resourceType
         * @param resId
         * @param imageView
         * @param context
         */
        FetchImageTask(int resourceType, String resId, ImageView imageView, Context context) { //FIXME Names
            String url = getUrlByType(resourceType);
            this.mImageUrl = url + resId;
            this.mImageView = imageView;
            this.mContext = context;
            this.mCacheKey = getCacheKey(resourceType, resId);
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            if (fetchingMap.containsKey(mCacheKey)) {
                try {
                    fetchingMap.get(mCacheKey).await();
                } catch (InterruptedException e) {
                    DrTinderLogger.writeLog(DrTinderLogger.WARN, "Fetch end wait interrupted");
                }
            }

            if (cacheMap.containsKey(mCacheKey)) {
                mImageBitmap = recoverCachedImg(mCacheKey);
                return true;
            }
            addToFetching(mCacheKey);
            mImageBitmap = getImage(mImageUrl);
            removeFromFetching(mCacheKey);
            return mImageBitmap != null;
        }

        @Override
        protected void onPostExecute(final Boolean success) {

            if (mContext instanceof Activity) {
                Activity contextActivity = (Activity) mContext;
                if (contextActivity.isDestroyed()) {
                    DrTinderLogger.writeLog(DrTinderLogger.INFO, "Trying to load image on destroyed activity");
                    return;
                }
            }

            if (mImageView == null) {
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Trying to load image on null imageView");
                return;
            }

            if (success) {
                mImageView.setImageBitmap(mImageBitmap);
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Loaded image " + mImageUrl);
                return;
            }

            DrTinderLogger.writeLog(DrTinderLogger.WARN, "Failed to get image");
            Glide.with(mContext).load(R.drawable.not_found).centerCrop().into(mImageView);

        }

        /**
         * TODO
         * @param cacheKey
         * @param dataArray
         * @param context
         */
        private void cacheImgFile(Integer cacheKey, byte[] dataArray, Context context) {
            if (context == null) {
                DrTinderLogger.writeLog(DrTinderLogger.WARN, "Context is null @cacheImgFile");
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
        }

        /**
         * TODO
         * @param imageUrl
         * @return
         */
        private Bitmap getImage(String imageUrl) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "Begin fetch " + imageUrl);
            RestTemplate restTemplate = new RestTemplate();
            restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
            String result;
            try {
                result = restTemplate.getForObject(imageUrl, String.class, "Android");
            } catch (HttpClientErrorException e) {
                DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
                return null;
            } catch (HttpServerErrorException e) {
                DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
                return null;
            }
            DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "End fetch " + imageUrl);
            byte[] imageString = Base64.decode(result, Base64.DEFAULT);
            cacheImgFile(mCacheKey, imageString, mContext);
            return convertToBitmap(imageString);
        }
    }
}
