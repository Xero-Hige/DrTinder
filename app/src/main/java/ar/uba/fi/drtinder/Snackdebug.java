package ar.uba.fi.drtinder;

import android.support.design.widget.Snackbar;
import android.view.View;

/**
 * ${FILE}
 * <p/>
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p/>
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
public class Snackdebug {


    public static final String DEFAULT_BUTTON = "DONE";

    public static void showMessage(String message, View coordinatorLayout) {
        showMessage(message, coordinatorLayout, DEFAULT_BUTTON, Snackbar.LENGTH_LONG);
    }

    public static void showMessage(String message, View coordinatorLayout, String button_string) {
        showMessage(message, coordinatorLayout, button_string, Snackbar.LENGTH_LONG);

    }

    public static void showMessage(String message, View coordinatorLayout, int display_time) {
        showMessage(message, coordinatorLayout, DEFAULT_BUTTON, display_time);
    }

    public static void showMessage(String message, View coordinatorLayout, String button_string, int display_time) {
        Snackbar snackbar = Snackbar
                .make(coordinatorLayout, message, display_time);
        snackbar.setAction(button_string, view -> {
            snackbar.dismiss();
        });
        snackbar.show();
    }
}
