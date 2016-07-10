package ar.uba.fi.drtinder;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.LoaderManager.LoaderCallbacks;
import android.content.Context;
import android.content.CursorLoader;
import android.content.Intent;
import android.content.Loader;
import android.database.Cursor;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;

import java.util.concurrent.CountDownLatch;

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
 * Basic login screen based on Android Studio login Activity template
 */
public class LoginActivity extends Activity implements LoaderCallbacks<Cursor> {

    /**
     * Keep track of the login task to ensure we can cancel it if requested.
     */
    private UserLoginTask mAuthTask = null;

    /**
     * Keep track of the login task to ensure we can cancel it if requested.
     */
    private RegisterTask mRegisterTask = null;

    /**
     * UI references.
     */
    private TextView mEmailTextView;
    private EditText mPasswordTextView;
    private View mProgressView;
    private View mLoginFormView;
    private TextView mUrl;

    private CountDownLatch mLoginLatch;

    /**
     * Called when the activity is starting
     *
     * @param instanceState If the activity is being re-initialized after previously being shut down
     *                      then this Bundle contains the data it most recently supplied in
     *                      onSaveInstanceState(Bundle). Note: Otherwise it is null.
     */
    @Override
    protected void onCreate(Bundle instanceState) {
        super.onCreate(instanceState);
        setContentView(R.layout.activity_login);
        LocationHandler.getLocationString(this); //Forces to fetch gps info

        mLoginLatch = new CountDownLatch(0);
        // Set up the login form.

        mEmailTextView = (EditText) findViewById(R.id.email);
        mPasswordTextView = (EditText) findViewById(R.id.password);
        mPasswordTextView.setOnEditorActionListener((textView, actorId, keyEvent) -> {
            if (actorId == R.id.login || actorId == EditorInfo.IME_NULL) {
                attemptLogin();
                return true;
            }
            return false;
        });

        Button mSignInButton = (Button) findViewById(R.id.email_sign_in_button);
        mSignInButton.setOnClickListener(view -> attemptLogin());

        Button mRegisterButton = (Button) findViewById(R.id.Register);
        mRegisterButton.setOnClickListener(view -> attemptRegister());

        mLoginFormView = findViewById(R.id.login_form);
        mProgressView = findViewById(R.id.login_progress);

        mUrl = (TextView) findViewById(R.id.url);
        mUrl.setEnabled(false);
        mUrl.setVisibility(View.GONE);
        mUrl.setText(ServerUrlWrapper.getServerUrl());

        addLogoShortcut();
    }

    private void addLogoShortcut() {
        ImageView logo = (ImageView) findViewById(R.id.drTinderLogo);
        logo.setOnLongClickListener(v -> {
            if (mUrl.isEnabled()) {
                mUrl.setEnabled(false);
                mUrl.setVisibility(View.GONE);
            } else {
                mUrl.setEnabled(true);
                mUrl.setVisibility(View.VISIBLE);
            }
            return true;
        });
    }

    private void executeWithLData(TaskExecutor executor) {
        if (mAuthTask != null) {
            return;
        }

        // Reset errors.
        mEmailTextView.setError(null);
        mPasswordTextView.setError(null);

        // Store values at the time of the login attempt.
        String email = mEmailTextView.getText().toString();
        String password = mPasswordTextView.getText().toString();

        boolean cancel = false;
        View focusView = null;

        // Check for a valid password, if the user entered one.
        if (!TextUtils.isEmpty(password) && !isPasswordValid(password)) {
            mPasswordTextView.setError(getString(R.string.error_invalid_password));
            focusView = mPasswordTextView;
            cancel = true;
        }

        // Check for a valid email address.
        if (TextUtils.isEmpty(email)) {
            mEmailTextView.setError(getString(R.string.error_field_required));
            focusView = mEmailTextView;
            cancel = true;
        } else if (!isEmailValid(email)) {
            mEmailTextView.setError(getString(R.string.error_invalid_email));
            focusView = mEmailTextView;
            cancel = true;
        }

        if (cancel) {
            // There was an error; don't attempt login and focus the first
            // form field with an error.
            focusView.requestFocus();
        } else {
            // Show a progress spinner, and kick off a background task to
            // perform the user login attempt.
            executor.execute(email, password);
        }
    }

    private void firebaseAuthenticate(String email, String password) {
        mLoginLatch = new CountDownLatch(1);
        FirebaseAuth.getInstance().signInWithEmailAndPassword(email, password)
                .addOnCompleteListener(this, task -> {
                    DrTinderLogger.writeLog(DrTinderLogger.INFO, "Logged in FCM completed.");
                    if (!task.isSuccessful()) {
                        DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Token login in FCM failed");
                    } else {
                        DrTinderLogger.writeLog(DrTinderLogger.INFO, "Successfully login with token in FCM");
                    }
                    mLoginLatch.countDown();
                });
    }

    private void attemptRegister() {
        if (mUrl.isEnabled()) {
            String url = mUrl.getText().toString();
            ServerUrlWrapper.setServerUrl(url);
        }
        executeWithLData(this::executeRegisterTask);
    }

    private void attemptLogin() {
        if (mUrl.isEnabled()) {
            String url = mUrl.getText().toString();
            ServerUrlWrapper.setServerUrl(url);
        }
        executeWithLData(this::executeLoginTask);
    }

    private void executeLoginTask(String email, String password) {
        // Show a progress spinner, and kick off a background task to
        // perform the user login attempt.
        showProgress(true);
        mAuthTask = new UserLoginTask(email, password, this);
        mAuthTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private void showProgress(final boolean show) {
        // On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
        // for very easy animations. If available, use these APIs to fade-in
        // the progress spinner.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = getResources().getInteger(android.R.integer.config_shortAnimTime);

            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
            mLoginFormView.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
                }
            });

            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mProgressView.animate().setDuration(shortAnimTime).alpha(
                    show ? 1 : 0).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
                }
            });
        } else {
            // The ViewPropertyAnimator APIs are not available, so simply show
            // and hide the relevant UI components.
            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }

    private void executeRegisterTask(String email, String password) {
        // Show a progress spinner, and kick off a background task to
        // perform the user login attempt.
        showProgress(true);
        mRegisterTask = new RegisterTask(email, this);
        mRegisterTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private boolean isEmailValid(String email) {
        return UserHandler.isValidEmail(email);
    }

    private boolean isPasswordValid(String password) {
        return UserHandler.isValidPassword(password);
    }

    private String getLocationString() {
        return LocationHandler.getLocationString(this);
    }

    /**
     * Instantiate and return a new Loader for the given ID.
     *
     * @param index  The ID whose loader is to be created.
     * @param bundle Any arguments supplied by the caller.
     * @return Return a new Loader instance that is ready to start loading.
     */
    @Override
    public Loader<Cursor> onCreateLoader(int index, Bundle bundle) {
        return new CursorLoader(this);
    }

    /**
     * Called when a previously created loader has finished its load. Note that normally an
     * application is not allowed to commit fragment transactions while in this call, since
     * it can happen after an activity's state is saved.
     *
     * @param cursorLoader The Loader that has finished
     * @param cursor       The data generated by the Loader.
     */
    @Override
    public void onLoadFinished(Loader<Cursor> cursorLoader, Cursor cursor) {

    }

    /**
     * Called when a previously created loader is being reset, and thus making its data unavailable.
     * The application should at this point remove any references it has to the Loader's data.
     *
     * @param cursorLoader The Loader that is being reset.
     */
    @Override
    public void onLoaderReset(Loader<Cursor> cursorLoader) {

    }

    private void startApp(Context context) {
        Intent menuIntent = new Intent(context, MainActivity.class);
        startActivity(menuIntent);
        finish();
    }

    private interface TaskExecutor {
        void execute(String email, String password);
    }

    /**
     * Represents an asynchronous login/registration task used to authenticate
     * the user.
     */
    private class RegisterTask extends AsyncTask<Void, Void, Boolean> {

        private final String mUserEmail;
        private final Activity mActivity;

        /**
         * Creates a new register task
         *
         * @param email    User email
         * @param activity Calling Activity
         */
        RegisterTask(String email, Activity activity) {
            mUserEmail = email;
            mActivity = activity;
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            return UserHandler.isValidEmail(this.mUserEmail);
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mRegisterTask = null;
            showProgress(false);

            if (success) {
                Intent intent = new Intent(this.mActivity, UserProfileActivity.class);
                intent.putExtra(UserProfileActivity.USER_EXTRA_USEREMAIL, this.mUserEmail);
                intent.putExtra(UserProfileActivity.PROFILE_EXTRA_ACTION, UserProfileActivity.PROFILE_ACTION_CREATE);
                startActivity(intent);
            } else {
                if (mUserEmail.equals("")) {
                    mEmailTextView.setError("Email needed");
                } else if (!UserHandler.isValidEmail(mUserEmail)) {
                    mEmailTextView.setError("Invalid email");
                } else {
                    mEmailTextView.setError("Already in use");
                }
                mEmailTextView.requestFocus();
            }
        }

        @Override
        protected void onCancelled() {
            mRegisterTask = null;
            showProgress(false);
        }
    }

    private class UserLoginTask extends AsyncTask<Void, Void, Boolean> {

        private final String mUserEmail;
        private final String mUserPassword;
        private final Activity mActivity;
        private String mAuthToken;


        UserLoginTask(String email, String password, Activity activity) {
            mUserEmail = email;
            mUserPassword = password;
            mActivity = activity;
        }


        @Override
        protected Boolean doInBackground(Void... params) {
            mAuthToken = UserHandler.getLoginToken(mUserEmail,
                    mUserPassword, getLocationString());
            if (mAuthToken == null) {
                return false;
            }
            if (mAuthToken.equals(UserHandler.ERROR_TOKEN)) {
                return false;
            }
            if (mAuthToken.equals(UserHandler.FAILED_TOKEN)) {
                return false;
            }

            firebaseAuthenticate(mUserEmail, mUserPassword);
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mAuthTask = null;

            if (success) {
                startApp(this.mActivity);
                return;
            }

            if (mAuthToken == null) {
                Utility.showMessage("Error durante el login. Reintente.", Utility.getViewgroup(mActivity));
                showProgress(false);
                return;
            }

            if (mAuthToken.equals(UserHandler.FAILED_TOKEN)) {
                mEmailTextView.setError(getString(R.string.error_failed_login));
                mEmailTextView.requestFocus();
                showProgress(false);
                return;
            }

            showProgress(false);
            ViewGroup viewgroup = Utility.getViewgroup(mActivity);
            Utility.showMessage("Error de conexion con el servidor", viewgroup, "OK");

        }

        @Override
        protected void onCancelled() {
            mAuthTask = null;
            showProgress(false);
        }
    }

}