package ar.uba.fi.drtinder;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;

import java.io.IOException;
import java.util.HashMap;

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
 * TODO
 */
public class UserProfileActivity extends AppCompatActivity {

    /**
     * TODO
     */
    public static final String USER_EXTRA_USERNAME = "name";
    /**
     * TODO
     */
    public static final String USER_EXTRA_USEREMAIL = "email";
    /**
     * TODO
     */
    public static final String USER_EXTRA_TOKEN = "token";

    /**
     * TODO
     */
    public static final String PROFILE_EXTRA_ACTION = "action";
    /**
     * TODO
     */
    public static final String PROFILE_ACTION_CREATE = "Create Profile";
    /**
     * TODO
     */
    public static final String PROFILE_ACTION_UPDATE = "Update Profile";

    private static final int PICK_IMAGE = 100;

    private ImageView mProfilePic;
    private String mActivityAction;
    private String mUsername;
    private String mToken;

    private String mEmail;
    private Bitmap image = null;

    /**
     * View
     */
    private TextView mPasswordView;
    private RadioButton sexMale;
    private RadioButton sexFemale;
    private CheckBox searchingMale;
    private CheckBox searchingFemale;
    private TextView mUserName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        assert toolbar != null;//DEBUG Assert

        mActivityAction = activityIntent.getStringExtra(PROFILE_EXTRA_ACTION);
        mToken = activityIntent.getStringExtra(USER_EXTRA_TOKEN);
        mUsername = activityIntent.getStringExtra(USER_EXTRA_USERNAME);
        assert mActivityAction != null; //DEBUG Assert

        toolbar.setTitle(mActivityAction);

        mProfilePic = (ImageView) findViewById(R.id.profUserAvatar);
        assert mProfilePic != null; //DEBUG Assert

        if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
            ImageResourcesHandler.fillImageResource(mUsername,
                    ImageResourcesHandler.RES_USER_IMG, mToken, mProfilePic, this);
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
        addDeleteButton();
        addFields();

        mPasswordView = (TextView) findViewById(R.id.profPassword);
        assert mPasswordView != null;//DEBUG Assert
        mPasswordView.setEnabled(mActivityAction.equals(PROFILE_ACTION_CREATE));

        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            return;
        }
        StringResourcesHandler.executeQuery(mUsername, StringResourcesHandler.USER_INFO, mToken,
                data -> {
                    String username = data.get(0)[0];
            String age = data.get(0)[1];
            String sex = data.get(0)[2];
            String lookingFor = data.get(0)[3];
            String interest = data.get(0)[4];

            TextView UsernameView = (TextView) findViewById(R.id.profUsername);
            assert UsernameView != null;//DEBUG Assert
            UsernameView.setText(username);

            sexMale.setChecked(sex.equals("Male"));
            sexFemale.setChecked(sex.equals("Female"));
            searchingMale.setChecked(lookingFor.equals("Male") || lookingFor.equals("Both"));
            searchingFemale.setChecked(lookingFor.equals("Female") || lookingFor.equals("Both"));

        });
    }

    private void addFields() {
        sexMale = (RadioButton) findViewById(R.id.sexMaleRadio);
        sexFemale = (RadioButton) findViewById(R.id.sexFemaleRadio);
        searchingMale = (CheckBox) findViewById(R.id.lookingMale);
        searchingFemale = (CheckBox) findViewById(R.id.lookingFemale);
        mUserName = (TextView) findViewById(R.id.profUsername);

        assert sexMale != null; //DEBUG Assert
        assert sexFemale != null; //DEBUG Assert
        assert searchingMale != null; //DEBUG Assert
        assert searchingFemale != null; //DEBUG Assert
        assert mUserName != null; //DEBUG Assert
    }

    private void addDeleteButton() {
        Button deleteProfile = (Button) findViewById(R.id.deleteButton);
        assert deleteProfile != null; //DEBUG Assert
        deleteProfile.setEnabled(mActivityAction.equals(PROFILE_ACTION_UPDATE));
        deleteProfile.setOnClickListener(view -> {
            UserHandler.deleteProfile(mToken);
            Intent intent = new Intent(this, LoginActivity.class);
            startActivity(intent);
            finish();
        });

        View deleteDiv = findViewById(R.id.deleteUserDivider);
        assert deleteDiv != null;
        deleteDiv.setEnabled(mActivityAction.equals(PROFILE_ACTION_UPDATE));
    }

    private void addSubmitButton() {
        Button submit = (Button) findViewById(R.id.submmit);
        assert submit != null; //DEBUG Assert

        String label = null;
        View.OnClickListener clickListener = v -> {
        }; //Empty initialize
        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            label = "Create";
            clickListener = v -> createUser();
        } else if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
            label = "Update";
            clickListener = v -> updateUser();
        }

        assert label != null;

        submit.setText(label);
        submit.setOnClickListener(clickListener);
    }

    private void updateUser() {
        if (image != null) {
            UserHandler.uploadProfilePicture(image, mToken);
        }
        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Updated info");
        finish();
    }

    private void createUser() {
        String password = mPasswordView.getText().toString();

        FirebaseAuth.getInstance().createUserWithEmailAndPassword(mEmail, password)
                .addOnCompleteListener(task -> {
                    if (!task.isSuccessful()) {
                        DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed create user at FB: "
                                + mEmail + " " + password);
                        finish();
                        return;
                    }

                    DrTinderLogger.writeLog(DrTinderLogger.INFO, "Created user at FB");

                    if (image == null) {
                        image = BitmapFactory.decodeResource(getResources(), R.drawable.not_found);
                    }
                    UserHandler.uploadProfilePicture(image, mToken);
                    HashMap<String, String> userdata = getUserdataMap();
                    UserHandler.signUp(mEmail, password, userdata);
                    finish();
                });
    }

    @NonNull
    private HashMap<String, String> getUserdataMap() {
        HashMap<String, String> userdata = new HashMap<>();
        userdata.put("name", mUserName.getText().toString());
        userdata.put("age", "21");//TODO
        userdata.put("sex", sexMale.isChecked() ? "Male" : "Female");

        if (searchingMale.isChecked() && !searchingFemale.isChecked()) {
            userdata.put("lookingFor", "Male");
        } else if (searchingFemale.isChecked() && !searchingMale.isChecked()) {
            userdata.put("lookingFor", "Female");
        } else {
            userdata.put("lookingFor", "Both");
        }
        userdata.put("interest", mUsername);
        return userdata;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && requestCode == PICK_IMAGE) {
            Uri imageUri = data.getData();
            try {
                image = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
            } catch (IOException e) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "The impossible happen. Image IOError");
                return; //No image to display (cant upload)
            }
            mProfilePic.setImageURI(imageUri);

        }
    }
}
