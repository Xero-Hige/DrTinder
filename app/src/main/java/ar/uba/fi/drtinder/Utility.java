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
import android.view.inputmethod.InputMethodManager;

/**
 * TODO
 */
public class Utility {

    private Utility() {
    }

    /**
     * TODO
     *
     * @param context
     */
    public static void hideKeyboard(Activity context) {
        InputMethodManager inputMManager =
                (InputMethodManager) context.getSystemService(Activity.INPUT_METHOD_SERVICE);
        inputMManager.hideSoftInputFromWindow(context.getCurrentFocus().getWindowToken(), 0);
    }

}
