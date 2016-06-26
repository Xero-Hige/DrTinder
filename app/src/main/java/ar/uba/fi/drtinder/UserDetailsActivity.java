package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

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
 * Activity used to display users details
 */
public class UserDetailsActivity extends AppCompatActivity {

    /**
     * User name key in extra map
     */
    public static final String EXTRA_USER_NAME = "name";
    /**
     * User age key in extra map
     */
    public static final String EXTRA_USER_AGE = "age";
    /**
     * User image key in extra map
     */
    public static final String EXTRA_USER_ID = "id";
    /**
     * User biography key in extra map
     */
    public static final String EXTRA_USER_BIO = "bio";
    /**
     * User interest key in extra map
     */
    public static final String EXTRA_USER_INTS = "inte";

    private String mToken;

    /**
     * Called when the activity is starting
     *
     * @param bundle If the activity is being re-initialized after previously being shut down
     *               then this Bundle contains the data it most recently supplied in
     *               onSaveInstanceState(Bundle). Note: Otherwise it is null.
     */
    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.frament_details);
        Intent intent = getIntent();

        this.setTitle(""); //Removes title from picture

        setUserImage(intent);
        setUserInfo(intent);
        setUserBio(intent);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        setInterestsList(intent);
    }

    /**
     * Creates interests list on the view
     */
    private void setInterestsList(Intent intent) {
        LinearLayout listLayout = (LinearLayout) findViewById(R.id.innerLay);
        LayoutInflater inflater = LayoutInflater.from(this);

        String interestExtra = intent.getStringExtra(EXTRA_USER_INTS);
        String[] interests = interestExtra.split(StringResourcesHandler.INTEREST_DIVIDER);
        for (String interestString : interests) {
            String[] interest = interestString.split(StringResourcesHandler.INTEREST_DATA_DIVIDER);
            String category = interest[0];
            String name = interest[1];

            View layout = inflater.inflate(R.layout.interest_lay, null);
            TextView textView = (TextView) layout.findViewById(R.id.interst_txt);
            String interestLabel = category + ":\n" + name;
            textView.setText(interestLabel);
            ImageView imageView = (ImageView) layout.findViewById(R.id.interst_img);
            ImageResourcesHandler.fillImageResource(name + category, ImageResourcesHandler.RES_INTEREST_IMG,
                    mToken, imageView, this);
            assert listLayout != null; //DEBUG Assert
            listLayout.addView(layout);
        }
    }

    private void setUserBio(Intent intent) {
        TextView biography = (TextView) findViewById(R.id.bios);
        assert biography != null;
        biography.setText(intent.getStringExtra(EXTRA_USER_BIO));
    }

    private void setUserInfo(Intent intent) {
        TextView descriptionTView = (TextView) findViewById(R.id.description);
        String infoDetails = "%s (%s)";
        String name = intent.getStringExtra(EXTRA_USER_NAME);
        String age = intent.getStringExtra(EXTRA_USER_AGE);
        String description = String.format(infoDetails, name, age);
        assert descriptionTView != null; //DEBUG Assert
        descriptionTView.setText(description);
    }

    private void setUserImage(Intent intent) {
        ImageView imageView = (ImageView) findViewById(R.id.backdrop);
        String userId = intent.getStringExtra(EXTRA_USER_ID);

        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG, mToken,
                imageView, getBaseContext());
    }
}
