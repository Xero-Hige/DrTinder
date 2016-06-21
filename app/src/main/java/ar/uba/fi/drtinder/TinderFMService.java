package ar.uba.fi.drtinder;

import android.os.Binder;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Map;

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
 * TODO
 */
public class TinderFMService extends FirebaseMessagingService {

    /**
     * TODO
     */
    public ChatSession session;

    /**
     * TODO
     *
     * @param remoteMessage
     */
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        if (session == null) {
            return;
        }

        Map<String, String> data = remoteMessage.getData();
        String message = data.get("message");
        String senderId = data.get("senderId");
        session.addResponse(message, senderId);
    }

    /**
     * TODO
     */
    public class LocalBinder extends Binder {
        TinderFMService getService() {
            // Return this instance of TrackingService so clients can call public methods
            return TinderFMService.this;
        }
    }
}