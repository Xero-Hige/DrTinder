package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.widget.ImageView;

public class UserProfile extends AppCompatActivity {

    public static String USER_EXTRA_USERNAME = "name";
    public static String PROFILE_EXTRA_ACTION = "action";

    public static String PROFILE_ACTION_CREATE = "create";
    public static String PROFILE_ACTION_UPDATE = "Update Profile";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        toolbar.setTitle(activityIntent.getStringExtra(PROFILE_ACTION_UPDATE));

        ImageView profilePic = (ImageView) findViewById(R.id.userAvatar);
        ImageResourcesHandler.fillImageResource(activityIntent.getStringExtra(USER_EXTRA_USERNAME), ImageResourcesHandler.RES_USER_IMG, profilePic, this);

    }
}
