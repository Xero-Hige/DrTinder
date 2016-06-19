package ar.uba.fi.drtinder;

import android.support.design.widget.Snackbar;
import android.view.View;

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
 * Snackbar class that could be used to output debug info
 */
public final class Snackdebug {

    private static final String DEFAULT_BUTTON = "DONE";

    private Snackdebug() {
    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     */
    public static void showMessage(String message, View coordinatorLayout) {
        showMessage(message, coordinatorLayout, DEFAULT_BUTTON, Snackbar.LENGTH_LONG);
    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     * @param buttonString      String to use as snackbar "ok" button
     */
    public static void showMessage(String message, View coordinatorLayout, String buttonString) {
        showMessage(message, coordinatorLayout, buttonString, Snackbar.LENGTH_LONG);

    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     * @param displayTime       Time that should be the snackbar visible (Snackbar.LENGTH_*)
     */
    public static void showMessage(String message, View coordinatorLayout, int displayTime) {
        showMessage(message, coordinatorLayout, DEFAULT_BUTTON, displayTime);
    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     * @param buttonString      String to use as snackbar "ok" button
     * @param displayTime       Time that should be the snackbar visible (Snackbar.LENGTH_*)
     */
    public static void showMessage(String message, View coordinatorLayout,
                                   String buttonString, int displayTime) {
        Snackbar snackbar = Snackbar
                .make(coordinatorLayout, message, displayTime);
        snackbar.setAction(buttonString, view -> {
            snackbar.dismiss();
        });
        snackbar.show();
    }
}
