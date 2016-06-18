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
        String[] interests = interestExtra.split("\\|\\|");
        for (String interestString : interests) {
            String[] interest = interestString.split("::");
            String category = interest[0];
            String name = interest[1];

            View layout = inflater.inflate(R.layout.interest_lay, null);
            TextView textView = (TextView) layout.findViewById(R.id.interst_txt);
            String interestLabel = category + ":\n" + name;
            textView.setText(interestLabel);
            ImageView imageView = (ImageView) layout.findViewById(R.id.interst_img);
            ImageResourcesHandler.fillImageResource(name, ImageResourcesHandler.RES_INTEREST_IMG,
                    imageView, this); //FIXME: Cambiar a campo combinado
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

        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG,
                imageView, getBaseContext());
    }
}
