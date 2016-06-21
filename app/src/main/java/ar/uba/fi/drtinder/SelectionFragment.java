package ar.uba.fi.drtinder;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
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
import com.google.firebase.messaging.FirebaseMessaging;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Random;

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
 * Swipe selection fragment. Where you can choose if you like someone o not.
 */
public class SelectionFragment extends Fragment {

    private static final int USER_NAME = 0;
    private static final int USER_AGE = 1;
    private static final int USER_ID = 2;
    private static final int USER_BIO = 3;
    private static final int USER_INTS = 4;

    private static final int USER_FIELDS = 5;

    private String mToken;

    //TODO: Remove
    private SwipeDeck mCardStack;

    private View mProgressView;

    private Queue<Map<Integer, String>> mUsersQueue;
    private Map<Integer, Map<Integer, String>> mUsersData;
    private View mFragmentView;

    /**
     * TODO
     *
     * @param inflater
     * @param container
     * @param bundle
     * @return
     */
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle bundle) {
        mFragmentView = inflater.inflate(R.layout.activity_selection, container, false);

        mToken = getActivity().getIntent().getExtras().getString(MainActivity.EXTRA_TOKEN);

        mCardStack = (SwipeDeck) mFragmentView.findViewById(R.id.swipe_deck);
        mCardStack.setHardwareAccelerationEnabled(true);

        mCardStack.setLeftImage(R.id.card_nope);
        mCardStack.setRightImage(R.id.card_like);
        mProgressView = mFragmentView.findViewById(R.id.login_progress);

        setButtons(mFragmentView);

        fillCardStack();

        Utility.hideKeyboard((Activity) getContext());
        return mFragmentView;
    }

    private void fillCardStack() {
        showProgress(true);
        UsersFetchTask mAuthTask = new UsersFetchTask();
        mAuthTask.execute((Void) null);
    }

    private void setButtons(View view) {
        FloatingActionButton floatingActionButton
                = (FloatingActionButton) view.findViewById(R.id.nope_button);
        floatingActionButton.setBackgroundTintList(ColorStateList.valueOf(-1));
        floatingActionButton.setRippleColor(getResources().getColor(R.color.colorAccentT));
        floatingActionButton.setOnClickListener(newView -> mCardStack.swipeTopCardLeft(90));

        floatingActionButton = (FloatingActionButton) view.findViewById(R.id.like_button);
        floatingActionButton.setBackgroundTintList(ColorStateList.valueOf(-1));
        floatingActionButton.setRippleColor(getResources().getColor(R.color.colorAccentT));
        floatingActionButton.setOnClickListener(newView -> mCardStack.swipeTopCardRight(90));
    }

    private void setCardsAdapter(View view) {
        SwipeDeckAdapter mCardDeckAdapter = new SwipeDeckAdapter(view.getContext());
        mCardStack.setAdapter(mCardDeckAdapter);

        mCardStack.setEventCallback(new SwipeDeck.SwipeEventCallback() {
            @Override
            public void cardSwipedLeft(int position) {
                if (mUsersQueue.isEmpty()) {
                    return;
                }
                Map<Integer, String> data = mUsersQueue.remove();

                FirebaseMessaging.getInstance().send(
                        new RemoteMessage.Builder(UserHandler.getUserEmail())
                                .setMessageId(" ")
                                .addData("user", UserHandler.getUsername())
                                .addData("candidate", data.get(USER_ID))
                                .addData("liked", "no")
                                .build());

                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Rejected " + data.get(USER_NAME));
                ImageResourcesHandler.freeCachedResource(data.get(USER_ID),
                        ImageResourcesHandler.RES_USER_IMG, getContext());
            }

            @Override
            public void cardSwipedRight(int position) {
                if (mUsersQueue.isEmpty()) {
                    return;
                }
                Map<Integer, String> data = mUsersQueue.remove();

                FirebaseMessaging.getInstance().send(
                        new RemoteMessage.Builder(UserHandler.getUserEmail())
                                .setMessageId(" ")
                                .addData("user", UserHandler.getUsername())
                                .addData("candidate", data.get(USER_ID))
                                .addData("liked", "yes")
                                .build());

                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Liked  " + data.get(USER_NAME));
                ImageResourcesHandler.freeCachedResource(data.get(USER_ID),
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
     * TODO: Check this, not working properly
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
        Intent detailsIntent = new Intent(getContext(), UserDetailsActivity.class);
        Map<Integer, String> data = mUsersQueue.peek();
        detailsIntent.putExtra(UserDetailsActivity.EXTRA_USER_NAME, data.get(USER_NAME));
        detailsIntent.putExtra(UserDetailsActivity.EXTRA_USER_AGE, data.get(USER_AGE));
        detailsIntent.putExtra(UserDetailsActivity.EXTRA_USER_ID, data.get(USER_ID));
        detailsIntent.putExtra(UserDetailsActivity.EXTRA_USER_BIO, data.get(USER_BIO));
        detailsIntent.putExtra(UserDetailsActivity.EXTRA_USER_INTS, data.get(USER_INTS));
        startActivity(detailsIntent);
    }

    private void requestUsersData() {
        mUsersQueue = new LinkedList<>();
        mUsersData = new HashMap<>();

        StringResourcesHandler.executeQuery(StringResourcesHandler.USER_CANDIDATES, mToken,
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
                                ImageResourcesHandler.RES_USER_IMG, mToken, getContext());
                        addUserCard(index - excluded, userData);
                    }

                    if (!isAdded()) {
                        return;
                    }

                    if (index - excluded > 0) {
                        setCardsAdapter(mFragmentView);
                        showProgress(false);
                    } else {
                        Utility.showMessage("There is no more candidates. Try later", getView());
                    }
                });
    }

    private void addUserCard(int index, String[] userData) {
        Map<Integer, String> userMap = new HashMap<>();
        userMap.put(USER_NAME, userData[USER_NAME]);
        userMap.put(USER_AGE, userData[USER_AGE]);
        userMap.put(USER_ID, userData[USER_ID]);
        userMap.put(USER_BIO, userData[USER_BIO]);
        userMap.put(USER_INTS, userData[USER_INTS]);

        mUsersData.put(index, userMap);
        mUsersQueue.add(userMap);
    }

    /**
     * Dummy onStart
     */
    @Override
    public void onStart() {
        super.onStart();
    }

    /**
     * Dummy onStop
     */
    @Override
    public void onStop() {
        super.onStop();
    }

    /**
     * Represents an asynchronous login/registration task used to authenticate
     * the user.
     */
    private class UsersFetchTask extends AsyncTask<Void, Void, Boolean> {

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
    private class SwipeDeckAdapter extends BaseAdapter {

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
            return mUsersData.get(position).get(USER_NAME);
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

            String item = mUsersData.get(position).get(USER_NAME) + " ("
                    + mUsersData.get(position).get(USER_AGE) + ")";

            textView.setText(item);

            view.setOnClickListener(clickListener -> {
                String item1 = (String) getItem(position);
                DrTinderLogger.writeLog(DrTinderLogger.INFO, "Touched: " + item1);
                showActualUserData();
            });

            return view;
        }

        private void fillUserImage(int position, View context) {
            String userId = mUsersData.get(position).get(USER_ID);
            ImageView imageView = (ImageView) context.findViewById(R.id.card_picture);
            ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG,
                    mToken, imageView, mContext);
        }

    }

}
