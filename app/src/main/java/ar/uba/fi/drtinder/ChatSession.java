package ar.uba.fi.drtinder;

import android.content.Intent;
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

    public static final String EXTRA_FRIEND_NAME = "friendname";
    public static final String EXTRA_FRIEND_ID = "friendid";
    public static final String EXTRA_USER_NAME = "username";
    public static final String EXTRA_USER_ID = "userid";

    private LinearLayout mMessagesLayout;

    private String mYourId;
    private String mFriendId;
    private String mFriendName;
    private String mToken;

    /**
     * TODO
     *
     * @param bundle
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
        mYourId = intent.getStringExtra(EXTRA_USER_ID);

        this.setTitle(mFriendName);

        mMessagesLayout = (LinearLayout) this.findViewById(R.id.messages);

        loadOldMessages();
        addSendListener();

        ImageView img = (ImageView) findViewById(R.id.backdrop);
        ImageResourcesHandler.fillImageResource(mFriendId, ImageResourcesHandler.RES_USER_IMG,
                mToken, img, this);

        FloatingActionButton scrollDownFB = (FloatingActionButton) this.findViewById(R.id.fab);
        assert scrollDownFB != null; //Debug assert
        scrollDownFB.setOnClickListener(listener -> scrollToLast());
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
                    addPersonalResponse(message);
                    msgView.setText("");
                    Utility.hideKeyboard(this);
                    scrollToLast();
                });
    }

    private void loadOldMessages() {
        for (int i = 0; i < 24; i++) {
            addFriendResponse("Hola, como estas?");
        }
        addPersonalResponse("Veo que estas desesperada");
        addFriendResponse("no, te parece nomas.");
        addPersonalResponse("Bueno. Si pinta sadomasoquismo, avisame.");

        scrollToLast();
    }

    private void addPersonalResponse(String message) {
        addResponse(R.layout.chat_session_you, "Tu", mYourId, message);
    }

    private void addFriendResponse(String message) {
        addResponse(R.layout.chat_session_friend, mFriendName, mFriendId, message);
    }

    private void addResponse(int layoutId, String username, String userId, String message) {
        LayoutInflater inflater = LayoutInflater.from(this);
        View layout = inflater.inflate(layoutId, null);

        BubbleImageView imageView = (BubbleImageView) layout.findViewById(R.id.chat_user_img);
        ImageResourcesHandler.fillImageResource(userId, ImageResourcesHandler.RES_USER_IMG,
                mToken, imageView, this);

        TextView nameTextView = (TextView) layout.findViewById(R.id.chat_user_name);
        nameTextView.setText(username + ":");

        TextView msgTextView = (TextView) layout.findViewById(R.id.chat_user_msg);
        msgTextView.setText(message);

        mMessagesLayout.addView(layout);
    }

    private void scrollToLast() {
        final NestedScrollView scrollview = ((NestedScrollView) findViewById(R.id.messages_lay));
        assert scrollview != null; //Debug assert
        scrollview.postDelayed(() -> scrollview.fullScroll(NestedScrollView.FOCUS_DOWN), 100);
        AppBarLayout appBarLayout = (AppBarLayout) findViewById(R.id.app_bar);
        assert appBarLayout != null; //Debug assert
        appBarLayout.setExpanded(false, true);
    }
}
