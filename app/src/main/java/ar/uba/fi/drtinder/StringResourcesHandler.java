package ar.uba.fi.drtinder;

import android.os.AsyncTask;

import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import au.com.bytecode.opencsv.CSVReader;

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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 *
 */
public final class StringResourcesHandler {

    static final String CANDIDATES_URL = "https://demo2753541.mockable.io/candidates/";
    static final String MATCHES_URL = "https://demo2753541.mockable.io/matches/";

    static final int USER_CANDIDATES = 0;
    static final int USER_MATCHES = 1;

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

    static void executeQuery(String resId, int resourceType, DataOperation operation) {
        FetchDataTask task = new FetchDataTask(resourceType, resId, operation);
        task.execute();
    }

    private static List<String[]> fetchData(String queryUrl) {
        DrTinderLogger.log(DrTinderLogger.NET_INFO, "Begin fetch " + queryUrl);
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
        String result = restTemplate.getForObject(queryUrl, String.class, "Android");
        DrTinderLogger.log(DrTinderLogger.NET_INFO, "End fetch " + queryUrl);

        StringReader stringReader = new StringReader(result);
        List<String[]> output = new ArrayList<>();
        CSVReader reader = new CSVReader(stringReader);

        while (true) {
            String[] next;

            try {
                next = reader.readNext();
            } catch (IOException e) {
                break;
            }

            if (next != null) {
                output.add(next);
            } else {
                break;
            }
        }

        return output;
    }

    public interface DataOperation {
        void execute(List<String[]> data);
    }

    private static class FetchDataTask extends AsyncTask<Void, Void, Boolean> {

        private final DataOperation mOperation;
        private final String mUrl;
        private List<String[]> mData;

        FetchDataTask(int resourceType, String resId, DataOperation operation) { //FIXME Names
            mUrl = getUrlByType(resourceType) + resId;
            this.mOperation = operation;
            this.mData = null;
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            mData = fetchData(mUrl);
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                return;
            }
            mOperation.execute(mData);
        }

    }
}
