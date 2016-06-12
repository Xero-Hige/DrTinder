package ar.uba.fi.drtinder;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.widget.ImageView;

public class UserProfile extends AppCompatActivity {

    private static final int PICK_IMAGE = 100;
    public static String USER_EXTRA_USERNAME = "name";
    public static String PROFILE_EXTRA_ACTION = "action";
    public static String PROFILE_ACTION_CREATE = "Create Profile";
    public static String PROFILE_ACTION_UPDATE = "Update Profile";

    private ImageView profilePic;
    private String action;
    private String username;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        assert toolbar != null;//DEBUG Assert

        action = activityIntent.getStringExtra(PROFILE_EXTRA_ACTION);
        toolbar.setTitle(action);

        profilePic = (ImageView) findViewById(R.id.userAvatar);
        ImageResourcesHandler.fillImageResource(activityIntent.getStringExtra(USER_EXTRA_USERNAME),
                ImageResourcesHandler.RES_USER_IMG, profilePic, this);

        assert profilePic != null; //DEBUG Assert

        profilePic.setOnClickListener(v -> {
            Intent gallery = new Intent(Intent.ACTION_PICK,
                    MediaStore.Images.Media.INTERNAL_CONTENT_URI);
            startActivityForResult(gallery, PICK_IMAGE);
        });

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && requestCode == PICK_IMAGE) {
            Uri imageUri = data.getData();
            profilePic.setImageURI(imageUri);
        }
    }
}
