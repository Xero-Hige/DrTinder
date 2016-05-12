package ar.uba.fi.drtinder;

import android.util.Log;

import java.util.Locale;

/**
 * ${FILE}
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
public class DrTinderLogger {

    public static final int INFO = 1;
    public static final int WARN = 2;
    public static final int ERRO = 4;
    public static final int NET_INFO = 8;
    private static final String TAG = "Dr.Tinder";

    private static void _log(int level, String message) {
        switch (level) {
            case WARN:
                Log.w(TAG, message);
            case ERRO:
                Log.e(TAG, message);
            case INFO:
            case NET_INFO:
                Log.i(TAG, message);
            default:
        }
    }


    private static String getLabel(int level) {
        switch (level) {
            case WARN:
                return "WARNING";
            case ERRO:
                return "ERROR";
            case INFO:
                return "INFO";
            case NET_INFO:
                return "NETWORK INFO";
            default:
                return ""; //TODO: Exception
        }
    }

    public static void log(int level, String message) {
        String printLabel = getLabel(level);

        String output = String.format(Locale.ENGLISH, "<%s> %s", printLabel, message);
        _log(level, output);
    }
}
