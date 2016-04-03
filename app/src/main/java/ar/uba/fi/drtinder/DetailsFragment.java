package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.widget.ImageView;
import android.widget.TextView;

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

    }
}
