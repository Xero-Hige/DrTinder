package ar.uba.fi.drtinder;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.net.Uri;
import android.os.AsyncTask;
import android.util.Base64;
import android.widget.ImageView;

import com.bumptech.glide.Glide;
import com.google.common.io.ByteStreams;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.HttpServerErrorException;
import org.springframework.web.client.ResourceAccessException;
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
     * User image resource type
     */
    public static final int RES_USER_IMG = 0;
    /**
     * Interest image resource type
     */
    public static final int RES_INTEREST_IMG = 1;

    private static final String USER_IMAGE_URL = "users/photo";
    private static final String INTER_IMAGE_URL = "interest";
    private static HashMap<Integer, String> cacheMap = new HashMap<>();
    private static HashMap<Integer, CountDownLatch> fetchingMap = new HashMap<>();


    private ImageResourcesHandler() {
    }

    private static String getUrlByType(Integer type) {
        switch (type) {
            case RES_USER_IMG:
                return ServerUrlWrapper.getServerUrl() + USER_IMAGE_URL;
            case RES_INTEREST_IMG:
                return ServerUrlWrapper.getServerUrl() + INTER_IMAGE_URL;
            default:
                return "";
        }
    }

    /**
     * Prefetch resources from server (if needed) in order to improve network resources use.
     *
     * @param imageId      Server id of the fetching resource
     * @param resourceType Type of the resource (One of listed const types)
     * @param context      Context of the calling activity
     * @param token        User token needed by the server to auth client
     */
    public static void prefetch(String imageId, int resourceType, String token, Context context) {
        DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "Prefetching: " + imageId);
        Integer cacheKey = getCacheKey(resourceType, imageId);
        if (cacheMap.containsKey(cacheKey) || fetchingMap.containsKey(cacheKey)) {
            return;
        }

        FetchImageTask task = new FetchImageTask(resourceType, imageId, token, null, context);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private static int getCacheKey(int resourceType, String resId) {
        return String.format(Locale.ENGLISH, "%d::%s", resourceType, resId).hashCode();
    }

    /**
     * Fills the parameter image view with the server image resourve
     *
     * @param imageId      Image id
     * @param resourceType Type of the resource (one of the listed RES_* constants)
     * @param token        Session token
     * @param imgView      Imageview to fill with the resource
     * @param context      Context of the calling activity
     */
    public static void fillImageResource(String imageId, int resourceType, String token,
                                         ImageView imgView, Context context) {
        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Filling resource with: " + imageId);
        FetchImageTask task = new FetchImageTask(resourceType, imageId, token, imgView, context);
        task.execute();
    }

    private static Bitmap recoverCachedImg(Integer cacheKey) {
        String path = cacheMap.get(cacheKey);
        try {
            FileInputStream input = new FileInputStream(path);
            byte[] byteImage = ByteStreams.toByteArray(input);
            return convertToBitmap(byteImage);
        } catch (FileNotFoundException e) {
            DrTinderLogger.writeLog(DrTinderLogger.WARN, "Cached image not found");
            return null;
        } catch (IOException ignored) {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Error opening cache image file");
            return null;
        }
    }

    private static Bitmap convertToBitmap(byte[] byteImage) {
        return BitmapFactory.decodeByteArray(byteImage, 0, byteImage.length);
    }

    private static void addToFetching(Integer cacheKey) {
        fetchingMap.put(cacheKey, new CountDownLatch(1));
    }

    private static void removeFromFetching(Integer cacheKey) {
        if (!fetchingMap.containsKey(cacheKey)) {
            return;
        }
        fetchingMap.get(cacheKey).countDown();
    }

    /**
     * Removes from cache an specific resource
     * <p>
     * (Helpful when a certain resource is never going to be used again for sure)
     *
     * @param imageId      Image id
     * @param resourceType Type of the resource (one of the listed RES_* constants)
     * @param context      Context of the calling activity
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
     * Clears all the cached files
     *
     * @param context Context of the calling activity
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

    private static class FetchImageTask extends AsyncTask<Void, Void, Boolean> {

        private final Integer mCacheKey;
        private final ImageView mImageView;
        private final Context mContext;
        private String mImageUrl;
        private Bitmap mImageBitmap;

        FetchImageTask(int resourceType, String resourceId, String token,
                       ImageView imageView, Context context) {
            setImageUrl(resourceType, resourceId, token);
            this.mImageView = imageView;
            this.mContext = context;
            this.mCacheKey = getCacheKey(resourceType, resourceId);
        }

        private void setImageUrl(int resourceType, String resId, String token) {
            String url = getUrlByType(resourceType);
            Uri.Builder uriBuilder = Uri.parse(url).buildUpon();
            uriBuilder.appendQueryParameter("token", token);
            uriBuilder.appendQueryParameter("res_id", resId);
            this.mImageUrl = uriBuilder.build().toString();
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
            mImageBitmap = getImage();
            removeFromFetching(mCacheKey);
            return mImageBitmap != null;
        }

        @Override
        protected void onPostExecute(final Boolean success) {

            if (mContext instanceof Activity) {
                Activity contextActivity = (Activity) mContext;
                if (contextActivity.isDestroyed()) {
                    DrTinderLogger.writeLog(DrTinderLogger.INFO,
                            "Trying to load image on destroyed activity");
                    return;
                }
            }

            if (mImageView == null) {
                DrTinderLogger.writeLog(DrTinderLogger.INFO,
                        "Trying to load image on null imageView");
                return;
            }

            if (success) {

                int bitmapWidth = mImageBitmap.getWidth();
                int bitmapHeight = mImageBitmap.getHeight();


                int initialY = (bitmapWidth < bitmapHeight) ? (bitmapHeight - bitmapWidth) / 2 : 0;
                int initialX = (bitmapWidth < bitmapHeight) ? 0 : (bitmapWidth - bitmapHeight) / 2;
                int resultWidth = (bitmapWidth < bitmapHeight) ? bitmapWidth : bitmapHeight;
                int resultHeight = (bitmapWidth < bitmapHeight) ? bitmapWidth : bitmapHeight;

                int viewWidth = mImageView.getWidth();
                int viewHeight = mImageView.getHeight();

                Matrix matrix = new Matrix();
                float scale = ((viewWidth < viewHeight) ? viewWidth + 0.0f : viewHeight + 0.0f)
                        / ((bitmapWidth < bitmapHeight) ? bitmapWidth : bitmapHeight);
                matrix.postScale(scale, scale);

                mImageBitmap = Bitmap.createBitmap(mImageBitmap, initialX, initialY,
                        resultWidth, resultHeight, matrix, true);

                mImageView.setImageBitmap(mImageBitmap);
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Loaded image " + mImageUrl);
                return;
            }

            DrTinderLogger.writeLog(DrTinderLogger.WARN, "Failed to get image");
            if (mContext != null) {
                Glide.with(mContext).load(R.drawable.not_found).centerCrop().into(mImageView);
            }

        }

        private Bitmap getImage() {
            DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "Begin image fetch " + mImageUrl);
            RestTemplate restTemplate = new RestTemplate();
            restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
            String result;
            try {
                result = restTemplate.getForObject(mImageUrl, String.class, "Android");
            } catch (HttpClientErrorException e) {
                DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
                return null;
            } catch (HttpServerErrorException e) {
                DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
                return null;
            } catch (ResourceAccessException e) {
                DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Can't connect: " + e.getMessage());
                return null;
            }
            DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "End image     fetch " + mImageUrl);

            try {
                byte[] imageString = Base64.decode(result, Base64.DEFAULT);
                cacheImgFile(mCacheKey, imageString, mContext);
                return convertToBitmap(imageString);
            } catch (Exception e) {
                return null;
            }
        }

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
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Cannot generate image cache (THIS SHOULD NOT HAPPEN)");
                return;
            }

            try {
                file.write(dataArray);
                file.close();
            } catch (IOException e) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Cannot generate image cache (IOError)");
                return;
            }

            cacheMap.put(cacheKey, cachePath);
        }
    }
}
