package ar.uba.fi.drtinder;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.TextView;

import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;

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
 * Activity that represents user profile for creation and update purposes
 */
public class UserProfileActivity extends AppCompatActivity {

    /**
     * Intent extra field: User name
     */
    public static final String USER_EXTRA_USERNAME = "name";
    /**
     * Intent extra field: User email
     */
    public static final String USER_EXTRA_USEREMAIL = "email";

    /**
     * Intent extra field: Activity action
     */
    public static final String PROFILE_EXTRA_ACTION = "action";
    /**
     * Profile activity action: Action create new profile
     */
    public static final String PROFILE_ACTION_CREATE = "Create Profile";
    /**
     * Profile activity action: Action update profile
     */
    public static final String PROFILE_ACTION_UPDATE = "Update Profile";

    private static final int PICK_IMAGE = 100;
    private static final String MALE = "man";
    private static final String FEMALE = "woman";
    private static final String CATEGORY_SEX = "sex";

    private ImageView mProfilePic;
    private String mActivityAction;

    private String mEmail;
    private Bitmap mProfileImage = null;

    private TextView mPasswordView;
    private RadioButton mSexMale;
    private RadioButton mSexFemale;
    private CheckBox mSearchingMale;
    private CheckBox mSearchingFemale;
    private TextView mUserName;
    private TextView mAge;
    private TextView mInterestCategory;
    private TextView mInterestId;

    private LinearLayout mInterestLLay;
    private LinkedList<String> mInterestList;
    private Button mInterestButton;
    private Button mDeleteButton;
    private Button mSubmitButton;

    /**
     * Called when the activity is starting
     *
     * @param bundle If the activity is being re-initialized after previously being shut down
     *               then this Bundle contains the data it most recently supplied in
     *               onSaveInstanceState(Bundle). Note: Otherwise it is null.
     */
    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.user_profile_activity);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent activityIntent = getIntent();

        assert toolbar != null; //DEBUG Assert

        mActivityAction = activityIntent.getStringExtra(PROFILE_EXTRA_ACTION);
        String mUsername = activityIntent.getStringExtra(USER_EXTRA_USERNAME);
        assert mActivityAction != null; //DEBUG Assert

        toolbar.setTitle(mActivityAction);

        mProfilePic = (ImageView) findViewById(R.id.profUserAvatar);
        assert mProfilePic != null; //DEBUG Assert

        if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
            ImageResourcesHandler.fillImageResource(mUsername,
                    ImageResourcesHandler.RES_USER_IMG, UserHandler.getToken(),
                    mProfilePic, this.getApplicationContext());
        }

        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            mEmail = activityIntent.getStringExtra(USER_EXTRA_USEREMAIL);
            assert mEmail != null; //DEBUG Assert
        }

        mProfilePic.setOnClickListener(data -> {
            Intent gallery = new Intent(Intent.ACTION_PICK,
                    MediaStore.Images.Media.INTERNAL_CONTENT_URI);
            startActivityForResult(gallery, PICK_IMAGE);
        });

        addSubmitButton();
        addDeleteButton();
        addFields();

        mInterestList = new LinkedList<>();

        addAddButton();

        mPasswordView = (TextView) findViewById(R.id.profPassword);
        assert mPasswordView != null; //DEBUG Assert
        mPasswordView.setVisibility(mActivityAction.equals(PROFILE_ACTION_CREATE) ? View.VISIBLE : View.GONE);
        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            return;
        }
        loadOldData(mUsername);
    }

    private void loadOldData(String mUsername) {
        StringResourcesHandler.executeQuery(mUsername, StringResourcesHandler.USER_INFO, UserHandler.getToken(),
                data -> {
                    if (data == null) {
                        Utility.showMessage("Error de conexion con el servidor", Utility.getViewgroup(this), "Ok");
                        return;
                    }

                    if (data.size() == 0) {
                        Utility.showMessage("Error de datos recibidos.\nContactese con soporte",
                                Utility.getViewgroup(this), "Ok");
                        return;
                    }

                    String username = "";
                    String age = "";
                    String sex = "";
                    String interest = "";
                    try {
                        username = data.get(0)[1];
                        age = data.get(0)[2];
                        sex = data.get(0)[5];
                        interest = data.get(0)[7];
                    } catch (ArrayIndexOutOfBoundsException e) {
                        DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO,
                                "Userdata doesn't have the required number of fields");
                        Utility.showMessage("Error en la recepcion de datos",
                                Utility.getViewgroup(this));
                        //return;
                    }

                    TextView usernameView = (TextView) findViewById(R.id.profUsername);
                    assert usernameView != null; //DEBUG Assert
                    usernameView.setText(username);

                    mAge.setText(age);
                    mSexMale.setChecked(sex.equals(MALE));
                    mSexFemale.setChecked(sex.equals(FEMALE));
                    mSearchingMale.setChecked(false);
                    mSearchingFemale.setChecked(false);

                    String[] interests = interest.split("\\|\\|");
                    for (String interest1 : interests) {
                        DrTinderLogger.writeLog(DrTinderLogger.DEBG, "Interest: " + interest1);
                        String[] params = interest1.split(StringResourcesHandler.INTEREST_DATA_DIVIDER);
                        if (params.length < 2) {
                            continue;
                        }
                        addInterest(params[0], params[1]);
                    }

                });
    }

    private void addAddButton() {
        this.mInterestButton = (Button) findViewById(R.id.addInterest);
        assert mInterestButton != null; //DEBUG Assert

        mInterestButton.setOnClickListener(view -> {
            String category = mInterestCategory.getText().toString();
            String id = mInterestId.getText().toString();
            if (category.equals("") || id.equals("")) {
                Utility.showMessage("Faltan datos del interes a agregar",
                        Utility.getViewgroup(this), "Ok");
            }
            addInterest(category, id);

            mInterestCategory.setText("");
            mInterestId.setText("");
        });
    }

    private void addInterest(String category, String id) {

        String trimmedCategory = category.replace("  ", " ").trim();
        String trimmedId = id.replace("  ", " ").trim();

        if (trimmedCategory.equals(CATEGORY_SEX)) {
            setLookingFor(trimmedId);
            return;
        }

        LayoutInflater inflater = LayoutInflater.from(this);

        View layout = inflater.inflate(R.layout.interest_lay, Utility.getViewgroup(this), false);
        TextView textView = (TextView) layout.findViewById(R.id.interst_txt);
        String interestLabel = trimmedCategory + ":\n" + trimmedId;
        textView.setText(interestLabel);
        ImageView imageView = (ImageView) layout.findViewById(R.id.interst_img);
        ImageResourcesHandler.fillImageResource(trimmedId + trimmedCategory,
                ImageResourcesHandler.RES_INTEREST_IMG, UserHandler.getToken(),
                imageView, this.getApplicationContext());
        mInterestLLay.addView(layout);
        mInterestList.add(trimmedCategory + StringResourcesHandler.INTEREST_DATA_DIVIDER + trimmedId);
    }

    private void setLookingFor(String id) {
        if (id.equals(MALE)) {
            mSearchingMale.setChecked(true);
        }

        if (id.equals(FEMALE)) {
            mSearchingFemale.setChecked(true);
        }
    }

    private void addFields() {
        mSexMale = (RadioButton) findViewById(R.id.sexMaleRadio);
        mSexFemale = (RadioButton) findViewById(R.id.sexFemaleRadio);
        mSearchingMale = (CheckBox) findViewById(R.id.lookingMale);
        mSearchingFemale = (CheckBox) findViewById(R.id.lookingFemale);
        mUserName = (TextView) findViewById(R.id.profUsername);
        mAge = (TextView) findViewById(R.id.profAge);
        mInterestLLay = (LinearLayout) findViewById(R.id.profInterest);
        mInterestCategory = (TextView) findViewById(R.id.interestCategory);
        mInterestId = (TextView) findViewById(R.id.interestId);

        assert mInterestCategory != null; //DEBUG Assert
        assert mInterestId != null; //DEBUG Assert
        assert mInterestLLay != null; //DEBUG Assert
        assert mSexMale != null; //DEBUG Assert
        assert mSexFemale != null; //DEBUG Assert
        assert mSearchingMale != null; //DEBUG Assert
        assert mSearchingFemale != null; //DEBUG Assert
        assert mUserName != null; //DEBUG Assert
        assert mAge != null; //DEBUG Assert
    }

    private void addDeleteButton() {
        this.mDeleteButton = (Button) findViewById(R.id.deleteButton);
        assert mDeleteButton != null; //DEBUG Assert
        mDeleteButton.setVisibility(mActivityAction.equals(PROFILE_ACTION_UPDATE) ? View.VISIBLE : View.GONE);
        mDeleteButton.setOnClickListener(view -> {
            disableButtons();
            Utility.showMessage("Borrando usuario", Utility.getViewgroup(this));
            DeleteUserTask task = new DeleteUserTask(this);
            task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        });

        View deleteDiv = findViewById(R.id.deleteUserDivider);
        assert deleteDiv != null;
        deleteDiv.setVisibility(mActivityAction.equals(PROFILE_ACTION_UPDATE) ? View.VISIBLE : View.GONE);
    }

    private void addSubmitButton() {
        this.mSubmitButton = (Button) findViewById(R.id.submmit);
        assert mSubmitButton != null; //DEBUG Assert

        String label = null;
        View.OnClickListener clickListener = data -> {
        }; //Empty initialize
        if (mActivityAction.equals(PROFILE_ACTION_CREATE)) {
            label = "Create";
            clickListener = data -> createUser();
        } else if (mActivityAction.equals(PROFILE_ACTION_UPDATE)) {
            label = "Update";
            clickListener = data -> updateUser();
        }

        assert label != null;

        mSubmitButton.setText(label);
        mSubmitButton.setOnClickListener(clickListener);
    }

    private void updateUser() {
        if (!validateFields()) {
            ViewGroup viewGroup = Utility.getViewgroup(this);
            Utility.showMessage("Datos incorrectos en campos obligatorios", viewGroup);
            return;
        }

        disableButtons();
        Utility.showMessage("Actualizando datos", Utility.getViewgroup(this));
        UpdateInfoTask task = new UpdateInfoTask(this);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void createUser() {
        ViewGroup viewGroup = Utility.getViewgroup(this);

        if (!validateFields()) {
            Utility.showMessage("Datos incorrectos en campos obligatorios", viewGroup);
            return;
        }

        Utility.showMessage("Creando usuario", viewGroup);
        String password = mPasswordView.getText().toString();

        if (mProfileImage == null) {
            mProfileImage = BitmapFactory.decodeResource(getResources(), R.drawable.not_found);
        }
        CreateUserTask createTask = new CreateUserTask(this, password);
        createTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        disableButtons();
    }

    private boolean validateFields() {
        String name = mUserName.getText().toString();
        String sAge = mAge.getText().toString();

        if (sAge.equals("") || name.equals("")) {
            return false;
        }

        int age = Integer.parseInt(sAge);
        return age >= 18;
    }

    @NonNull
    private HashMap<String, String> getUserdataMap() {
        HashMap<String, String> userdata = new HashMap<>();
        userdata.put("name", mUserName.getText().toString());
        userdata.put("age", mAge.getText().toString());
        userdata.put("sex", mSexMale.isChecked() ? MALE : FEMALE);
        userdata.put("localization", LocationHandler.getLocationString(this));

        String interests = "";
        for (int i = 0; i < mInterestList.size(); i++) {
            interests = interests + mInterestList.get(i);
            interests = interests + StringResourcesHandler.INTEREST_DIVIDER;
        }

        if (mSearchingMale.isChecked()) {
            interests += CATEGORY_SEX + StringResourcesHandler.INTEREST_DATA_DIVIDER + MALE
                    + StringResourcesHandler.INTEREST_DIVIDER;
        }

        if (mSearchingFemale.isChecked()) {
            interests += CATEGORY_SEX + StringResourcesHandler.INTEREST_DATA_DIVIDER + FEMALE
                    + StringResourcesHandler.INTEREST_DIVIDER;
        }

        if (!interests.equals("")) {
            interests = interests.substring(0,
                    interests.length() - StringResourcesHandler.INTEREST_DIVIDER.length());
        }
        userdata.put("interest", interests);
        return userdata;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && requestCode == PICK_IMAGE) {
            Uri imageUri = data.getData();
            try {
                mProfileImage = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
            } catch (IOException e) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "The impossible happen. Image IOError");
                return; //No image to display (cant upload)
            }
            mProfilePic.setImageURI(imageUri);
        }
    }

    private void enableButtons() {
        this.mDeleteButton.setEnabled(true);
        this.mSubmitButton.setEnabled(true);
        this.mInterestButton.setEnabled(true);
    }

    private void disableButtons() {
        this.mDeleteButton.setEnabled(false);
        this.mSubmitButton.setEnabled(false);
        this.mInterestButton.setEnabled(false);
    }

    private class UpdateInfoTask extends AsyncTask<Void, Void, Boolean> {

        private Activity mContext;

        UpdateInfoTask(Activity context) {
            mContext = context;
        }

        @Override
        protected Boolean doInBackground(Void... voids) {
            if (mProfileImage != null) {
                UserHandler.uploadProfilePicture(mProfileImage, UserHandler.getToken());
            }
            return UserHandler.updateInfo(UserHandler.getToken(), getUserdataMap());
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                Utility.showMessage("Fallo al actualizar. Intente nuevamente",
                        Utility.getViewgroup(mContext), "Ok");
                enableButtons();
                return;
            }
            DrTinderLogger.writeLog(DrTinderLogger.INFO, "Updated info");
            mContext.finish();
        }
    }

    private class CreateUserTask extends AsyncTask<Void, Void, Boolean> {

        private Activity mContext;
        private String mPasswd;
        private String mResult;

        CreateUserTask(Activity context, String pass) {
            mContext = context;
            mPasswd = pass;
        }

        @Override
        protected Boolean doInBackground(Void... voids) {
            mResult = UserHandler.signUp(mEmail, mPasswd, getUserdataMap());
            UserHandler.uploadProfilePicture(mProfileImage, UserHandler.getToken());
            return mResult.equals(UserHandler.SIGNUP_SUCCESS);
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                if (mResult.equals(UserHandler.SIGNUP_FAILED)) {
                    Utility.showMessage("Fallo al crear el usuario. Intente mas tarde",
                            Utility.getViewgroup(mContext), "Ok");
                    enableButtons();
                    return;
                }
                assert false; //DEBUG: should not reach here
            }
            mContext.finish();
        }
    }


    private class DeleteUserTask extends AsyncTask<Void, Void, Boolean> {

        private Activity mContext;

        DeleteUserTask(Activity context) {
            mContext = context;
        }

        @Override
        protected Boolean doInBackground(Void... voids) {
            boolean success = UserHandler.deleteProfile(UserHandler.getToken());
            if (success) {
                Utility.showMessage("Usuario eliminado", Utility.getViewgroup(mContext), "Ok");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    return true;
                }
            }
            return success;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (!success) {
                Utility.showMessage("Fallo al borrar el usuario. Intente mas tarde",
                        Utility.getViewgroup(mContext), "Ok");
                enableButtons();
                return;
            }
            Intent intent = new Intent(mContext, LoginActivity.class);
            startActivity(intent);
            mContext.finish();
        }
    }
}
