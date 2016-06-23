package ar.uba.fi.drtinder;

import android.net.Uri;
import android.os.AsyncTask;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpServerErrorException;
import org.springframework.web.client.ResourceAccessException;
import org.springframework.web.client.RestTemplate;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import au.com.bytecode.opencsv.CSVReader;

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
 *
 */
public final class StringResourcesHandler {

    public static final int USER_CANDIDATES = 0;
    public static final int USER_MATCHES = 1;
    public static final int USER_CHAT = 2;
    public static final int USER_INFO = 3;

    public static final String INTEREST_DIVIDER = "\\|\\|";
    public static final String INTEREST_DATA_DIVIDER = "::";

    private static final String SERVER_URL = "http://190.55.231.26/";
    private static final String CANDIDATES_URL = SERVER_URL + "users";
    private static final String MATCHES_URL = SERVER_URL + "chats";
    private static final String CHATMSG_URL = SERVER_URL + "chats";
    private static final String USERINFO_URL = SERVER_URL + "user";

    private StringResourcesHandler() {
    }

    private static String getUrlByType(Integer type) {
        switch (type) {
            case USER_CANDIDATES:
                return CANDIDATES_URL;
            case USER_MATCHES:
                return MATCHES_URL;
            case USER_CHAT:
                return CHATMSG_URL;
            case USER_INFO:
                return USERINFO_URL;
            default:
                return "";
        }
    }

    /**
     * TODO
     *
     * @param requestType
     * @param token
     * @param operation
     */
    public static void executeQuery(int requestType, String token, CallbackOperation operation) {
        executeQuery("", requestType, token, operation);
    }

    /**
     * TODO
     *
     * @param resId
     * @param requestType
     * @param token
     * @param operation
     */
    public static void executeQuery(String resId, int requestType, String token, CallbackOperation operation) {
        FetchDataTask task = new FetchDataTask(requestType, resId, token, operation);
        task.execute();
    }

    private static List<String[]> fetchData(String queryUrl) {
        DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "Begin fetch " + queryUrl);
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

        String result;

        try {
            result = restTemplate.getForObject(queryUrl, String.class, "Android");
        } catch (HttpServerErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
            return null;
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Failed to connect: " + e.getMessage());
            return null;
        }
        DrTinderLogger.writeLog(DrTinderLogger.NET_INFO, "End fetch " + queryUrl);

        StringReader stringReader = new StringReader(result);
        List<String[]> output = new ArrayList<>();
        CSVReader reader = new CSVReader(stringReader);

        while (true) {
            String[] next;

            try {
                next = reader.readNext();
            } catch (IOException e) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "CSV parse error");
                return null;
            }

            if (next == null) {
                return output;
            }
            output.add(next);
        }
    }

    /**
     * TODO
     */
    public interface CallbackOperation {
        void execute(List<String[]> data);
    }

    /**
     * TODO
     */
    private static class FetchDataTask extends AsyncTask<Void, Void, Boolean> {

        private final CallbackOperation mCallbackOp;
        private String mResourceUrl;
        private List<String[]> mData;

        FetchDataTask(int resourceType, String resourceId, String token, CallbackOperation callbackOperation) {
            setImageUrl(resourceType, resourceId, token);
            this.mCallbackOp = callbackOperation;
            this.mData = null;
        }

        private void setImageUrl(int resourceType, String resId, String token) {
            String url = getUrlByType(resourceType);
            Uri.Builder uriBuilder = Uri.parse(url).buildUpon();
            uriBuilder.appendQueryParameter("token", token);
            if (!resId.equals("")) {
                uriBuilder.appendQueryParameter("res_id", resId);
            }
            this.mResourceUrl = uriBuilder.build().toString();
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            mData = fetchData(mResourceUrl);
            return mData != null;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed to get data from server");
            }
            mCallbackOp.execute(mData);
        }

    }
}