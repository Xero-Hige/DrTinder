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

public class DetailsFragment extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.frament_details);
        Intent intent = getIntent();

        this.setTitle("");

        ImageView img = (ImageView) findViewById(R.id.backdrop);
        int resId = Integer.parseInt(intent.getStringExtra("img"));
        img.setImageResource(resId);

        TextView desc = (TextView) findViewById(R.id.description);
        String description = intent.getStringExtra("name") + " , '" + intent.getStringExtra("age") + "'";
        desc.setText(description);

        TextView bio = (TextView) findViewById(R.id.bios);
        bio.setText(intent.getStringExtra("bio"));

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        LinearLayout bar = (LinearLayout) findViewById(R.id.innerLay);
        LayoutInflater inflater = LayoutInflater.from(this);
        for (int i = 9; i > -1; i--) {
            View layout = inflater.inflate(R.layout.interest_lay, null);
            TextView v = (TextView) layout.findViewById(R.id.interst_txt);
            v.setText("Interes: " + String.valueOf(i));
            ImageView vi = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(R.drawable.ubuntu).fit().centerCrop().into(vi);
            bar.addView(layout);

            layout = inflater.inflate(R.layout.interest_lay, null);
            v = (TextView) layout.findViewById(R.id.interst_txt);
            v.setText("Interes: " + String.valueOf(i));
            vi = (ImageView) layout.findViewById(R.id.interst_img);
            Picasso.with(bar.getContext()).load(R.drawable.logo).fit().centerCrop().into(vi);
            bar.addView(layout);
        }

    }
}
