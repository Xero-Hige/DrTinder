package ar.uba.fi.drtinder;

import android.os.AsyncTask;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpServerErrorException;
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

    static final int USER_CANDIDATES = 0;
    static final int USER_MATCHES = 1;
    private static final String CANDIDATES_URL = "https://demo2753541.mockable.io/candidates/";
    private static final String MATCHES_URL = "https://demo2753541.mockable.io/matches/";

    private StringResourcesHandler() {
    }

    private static String getUrlByType(Integer type) {
        switch (type) {
            case USER_CANDIDATES:
                return CANDIDATES_URL;
            case USER_MATCHES:
                return MATCHES_URL;
            default:
                return "";
        }
    }

    /**
     * TODO
     * @param resId
     * @param requestType
     * @param operation
     */
    public static void executeQuery(String resId, int requestType, CallbackOperation operation) {
        FetchDataTask task = new FetchDataTask(requestType, resId, operation);
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
        private final String mUrl;
        private List<String[]> mData;

        FetchDataTask(int resourceType, String resourceId, CallbackOperation callbackOperation) {
            mUrl = getUrlByType(resourceType) + resourceId;
            this.mCallbackOp = callbackOperation;
            this.mData = null;
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            mData = fetchData(mUrl);
            return mData != null;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed to get data from server");
                return;
            }
            mCallbackOp.execute(mData);
        }

    }
}
