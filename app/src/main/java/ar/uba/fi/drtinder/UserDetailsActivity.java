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

import com.squareup.picasso.Picasso;

/**
 * Activity used to display users details
 */
public class UserDetailsActivity extends AppCompatActivity {

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

        setInterestsList();
    }

    /**
     * Creates interests list on the view
     */
    private void setInterestsList() {
        LinearLayout bar = (LinearLayout) findViewById(R.id.innerLay);
        LayoutInflater inflater = LayoutInflater.from(this);
        for (int i = 9; i > -1; i--) {
            View layout = inflater.inflate(R.layout.interest_lay, null);
            TextView textView = (TextView) layout.findViewById(R.id.interst_txt);
            textView.setText("Interes: " + i);
            ImageView imageView = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(
                    R.drawable.ubuntu).fit().centerCrop().into(imageView);
            bar.addView(layout);

            layout = inflater.inflate(R.layout.interest_lay, null);
            textView = (TextView) layout.findViewById(R.id.interst_txt);
            textView.setText("Interes: " + i);
            imageView = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(R.drawable.logo).fit().centerCrop().into(imageView);
            bar.addView(layout);
        }
    }

    private void setUserBio(Intent intent) {
        TextView bio = (TextView) findViewById(R.id.bios);
        bio.setText(intent.getStringExtra(SelectionFragment.EXTRA_USER_BIO));
    }

    private void setUserInfo(Intent intent) {
        TextView desc = (TextView) findViewById(R.id.description);
        String infoDetails = "%s (%s)";
        String name = intent.getStringExtra(SelectionFragment.EXTRA_USER_NAME);
        String age = intent.getStringExtra(SelectionFragment.EXTRA_USER_AGE);
        String description = String.format(infoDetails, name, age);
        desc.setText(description);
    }

    private void setUserImage(Intent intent) {
        ImageView img = (ImageView) findViewById(R.id.backdrop);
        int resId = Integer.parseInt(intent.getStringExtra(SelectionFragment.EXTRA_USER_IMAGE));
        img.setImageResource(resId);
    }
}
