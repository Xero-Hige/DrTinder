package ar.uba.fi.drtinder;

import android.content.Context;
import android.os.AsyncTask;

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
 * Handler of the listener of new matches
 */
public class MessagesListener {

    private static final int POLLING_INTERVAL = 1000;
    private static ChatListeningTask task;
    private static boolean running = false;
    private static ChatSession mSession = null;


    /**
     * Starts the listening
     *
     * @param token   user token
     * @param context app context
     */
    static void startListening(String token, Context context, ChatSession session) {
        if (running) {
            return;
        }
        running = true;
        mSession = session;
        task = new ChatListeningTask(token, context);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    /**
     * Stops the listening
     */
    static void stopListening() {
        running = false;
        task.cancel(true);
        mSession = null;
    }

    static private class ChatListeningTask extends AsyncTask<Void, Void, Boolean> {

        private final String mToken;
        private final Context mContext;

        ChatListeningTask(String token, Context context) {
            mToken = token;
            mContext = context;
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            DrTinderLogger.writeLog(DrTinderLogger.INFO, "Started chat listener");
            while (running) {
                try {
                    Thread.sleep(POLLING_INTERVAL);
                    CountDownLatch barrier = new CountDownLatch(1);
                    StringResourcesHandler.executeQuery(StringResourcesHandler.SERVICE_CHAT, mToken, data -> {
                        for (int i = 0; i < data.size(); i++) {
                            if (data.get(i).length < 2) {
                                continue;
                            }

                            if (mSession == null) {
                                break;
                            }

                            String userId = data.get(i)[0];
                            String message = data.get(i)[1];

                            mSession.addResponse(userId, message);
                        }
                        barrier.countDown();
                    });
                    barrier.await();
                } catch (InterruptedException e) {
                    DrTinderLogger.writeLog(DrTinderLogger.DEBG, "Listener thread stopped");
                    return true;
                }

            }
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
        }

        @Override
        protected void onCancelled() {
            DrTinderLogger.writeLog(DrTinderLogger.INFO, "Stopped chat Listener");
        }
    }
}
