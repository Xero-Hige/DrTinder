package ar.uba.fi.drtinder;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.daprlabs.cardstack.SwipeDeck;
import com.squareup.picasso.Picasso;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

/**
 * Swipe selection activity
 */
public class SelectionFragment extends Fragment {

    public static final String EXTRA_USER_NAME = "name";
    public static final String EXTRA_USER_AGE = "age";
    public static final String EXTRA_USER_IMAGE = "img";
    public static final String EXTRA_USER_BIO = "bio";
    //TODO: Remove
    private SwipeDeck mCardStack;

    private boolean mShowSwipeResult = true;
    private View mProgressView;

    private Queue<Map<String, String>> mUsersQueue;
    private Map<Integer, Map<String, String>> mUsersData;
    private View fragmentView;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        fragmentView = inflater.inflate(R.layout.activity_selection, container, false);

        mCardStack = (SwipeDeck) fragmentView.findViewById(R.id.swipe_deck);
        mCardStack.setHardwareAccelerationEnabled(true);

        requestUsersData();

        setCardsAdapter(fragmentView);
        mCardStack.setLeftImage(R.id.card_nope);
        mCardStack.setRightImage(R.id.card_like);
        mProgressView = fragmentView.findViewById(R.id.login_progress);

        setButtons(fragmentView);

        return fragmentView;
    }

    private void setButtons(View view) {
        FloatingActionButton btn = (FloatingActionButton) view.findViewById(R.id.nope_button);
        btn.setBackgroundTintList(ColorStateList.valueOf(-1)); //FIXME
        btn.setRippleColor(-3355444); //FIXME
        btn.setOnClickListener(newView -> mCardStack.swipeTopCardLeft(90));

        FloatingActionButton btn2 = (FloatingActionButton) view.findViewById(R.id.like_button);
        btn2.setBackgroundTintList(ColorStateList.valueOf(-1)); //FIXME
        btn2.setRippleColor(-3355444); //FIXME
        btn2.setOnClickListener(newView -> mCardStack.swipeTopCardRight(90));
    }

    private void setCardsAdapter(View view) {
        SwipeDeckAdapter mCardDeckAdapter = new SwipeDeckAdapter(view.getContext());
        mCardStack.setAdapter(mCardDeckAdapter);

        mCardStack.setEventCallback(new SwipeDeck.SwipeEventCallback() {
            //FIXME: remove debug information
            @Override
            public void cardSwipedLeft(int position) {
                Map<String, String> data = mUsersQueue.remove();
                Snackdebug.showMessage("No te gusto " + data.get(EXTRA_USER_NAME), getView());
                mShowSwipeResult = false;
            }

            @Override
            public void cardSwipedRight(int position) {
                Map<String, String> data = mUsersQueue.remove();
                Snackdebug.showMessage("Te gustó " + data.get(EXTRA_USER_NAME), getView());
                mShowSwipeResult = false;
            }

            @Override
            public void cardsDepleted() {
                showProgress(true);
                UsersFetchTask mAuthTask = new UsersFetchTask(getContext());
                mAuthTask.execute((Void) null);

                Snackdebug.showMessage("No more cats", getView());

            }

            @Override
            public void cardActionDown() {
                Snackdebug.showMessage("Down", getView());

            }

            @Override
            public void cardActionUp() {
                if (!mShowSwipeResult) {
                    mShowSwipeResult = true;
                    return;
                }
                Log.i(" ", "cardActionUp");
                Snackdebug.showMessage("Up", getView());
                showActualUserData();
            }

        });
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

            mCardStack.setVisibility(show ? View.GONE : View.VISIBLE);
            mCardStack.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mCardStack.setVisibility(show ? View.GONE : View.VISIBLE);
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
            mCardStack.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }

    private void showActualUserData() {
        Intent menuIntent = new Intent(getContext(), UserDetailsActivity.class);
        Map<String, String> data = mUsersQueue.peek();
        menuIntent.putExtra(EXTRA_USER_NAME, data.get(EXTRA_USER_NAME));
        menuIntent.putExtra(EXTRA_USER_AGE, data.get(EXTRA_USER_AGE));
        menuIntent.putExtra(EXTRA_USER_IMAGE, data.get(EXTRA_USER_IMAGE));
        menuIntent.putExtra(EXTRA_USER_BIO, data.get(EXTRA_USER_BIO));
        startActivity(menuIntent);
    }

    private void requestUsersData() {
        //TODO: Remove testing data

        mUsersQueue = new LinkedList<>();
        mUsersData = new HashMap<>();


        addUserCard(0, "Gata 1", "21", String.valueOf(R.drawable.gato_3), "Una Gata");
        addUserCard(1, "Gata 2", "21", String.valueOf(R.drawable.gato_3), "Una Gata");
        addUserCard(2, "Gata 3", "21", String.valueOf(R.drawable.gato_3), "Una Gata");

        String bio = "EN ESTE HIPERMERCADO SECTOR CARNES, BUSCO HACER LA"
                + " DIFERENCIA. Soy de Zona sur, tengo 35 años y busco algun chico lindo e"
                + " interesante para conocer y salir. Sin apuros, pero con buenas intenciones. Soy"
                + " copada, simpatica y compañera. Buena gente, positiva y siempre para adelante. "
                + "Si queres conocerme, dale like o super like (previa lectura de mi simpatica"
                + " plaquita) =)";

        addUserCard(3, "Tienda", "25", String.valueOf(R.drawable.gato_4), bio);

        bio = "Joven estudiante de 21 años que cursa la carrera de Literatura"
                + "en la Universidad de Washington. Vivo con mi mejor amiga, Katherine Kavanagh, "
                + "quien escribe para el periódico estudiantil de su universidad.";

        addUserCard(4, "Anastasia Steele", "21", String.valueOf(R.drawable.gato_5), bio);

        //TODO: remove testing data
    }

    private void addUserCard(int index, String name, String age, String image, String bio) {
        Map<String, String> userMap = new HashMap<>();
        userMap.put(EXTRA_USER_NAME, name);
        userMap.put(EXTRA_USER_AGE, age);
        userMap.put(EXTRA_USER_IMAGE, image);
        userMap.put(EXTRA_USER_BIO, bio);

        mUsersData.put(index, userMap);
        mUsersQueue.add(userMap);
    }

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    /**
     * Represents an asynchronous login/registration task used to authenticate
     * the user.
     */
    public class UsersFetchTask extends AsyncTask<Void, Void, Boolean> {

        private final Context mTaskContext;

        /**
         * Creates a new Login task
         *
         * @param context Calling activity context
         */
        UsersFetchTask(Context context) {
            this.mTaskContext = context;
        }

        /**
         * @param params params
         * @return Task successful
         */
        @Override
        protected Boolean doInBackground(Void... params) {
            try {
                // Simulate network access.
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                return false;
            }
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            showProgress(false);

            if (success) {
                requestUsersData();
                setCardsAdapter(fragmentView);
                showProgress(false);
            } else {
                Snackdebug.showMessage("Really, no more cats", getView());
            }
        }

        @Override
        protected void onCancelled() {
            requestUsersData();
            setCardsAdapter(fragmentView);
            showProgress(false);
        }
    }

    /**
     * Adapter for swipe deck
     */
    public class SwipeDeckAdapter extends BaseAdapter {

        private Context mContext;

        /**
         * @param context Caller context
         */
        public SwipeDeckAdapter(Context context) {

            this.mContext = context;
        }

        @Override
        public int getCount() {
            return mUsersData.size();
        }

        @Override
        public Object getItem(int position) {
            return mUsersData.get(position).get(EXTRA_USER_NAME);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(final int position, View convertView, ViewGroup parent) {

            View view = convertView;

            if (view == null) {
                LayoutInflater inflater = getActivity().getLayoutInflater();
                view = inflater.inflate(R.layout.card, parent, false);
            }

            ViewGroup.LayoutParams layoutParams = view.getLayoutParams();
            layoutParams.height = (int) getActivity().getResources().getDimension(
                    R.dimen.activity_horizontal_margin);
            view.setLayoutParams(layoutParams);

            ImageView imageView = (ImageView) view.findViewById(R.id.card_picture);
            Picasso.with(mContext).load(getResourceId(position)).fit().centerCrop().into(imageView);
            TextView textView = (TextView) view.findViewById(R.id.card_text);

            String item = mUsersData.get(position).get(EXTRA_USER_NAME) + " , '"
                    + mUsersData.get(position).get(EXTRA_USER_AGE) + "'";

            textView.setText(item);

            view.setOnClickListener(clickListener -> {
                String item1 = (String) getItem(position);
                Log.i("MainActivity", item1);
            });

            return view;
        }

        private Integer getResourceId(int position) {
            return Integer.parseInt(mUsersData.get(position).get(EXTRA_USER_IMAGE));
        }
    }

}
