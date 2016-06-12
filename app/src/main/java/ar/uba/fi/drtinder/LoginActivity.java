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
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

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

    // UI references.
    private TextView mEmailTextView;
    private EditText mPasswordTextView;
    private View mProgressView;
    private View mLoginFormView;

    private FirebaseAuth mFirebaseAuth;
    private boolean firebaseLogedIn;


    @Override
    protected void onCreate(Bundle instanceState) {
        super.onCreate(instanceState);
        setContentView(R.layout.activity_login);
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

        mFirebaseAuth = FirebaseAuth.getInstance();
        firebaseLogedIn = false;

        FirebaseUser user = mFirebaseAuth.getCurrentUser();
        if (user != null) {
            startApp(user.getEmail(), this);
        }
    }

    /**
     * Executes a TaskExecutor with the user data from email and password fields
     */
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
        mFirebaseAuth.signInWithEmailAndPassword(email, password)
                .addOnCompleteListener(this, task -> {
            DrTinderLogger.writeLog(DrTinderLogger.INFO, "Logged in FCM completed.");
            if (!task.isSuccessful()) {
                DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Token login in FCM failed");
            } else {
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Successfully login with token in FCM");
            }
            firebaseLogedIn = task.isSuccessful();
        });
    }

    private void attemptRegister() {
        executeWithLData(this::executeRegisterTask);
    }

    private void attemptLogin() {
        executeWithLData(this::executeLoginTask);
    }

    private void executeLoginTask(String email, String password) {
        // Show a progress spinner, and kick off a background task to
        // perform the user login attempt.
        showProgress(true);
        mAuthTask = new UserLoginTask(email, password, this);
        mAuthTask.execute((Void) null);
    }

    private void executeRegisterTask(String email, String password) {
        // Show a progress spinner, and kick off a background task to
        // perform the user login attempt.
        showProgress(true);
        mRegisterTask = new RegisterTask(email, password, this);
        mRegisterTask.execute((Void) null);
    }

    private boolean isEmailValid(String email) {
        return UserInfoHandler.isValidEmail(email);
    }

    private boolean isPasswordValid(String password) {
        return UserInfoHandler.isValidPassword(password);
    }

    /**
     * Shows the progress UI and hides the login form.
     */
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

    private String getLocationString() {
        return LocationHandler.getLocationString(this);
    }

    @Override
    public Loader<Cursor> onCreateLoader(int index, Bundle bundle) {
        return new CursorLoader(this);
    }

    @Override
    public void onLoadFinished(Loader<Cursor> cursorLoader, Cursor cursor) {

    }

    @Override
    public void onLoaderReset(Loader<Cursor> cursorLoader) {

    }

    private void startApp(String userEmail, Context context) {
        Intent menuIntent = new Intent(context, MainActivity.class);
        menuIntent.putExtra("User", userEmail);
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
    public class RegisterTask extends AsyncTask<Void, Void, Boolean> {

        private final String mUserEmail;
        private final String mUserPassword;
        private final Context mTaskContext;
        private String mAuthToken;

        /**
         * Creates a new Login task
         *
         * @param email    User email
         * @param password User password
         * @param context  Calling activity context
         */
        RegisterTask(String email, String password, Context context) {
            mUserEmail = email;
            mUserPassword = password;
            mTaskContext = context;
            mAuthToken = UserInfoHandler.NULL_TOKEN;
        }

        /**
         * @param params params
         * @return Task successful
         */
        @Override
        protected Boolean doInBackground(Void... params) {
            //Todo: Change
            String mAuthToken = UserInfoHandler.getLoginToken(mUserEmail,
                    mUserPassword, getLocationString());
            if (mAuthToken.equals(UserInfoHandler.NULL_TOKEN)) {
                return false;
            }
            firebaseAuthenticate(mUserEmail, mUserPassword);

            return firebaseLogedIn;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mRegisterTask = null;
            showProgress(false);

            if (success) {
                Intent intent = new Intent(this.mTaskContext, UserProfile.class);
                intent.putExtra("User", this.mUserEmail);
                intent.putExtra("Action", "register");
                intent.putExtra("Token", mAuthToken);
                startActivity(intent);
                finish();
            } else {
                mEmailTextView.setError("User already exists");
                mEmailTextView.requestFocus();
            }
        }

        @Override
        protected void onCancelled() {
            mRegisterTask = null;
            showProgress(false);
        }
    }

    public class UserLoginTask extends AsyncTask<Void, Void, Boolean> {

        private final String mUserEmail;
        private final String mUserPassword;
        private final Context mTaskContext;

        /**
         * Creates a new Login task
         *
         * @param email    User email
         * @param password User password
         * @param context  Calling activity context
         */
        UserLoginTask(String email, String password, Context context) {
            mUserEmail = email;
            mUserPassword = password;
            this.mTaskContext = context;
        }

        /**
         * @param params params
         * @return Task successful
         */
        @Override
        protected Boolean doInBackground(Void... params) {
            String mAuthToken = UserInfoHandler.getLoginToken(mUserEmail,
                    mUserPassword, getLocationString());
            if (mAuthToken.equals(UserInfoHandler.NULL_TOKEN)) {
                return false;
            }
            firebaseAuthenticate(mUserEmail, mUserPassword);

            return firebaseLogedIn;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            mAuthTask = null;
            showProgress(false);

            if (success) {
                startApp(mUserEmail, this.mTaskContext);
            } else {
                mPasswordTextView.setError(getString(R.string.error_incorrect_password));
                mPasswordTextView.requestFocus();
            }
        }

        @Override
        protected void onCancelled() {
            mAuthTask = null;
            showProgress(false);
        }
    }

}

