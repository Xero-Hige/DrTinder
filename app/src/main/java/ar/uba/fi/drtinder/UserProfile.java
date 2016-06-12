package ar.uba.fi.drtinder;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;

public class UserProfile extends AppCompatActivity {

    private static final int PICK_IMAGE = 100;
    public static String USER_EXTRA_USERNAME = "name";
    public static String USER_EXTRA_USEREMAIL = "email";

    public static String PROFILE_EXTRA_ACTION = "action";
    public static String PROFILE_ACTION_CREATE = "Create Profile";
    public static String PROFILE_ACTION_UPDATE = "Update Profile";

    private ImageView profilePic;
    private String action;
    private String username;

    private String email;
    private TextView passwordTV;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        assert toolbar != null;//DEBUG Assert

        action = activityIntent.getStringExtra(PROFILE_EXTRA_ACTION);
        assert action != null; //DEBUG Assert

        toolbar.setTitle(action);

        profilePic = (ImageView) findViewById(R.id.userAvatar);
        assert profilePic != null; //DEBUG Assert

        if (action.equals(PROFILE_ACTION_UPDATE)) {
            ImageResourcesHandler.fillImageResource(activityIntent.getStringExtra(USER_EXTRA_USERNAME),
                    ImageResourcesHandler.RES_USER_IMG, profilePic, this);
        }

        if (action.equals(PROFILE_ACTION_CREATE)) {
            email = activityIntent.getStringExtra(USER_EXTRA_USEREMAIL);
            assert email != null;//DEBUG Assert
        }

        profilePic.setOnClickListener(v -> {
            Intent gallery = new Intent(Intent.ACTION_PICK,
                    MediaStore.Images.Media.INTERNAL_CONTENT_URI);
            startActivityForResult(gallery, PICK_IMAGE);
        });

        addSubmitButton();

        passwordTV = (TextView) findViewById(R.id.password);
        assert passwordTV != null;//DEBUG Assert

        passwordTV.setEnabled(action.equals(PROFILE_ACTION_CREATE));
    }

    private void addSubmitButton() {
        Button submit = (Button) findViewById(R.id.submmit);
        assert submit != null; //DEBUG Assert

        String label = null;
        View.OnClickListener clickListener = v -> {
        }; //Empty initialize
        if (action.equals(PROFILE_ACTION_CREATE)) {
            label = "Create";
            clickListener = v -> {
                String password = passwordTV.getText().toString();
                FirebaseAuth.getInstance().createUserWithEmailAndPassword(email, password).addOnCompleteListener(task -> {
                    if (task.isSuccessful()) {
                        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Created user");
                    } else {
                        DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed create user " + email + " " + password);
                    }
                });
                finish();
            };
        } else if (action.equals(PROFILE_ACTION_UPDATE)) {
            label = "Update";
            clickListener = v -> {
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Updated info");
                finish();
            };
        }

        assert label != null;

        submit.setText(label);
        submit.setOnClickListener(clickListener);
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
