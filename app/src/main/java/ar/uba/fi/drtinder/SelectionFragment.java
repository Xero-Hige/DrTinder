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

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

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
    public static final String EXTRA_USER_IMAGE = "img";
    /**
     * User biography key in extra map
     */
    public static final String EXTRA_USER_BIO = "bio";

    private static final int USER_NAME = 0;
    private static final int USER_AGE = 1;
    private static final int USER_ID = 2;
    private static final int USER_BIO = 3;
    //TODO: Remove
    private SwipeDeck mCardStack;

    private boolean mShowSwipeResult = true;
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
                fillCardStack();
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
        menuIntent.putExtra(EXTRA_USER_IMAGE, data.get(EXTRA_USER_IMAGE));
        menuIntent.putExtra(EXTRA_USER_BIO, data.get(EXTRA_USER_BIO));
        startActivity(menuIntent);
    }

    private void requestUsersData() {
        mUsersQueue = new LinkedList<>();
        mUsersData = new HashMap<>();

        StringResourcesHandler.executeQuery("christianGray", StringResourcesHandler.USER_CANDIDATES,
                data -> {
                    int excluded = 0;
                    int i = 0;
                    for (; i < data.size(); i++) {
                        String[] userData = data.get(i);
                        if (userData.length != 4) {
                            excluded++;
                            continue;
                        }
                        ImageResourcesHandler.prefetch(userData[USER_ID], ImageResourcesHandler.RES_USER_IMG, getContext());
                        addUserCard(i - excluded, userData[USER_NAME], userData[USER_AGE], userData[USER_ID], userData[USER_BIO]);
                    }
                    if (i - excluded > 0) {
                        setCardsAdapter(mFragmentView);
                        showProgress(false);
                    } else {
                        Snackdebug.showMessage("Really, no more cats", getView());
                    }
                });
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
                Log.i("MainActivity", item1);
            });

            return view;
        }

        private void fillUserImage(int position, View context) {
            String userId = mUsersData.get(position).get(EXTRA_USER_IMAGE); //FIXME change to user id
            ImageView imageView = (ImageView) context.findViewById(R.id.card_picture);
            ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG, imageView, mContext);
        }


    }

}
