package ar.uba.fi.drtinder;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.os.AsyncTask;
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

    private static final int USER_NAME = 1;
    private static final int USER_AGE = 2;
    private static final int USER_ID = 3;
    private static final int USER_EMAIL = 4;
    private static final int USER_BIO = 5;
    private static final int USER_INTS = 7;

    private static final int USER_FIELDS = 10;

    //TODO: Remove
    private SwipeDeck mCardStack;

    private View mProgressView;

    private Queue<Map<Integer, String>> mUsersQueue;
    private Map<Integer, Map<Integer, String>> mUsersData;
    private View mFragmentView;

    /**
     * Called to have the fragment instantiate its user interface view.
     *
     * @param inflater  The LayoutInflater object that can be used to inflate any views in the fragment,
     * @param container If non-null, this is the parent view that the fragment's UI should be
     *                  attached to. The fragment should not add the view itself, but this can
     *                  be used to generate the LayoutParams of the view.
     * @param bundle    If non-null, this fragment is being re-constructed from a previous saved
     *                  state as given here.
     * @return Return the View for the fragment's UI, or null.
     */
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

        Utility.hideKeyboard((Activity) getContext());
        return mFragmentView;
    }

    private void fillCardStack() {
        Utility.showMessage("Buscando candidatos", getView(), "Ok", 100);
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

    /**
     * Called when the Fragment is visible to the user. This is generally tied to Activity.onStart
     * of the containing Activity's lifecycle.
     */
    @Override
    public void onStart() {
        super.onStart();
    }

    /**
     * Called when the Fragment is no longer started. This is generally tied to Activity.onStop of
     * the containing Activity's lifecycle.
     */
    @Override
    public void onStop() {
        super.onStop();
    }

    private void showActualUserData() {
        Intent detailsIntent = new Intent(getContext(), UserDetailsActivity.class);
        Map<Integer, String> data = mUsersQueue.peek();
        if (data == null) {
            return;
        }
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

        StringResourcesHandler.executeQuery(StringResourcesHandler.USER_CANDIDATES,
                UserHandler.getToken(),
                data -> {
                    if (data == null) {
                        Utility.showMessage("Error de conexion con el servidor", getView(), "Ok");
                        return;
                    }

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

                        userData[USER_ID] = UserHandler.getUsernameFrom(userData[USER_EMAIL]);


                        ImageResourcesHandler.prefetch(userData[USER_ID],
                                ImageResourcesHandler.RES_USER_IMG, UserHandler.getToken(),
                                getContext());
                        addUserCard(index - excluded, userData);
                    }

                    if (!isAdded()) {
                        return;
                    }

                    if (index - excluded > 0) {
                        setCardsAdapter(mFragmentView);
                    } else {
                        Utility.showMessage("De momento no hay mas candidatos", getView());
                    }
                });
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
                        new RemoteMessage.Builder("292426067795@gcm.googleapis.com")
                                .setMessageId(UserHandler.getMessageId().toString())
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
                        new RemoteMessage.Builder("292426067795@gcm.googleapis.com")
                                .setMessageId(UserHandler.getMessageId().toString())
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
                    UserHandler.getToken(), imageView, mContext);
        }

    }

}
