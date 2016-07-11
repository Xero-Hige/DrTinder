package ar.uba.fi.drtinder;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.AppBarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.widget.NestedScrollView;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.github.siyamed.shapeimageview.BubbleImageView;

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
 * Chat session activity. Represents a chat session between 2 users.
 */
public class ChatSession extends AppCompatActivity {

    /**
     * Intent extra field: Friend name
     */
    public static final String EXTRA_FRIEND_NAME = "friendname";
    /**
     * Intent extra field: Friend user id
     */
    public static final String EXTRA_FRIEND_ID = "friendid";

    private static final int DATA_FIELDS = 2;

    private LinearLayout mMessagesLayout;

    private String mFriendId;
    private String mFriendName;


    /**
     * Perform initialization of all fragments and loaders.
     *
     * @param bundle If the activity is being re-initialized after previously being shut down then
     *               this Bundle contains the data it most recently supplied in
     *               onSaveInstanceState(Bundle). Note: Otherwise it is null.
     */
    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.activity_chat_session);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent intent = getIntent();
        mFriendName = intent.getStringExtra(EXTRA_FRIEND_NAME);
        mFriendId = intent.getStringExtra(EXTRA_FRIEND_ID);

        this.setTitle(mFriendName);

        mMessagesLayout = (LinearLayout) this.findViewById(R.id.messages);

        loadOldMessages();
        addSendListener();

        ImageView img = (ImageView) findViewById(R.id.backdrop);
        ImageResourcesHandler.fillImageResource(mFriendId, ImageResourcesHandler.RES_USER_IMG,
                UserHandler.getToken(), img, this);

        FloatingActionButton scrollDownFB = (FloatingActionButton) this.findViewById(R.id.fab);
        assert scrollDownFB != null; //Debug assert
        scrollDownFB.setOnClickListener(listener -> scrollToLast());
    }

    /**
     * Called when you are no longer visible to the user. You will next receive either onRestart(),
     * onDestroy(), or nothing, depending on later user activity.
     * <p>
     * Derived classes must call through to the super class's implementation of this method.
     * If they do not, an exception will be thrown.
     */
    @Override
    public void onStop() {
        super.onStop();
        MessagesListener.stopListening();
    }

    private void addSendListener() {
        ImageButton sendButton = (ImageButton) this.findViewById(R.id.send);
        assert sendButton != null; //Debug assert
        sendButton.setOnClickListener(
                listener -> {
                    EditText msgView = (EditText) findViewById(R.id.message);

                    assert msgView != null; //Debug assert
                    String message = msgView.getText().toString();

                    if (message.isEmpty()) {
                        return;
                    }

                    sendMessage(message);

                    msgView.setText("");
                    Utility.hideKeyboard(this);
                    scrollToLast();
                });
    }

    private void sendMessage(String message) {
        SendMessageTask task = new SendMessageTask(mFriendId, message);
        task.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void loadOldMessages() {
        StringResourcesHandler.executeQuery(mFriendId, StringResourcesHandler.USER_CHAT, UserHandler.getToken()
                , data -> {
                    for (int index = 0; index < data.size(); index++) {
                        String[] messageData = data.get(index);
                        if (messageData.length != DATA_FIELDS) {
                            DrTinderLogger.writeLog(DrTinderLogger.WARN, "Message length mismatch");
                            continue;
                        }

                        String senderId = messageData[0];
                        String messageText = messageData[1];

                        if (senderId.equals(UserHandler.getUsername())) {
                            addPersonalResponse(messageText);
                            continue;
                        }

                        if (senderId.equals(mFriendId)) {
                            addFriendResponse(messageText);
                            continue;
                        }

                        DrTinderLogger.writeLog(DrTinderLogger.ERRO,
                                "Received unknown sender id " + senderId);
                    }
                    scrollToLast();
                });
    }

    private void scrollToLast() {
        final NestedScrollView scrollview = ((NestedScrollView) findViewById(R.id.messages_lay));
        assert scrollview != null; //Debug assert
        scrollview.postDelayed(() -> scrollview.fullScroll(NestedScrollView.FOCUS_DOWN), 100);
        AppBarLayout appBarLayout = (AppBarLayout) findViewById(R.id.app_bar);
        assert appBarLayout != null; //Debug assert
        appBarLayout.setExpanded(false, true);
    }

    private void addPersonalResponse(String message) {
        addResponse(R.layout.chat_session_you, "Tu", UserHandler.getUsername(), message);
    }

    private void addResponse(int layoutId, String username, String userId, String message) {
        LayoutInflater inflater = LayoutInflater.from(this);
        View layout = inflater.inflate(layoutId, null);

        BubbleImageView imageView = (BubbleImageView) layout.findViewById(R.id.chat_user_img);
        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG,
                UserHandler.getToken(), imageView, this);

        TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText(username + ":");

        TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText(message);

        runOnUiThread(() -> mMessagesLayout.addView(layout));
    }

    private void addFriendResponse(String message) {
        addResponse(R.layout.chat_session_friend, mFriendName, mFriendId, message);
    }

    /**
     * Called after onCreate(Bundle) — or after onRestart() when the activity had been stopped, but
     * is now again being displayed to the user. It will be followed by onResume().
     * <p>
     * Derived classes must call through to the super class's implementation of this method.
     * If they do not, an exception will be thrown.
     */
    @Override
    protected void onStart() {
        super.onStart();
        MessagesListener.startListening(UserHandler.getToken(), mFriendId
                , this);
    }

    /**
     * Adds an user response to the chat log
     *
     * @param message Message to add
     * @param userId  Sender id
     */
    public void addResponse(String message, String userId) {
        if (userId.equals(mFriendId)) {
            addFriendResponse(message);
            return;
        }
        if (userId.equals(UserHandler.getUsername())) {
            addPersonalResponse(message);
            return;
        }
        DrTinderLogger.writeLog(DrTinderLogger.WARN, "Response from unknown id:"
                + userId + " - " + message);
    }

    private class SendMessageTask extends AsyncTask<Void, Void, Boolean> {

        private final String mCandidateId;
        private final String mMessage;

        SendMessageTask(String candidateId, String message) {
            mCandidateId = candidateId;
            mMessage = message;
        }

        /**
         * @param params params
         * @return Task successful
         */
        @Override
        protected Boolean doInBackground(Void... params) {
            return UserHandler.sendMessage(UserHandler.getToken(), mCandidateId, mMessage);
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (success) {
                DrTinderLogger.writeLog(DrTinderLogger.DEBG, "Message sent: " + mMessage);
                return;
            }
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Failed to send: " + mMessage);

        }

        @Override
        protected void onCancelled() {
        }
    }
}
