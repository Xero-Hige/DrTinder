package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
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

        String s = intent.getStringExtra("name") + " , '" + intent.getStringExtra("age") + "'";
        this.setTitle(s);


        ImageView img = (ImageView) findViewById(R.id.backdrop);
        int resId = Integer.parseInt(intent.getStringExtra("img"));
        img.setImageResource(resId);

        TextView bio = (TextView) findViewById(R.id.bios);
        bio.setText(intent.getStringExtra("bio"));

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(view -> Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                .setAction("Action", null).show());
    }
}
