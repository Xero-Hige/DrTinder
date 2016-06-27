package ar.uba.fi.drtinder;

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
 * Singleton that stores the server url. This should be checked.
 */
public final class ServerUrlWrapper {
    private static String serverUrl = "http://190.55.231.26/";

    private ServerUrlWrapper() {
    }

    /**
     * Gets the stored server URL
     *
     * @return Stored server URL
     */
    public static String getServerUrl() {
        return serverUrl;
    }

    /**
     * Sets the server URL
     *
     * @param url New url
     */
    public static void setServerUrl(String url) {
        serverUrl = url;
    }
}
