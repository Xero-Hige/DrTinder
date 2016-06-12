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

    public static final String USER_EXTRA_USERNAME = "name";
    public static final String USER_EXTRA_USEREMAIL = "email";
    public static final String PROFILE_EXTRA_ACTION = "action";
    public static final String PROFILE_ACTION_CREATE = "Create Profile";
    public static final String PROFILE_ACTION_UPDATE = "Update Profile";
    private static final int PICK_IMAGE = 100;

    private ImageView mProfilePic;
    private String mActivityAction;
    private String mUsername;

    private String mEmail;
    private TextView mPasswordView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        assert toolbar != null;//DEBUG Assert

        mActivityAction = activityIntent.getStringExtra(PROFILE_EXTRA_ACTION);
        assert mActivityAction != null; //DEBUG Assert

        toolbar.setTitle(mActivityAction);

        mProfilePic = (ImageView) findViewById(R.id.userAvatar);
        assert mProfilePic != null; //DEBUG Assert

        if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
            ImageResourcesHandler.fillImageResource(activityIntent.getStringExtra(USER_EXTRA_USERNAME),
                    ImageResourcesHandler.RES_USER_IMG, mProfilePic, this);
        }

        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            mEmail = activityIntent.getStringExtra(USER_EXTRA_USEREMAIL);
            assert mEmail != null;//DEBUG Assert
        }

        mProfilePic.setOnClickListener(v -> {
            Intent gallery = new Intent(Intent.ACTION_PICK,
                    MediaStore.Images.Media.INTERNAL_CONTENT_URI);
            startActivityForResult(gallery, PICK_IMAGE);
        });

        addSubmitButton();

        mPasswordView = (TextView) findViewById(R.id.password);
        assert mPasswordView != null;//DEBUG Assert

        mPasswordView.setEnabled(mActivityAction.equals(PROFILE_ACTION_CREATE));
    }

    private void addSubmitButton() {
        Button submit = (Button) findViewById(R.id.submmit);
        assert submit != null; //DEBUG Assert

        String label = null;
        View.OnClickListener clickListener = v -> {
        }; //Empty initialize
        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            label = "Create";
            clickListener = v -> {
                String password = mPasswordView.getText().toString();
                FirebaseAuth.getInstance().createUserWithEmailAndPassword(mEmail, password)
                        .addOnCompleteListener(task -> {
                    if (task.isSuccessful()) {
                        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Created user");
                    } else {
                        DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed create user "
                                + mEmail + " " + password);
                    }
                });
                finish();
            };
        } else if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
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
            mProfilePic.setImageURI(imageUri);
        }
    }
}
