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
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.daprlabs.cardstack.SwipeDeck;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Random;

/**
 * Swipe selection fragment. Where you can choose if you like someone o not.
 */
public class SelectionFragment extends Fragment {

    /**
     * User name key in extra map
     */
    public static final String EXTRA_USER_NAME = "name";
    /**
     * User age key in extra map
     */
    public static final String EXTRA_USER_AGE = "age";
    /**
     * User image key in extra map
     */
    public static final String EXTRA_USER_ID = "id";
    /**
     * User biography key in extra map
     */
    public static final String EXTRA_USER_BIO = "bio";
    /**
     * User interest key in extra map
     */
    public static final String EXTRA_USER_INTS = "inte";

    private static final int USER_NAME = 0;
    private static final int USER_AGE = 1;
    private static final int USER_ID = 2;
    private static final int USER_BIO = 3;
    private static final int USER_INTS = 4;

    private static final int USER_FIELDS = 5;

    //TODO: Remove
    private SwipeDeck mCardStack;

    private View mProgressView;

    private Queue<Map<String, String>> mUsersQueue;
    private Map<Integer, Map<String, String>> mUsersData;
    private View mFragmentView;

    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        mFragmentView = inflater.inflate(R.layout.activity_selection, container, false);

        mCardStack = (SwipeDeck) mFragmentView.findViewById(R.id.swipe_deck);
        mCardStack.setHardwareAccelerationEnabled(true);

        mCardStack.setLeftImage(R.id.card_nope);
        mCardStack.setRightImage(R.id.card_like);
        mProgressView = mFragmentView.findViewById(R.id.login_progress);

        setButtons(mFragmentView);

        fillCardStack();

        return mFragmentView;
    }

    private void fillCardStack() {
        showProgress(true);
        UsersFetchTask mAuthTask = new UsersFetchTask();
        mAuthTask.execute((Void) null);
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
                if (mUsersQueue.isEmpty()){
                    return;
                }
                Map<String, String> data = mUsersQueue.remove();
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Rejected " + data.get(EXTRA_USER_NAME));
                ImageResourcesHandler.freeCachedResource(data.get(EXTRA_USER_ID),
                        ImageResourcesHandler.RES_USER_IMG, getContext());
            }

            @Override
            public void cardSwipedRight(int position) {
                if(mUsersQueue.isEmpty()){
                    return;
                }
                Map<String, String> data = mUsersQueue.remove();
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Liked  " + data.get(EXTRA_USER_NAME));
                ImageResourcesHandler.freeCachedResource(data.get(EXTRA_USER_ID),
                        ImageResourcesHandler.RES_USER_IMG, getContext());
            }

            @Override
            public void cardsDepleted() {
                fillCardStack();
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "No more cards");
            }

            @Override
            public void cardActionDown() {
            }

            @Override
            public void cardActionUp() {
            }

        });
    }

    /**
     * TODO: Check this, not working
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
        menuIntent.putExtra(EXTRA_USER_ID, data.get(EXTRA_USER_ID));
        menuIntent.putExtra(EXTRA_USER_BIO, data.get(EXTRA_USER_BIO));
        menuIntent.putExtra(EXTRA_USER_INTS, data.get(EXTRA_USER_INTS));
        startActivity(menuIntent);
    }

    private void requestUsersData() {
        mUsersQueue = new LinkedList<>();
        mUsersData = new HashMap<>();

        StringResourcesHandler.executeQuery("christianGray", StringResourcesHandler.USER_CANDIDATES,
                data -> {
                    int excluded = 0;
                    int index = 0;

                    long seed = System.nanoTime();
                    Collections.shuffle(data, new Random(seed));

                    for (; index < data.size(); index++) {
                        String[] userData = data.get(index);
                        if (userData.length != USER_FIELDS) {
                            excluded++;
                            continue;
                        }
                        ImageResourcesHandler.prefetch(userData[USER_ID],
                                ImageResourcesHandler.RES_USER_IMG, getContext());
                        addUserCard(index - excluded, userData);
                    }

                    if (!isAdded()) {
                        return;
                    }

                    if (index - excluded > 0) {
                        setCardsAdapter(mFragmentView);
                        showProgress(false);
                    } else {
                        Snackdebug.showMessage("There is no more candidates. Try later", getView());
                    }
                });
    }

    private void addUserCard(int index, String[] userData) {
        Map<String, String> userMap = new HashMap<>();
        userMap.put(EXTRA_USER_NAME, userData[USER_NAME]);
        userMap.put(EXTRA_USER_AGE, userData[USER_AGE]);
        userMap.put(EXTRA_USER_ID, userData[USER_ID]);
        userMap.put(EXTRA_USER_BIO, userData[USER_BIO]);
        userMap.put(EXTRA_USER_INTS, userData[USER_INTS]);

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

        /**
         * @param params params
         * @return Task successful
         */
        @Override
        protected Boolean doInBackground(Void... params) {
            requestUsersData();
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
        }

        @Override
        protected void onCancelled() {
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

            fillUserImage(position, view);

            ViewGroup.LayoutParams layoutParams = view.getLayoutParams();
            layoutParams.height = (int) getActivity().getResources().getDimension(
                    R.dimen.activity_horizontal_margin);
            view.setLayoutParams(layoutParams);

            TextView textView = (TextView) view.findViewById(R.id.card_text);

            String item = mUsersData.get(position).get(EXTRA_USER_NAME) + " , '"
                    + mUsersData.get(position).get(EXTRA_USER_AGE) + "'";

            textView.setText(item);

            view.setOnClickListener(clickListener -> {
                String item1 = (String) getItem(position);
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Touched: " + item1);
                showActualUserData();
            });

            return view;
        }

        private void fillUserImage(int position, View context) {
            String userId = mUsersData.get(position).get(EXTRA_USER_ID); //FIXME change to user id
            ImageView imageView = (ImageView) context.findViewById(R.id.card_picture);
            ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG,
                    imageView, mContext);
        }

    }

}
