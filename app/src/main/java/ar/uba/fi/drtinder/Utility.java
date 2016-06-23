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

import android.app.Activity;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;

/**
 * TODO
 */
public final class Utility {

    private static final String DEFAULT_BUTTON = "DONE";

    private Utility() {
    }

    /**
     * TODO
     *
     * @param context
     */
    public static void hideKeyboard(Activity context) {
        InputMethodManager inputMManager
                = (InputMethodManager) context.getSystemService(Activity.INPUT_METHOD_SERVICE);
        View currentFocus = context.getCurrentFocus();
        if (currentFocus == null) {
            return;
        }
        inputMManager.hideSoftInputFromWindow(currentFocus.getWindowToken(), 0);
    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     */
    public static void showMessage(String message, View coordinatorLayout) {
        showMessage(message, coordinatorLayout, DEFAULT_BUTTON, android.support.design.widget.Snackbar.LENGTH_LONG);
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
        if (coordinatorLayout == null) {
            return;
        }
        android.support.design.widget.Snackbar snackbar = android.support.design.widget.Snackbar
                .make(coordinatorLayout, message, displayTime);
        snackbar.setAction(buttonString, view -> {
            snackbar.dismiss();
        });
        snackbar.show();
    }

    /**
     * Show a message in a snackbar
     *
     * @param message           Message to show
     * @param coordinatorLayout layout coordinator where snackbar should be displayed
     * @param buttonString      String to use as snackbar "ok" button
     */
    public static void showMessage(String message, View coordinatorLayout, String buttonString) {
        showMessage(message, coordinatorLayout, buttonString, android.support.design.widget.Snackbar.LENGTH_LONG);

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
     * Gets the viewgroup corresponding to an activity
     *
     * @param activity Activity from where are you
     * @return Activity viewgroup
     */
    public static ViewGroup getViewgroup(Activity activity) {
        return (ViewGroup) ((ViewGroup) activity
                .findViewById(android.R.id.content)).getChildAt(0);
    }

}
